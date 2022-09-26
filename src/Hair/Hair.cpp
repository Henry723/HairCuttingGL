#include "Hair.h"

Hair::Hair(vec3 contolPos1, vec3 contolPos2, vec3 contolPos3, vec3 contolPos4, int numLinks, const char* texSource = NULL)
{
	cPos1 = contolPos1;
	cPos2 = contolPos2;
	cPos3 = contolPos3;
	cPos4 = contolPos4;
    hairPosition = contolPos1;

	nLinks = numLinks;
	cardWidth = 0.5f;

    // Make sure how many nodes to generate when calculating links
    
    //Creates floating point error
    // --------------------------
    //float resolution = (float)1 / nLinks;
    //printf("Resolution: %f\n", resolution);
    //for (float t = 0; t < 1 + resolution; t += resolution) {
    //    if (t >= 1 && !reached)
    //    {
    //        CubicBezier(cPos1, cPos2, cPos3, cPos4, 1);
    //        reached = true;
    //    }
    //    else
    //    {
    //        CubicBezier(cPos1, cPos2, cPos3, cPos4, t);
    //    }
    //    count++;
    //}
    // --------------------------

    for (int n = 0; n <= nLinks; n++) {
        if (n >= nLinks)
        {
            CubicBezier(cPos1, cPos2, cPos3, cPos4, 1); // Make sure ends at t = 1
        }
        else 
        {
            float t = (float)n / nLinks;
            CubicBezier(cPos1, cPos2, cPos3, cPos4, t);
        }
    }

    //printf("Node count: %i\n", count);
    //CubicBezier(vec3(0, 0, 0), vec3(0, 1, 0), vec3(1, 1, 0), vec3(1, 0, 0), 0.5);

    //Start normalizing the total hair distance on the group of links and assign their uv's
    CreateNormalizedLinks(hairLinks);

    //Push all node data and texture coordinates in hairVerticies

	Setup();
	LoadTexture(texSource, hairTextureID);
}

Hair::~Hair()
{
    cout << "Hair destructor was called"<< endl;
    for (HairNode* node : hairNodes)
    {
        //nodeCount--;//delete soon
        delete node;
    }
   hairNodes.clear();

   for (HairLink* link : hairLinks)
   {
       delete link;
   }
   hairLinks.clear();

   glDeleteVertexArrays(1, &hairVAO);
   glDeleteBuffers(1, &hairVBO);
}

void Hair::Setup()
{
    glGenVertexArrays(1, &hairVAO);
    glGenBuffers(1, &hairVBO);
    glBindVertexArray(hairVAO);
    glBindBuffer(GL_ARRAY_BUFFER, hairVBO);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(hairVertices), hairVertices, GL_STATIC_DRAW);

    glBufferData(GL_ARRAY_BUFFER, v_hairVertices.size() * sizeof(float), &v_hairVertices[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glBindVertexArray(0);
}

void Hair::LoadTexture(const char* texSource, unsigned int& textureID)
{
    // load image, create texture and generate mipmaps
    int width, height, nrChannels;

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glGenerateMipmap(GL_TEXTURE_2D);

    // Set how textures will be wrapped if a vertex falls outside the given coordinates
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // Set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    unsigned char* textureData = stbi_load(texSource, &width, &height, &nrChannels, 0);
    if (textureData)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(textureData);
}

void Hair::CubicBezier(vec3 contolPos1, vec3 contolPos2, vec3 contolPos3, vec3 contolPos4, float t)
{
    //Squared and cubed time step
    float t2 = t * t;
    float t3 = t2 * t;
    //printf("t: %f, t2: %f, t3: %f\n", t, t2, t3);

    float mt = 1 - t;
    float mt2 = mt * mt;
    float mt3 = mt2 * mt;

    //Bezier's method
    float x = (mt3 * contolPos1.x) + (3 * t * mt2 * contolPos2.x) + (3 * t2 * mt * contolPos3.x) + (t3 * contolPos4.x);
    float y = (mt3 * contolPos1.y) + (3 * t * mt2 * contolPos2.y) + (3 * t2 * mt * contolPos3.y) + (t3 * contolPos4.y);
    float z = (mt3 * contolPos1.z) + (3 * t * mt2 * contolPos2.z) + (3 * t2 * mt * contolPos3.z) + (t3 * contolPos4.z);
    //printf("t: %f [x: %f, y: %f, z: %f]\n", t, x, y, z);

    GenBezNode(vec3(x, y, z));
}

void Hair::GenBezNode(vec3 nodePos)
{
    //If the first Node at t = 0 is placed, make it pinned and will not affected by physics
    if (hairNodes.size() == 0) {
        HairNode* rootNode = new HairNode(nodePos);
        rootNode->PinHair();
        hairNodes.push_back(rootNode);
    }
    else
    {
        size_t currNodeSize = hairNodes.size();

        // Create nodes that is not root nodes
        HairNode* hairNode = new HairNode(nodePos);
        hairNode->PinHair();
        hairNodes.push_back(hairNode);

        //If there are 2 or more nodes, start linking the previous node and the current node
        if (hairNodes.size() >= 2) {
            LinkNodes(hairNodes.at((size_t)currNodeSize - 1), hairNodes.at(currNodeSize));
            linkCount++;
        }
    }
    //nodeCount++;//delete soon
}

void Hair::LinkNodes(HairNode* node1, HairNode* node2)
{
    HairLink* link = new HairLink(node1, node2, true);
    link->SetID(linkCount);
    hairLinks.push_back(link);
}

void Hair::CreateNormalizedLinks(vector<HairLink*>& hairLinks)
{
    // Total up the lengths for the links for this hair.
    float totalLength = 0.0f;
    //float u = 0.0f;
    float v = 0.0f;

    for (HairLink* link : hairLinks)
    {
        totalLength += link->GetLength();
    }
    //printf("total length: %f\n", totalLength);

    // Assign every link with their starting and ending uvs
    for (HairLink* link : hairLinks)
    {
        link->SetStartUV(0.0f, v);
        v += link->GetLength() / totalLength;
        link->SetEndUV(1.0f, v);

        //Create mesh along the way
       CreateHairMesh(link->GetNode1(),link->GetNode2(), link->GetLength(), cardWidth, link->GetStartU(), link->GetStartV(), link->GetEndU(), link->GetEndV());
       link->SetBoxMinMax(cardWidth/2);
    }
    //printf("total u: %f\n", u);
    //printf("total v: %f\n", v);

    //for (int i = 0; i < v_hairVertices.size(); i += 5) {
    //    printf("%f, %f, %f, %f, %f,\n", v_hairVertices.at(i), v_hairVertices.at(i+1), v_hairVertices.at(i+2), v_hairVertices.at(i+3), v_hairVertices.at(i+4));
    //}
}

void Hair::CreateHairMesh(HairNode* node1, HairNode* node2, float length, float width, float startU, float startV, float endU, float endV)
{
    //printf("\nCreating Hair Mesh \n");
    // Extrude using width
    float halfWidth = width / 2;

    // Extrude towards x axis first to show in front for demo / futher testing
    // First vertex data (position)
    v_hairVertices.push_back(node1->position.x - halfWidth);
    v_hairVertices.push_back(node1->position.y);
    v_hairVertices.push_back(node1->position.z);

    // First vertex data (uv)
    v_hairVertices.push_back(startU);
    v_hairVertices.push_back(startV);

    // Second vertex data (position)
    v_hairVertices.push_back(node2->position.x - halfWidth);
    v_hairVertices.push_back(node2->position.y);
    v_hairVertices.push_back(node2->position.z);

    // Second vertex data (uv)
    v_hairVertices.push_back(startU);
    v_hairVertices.push_back(endV);

    // Third vertex data (position)
    v_hairVertices.push_back(node2->position.x + halfWidth);
    v_hairVertices.push_back(node2->position.y);
    v_hairVertices.push_back(node2->position.z);

    // Third vertex data (uv)
    v_hairVertices.push_back(endU);
    v_hairVertices.push_back(endV);

    // First vertex data (position)
    v_hairVertices.push_back(node1->position.x - halfWidth);
    v_hairVertices.push_back(node1->position.y);
    v_hairVertices.push_back(node1->position.z);

    // First vertex data (uv)
    v_hairVertices.push_back(startU);
    v_hairVertices.push_back(startV);

    // Third vertex data (position)
    v_hairVertices.push_back(node2->position.x + halfWidth);
    v_hairVertices.push_back(node2->position.y);
    v_hairVertices.push_back(node2->position.z);

    // Third vertex data (uv)
    v_hairVertices.push_back(endU);
    v_hairVertices.push_back(endV);

    // Fourth vertex data (position)
    v_hairVertices.push_back(node1->position.x + halfWidth);
    v_hairVertices.push_back(node1->position.y);
    v_hairVertices.push_back(node1->position.z);

    // Fourth vertex data (uv)
    v_hairVertices.push_back(endU);
    v_hairVertices.push_back(startV);
}


void Hair::UpdateBufferData()
{
    glBindBuffer(GL_ARRAY_BUFFER, hairVBO);
    glBufferData(GL_ARRAY_BUFFER, v_hairVertices.size() * sizeof(float), &v_hairVertices[0], GL_STATIC_DRAW);
}

void Hair::UpdateHairMesh(HairNode* node1, HairNode* node2, float width, int index)
{
    // Extrude using width
    float halfWidth = width / 2;

    // Only updates the position data for now
    //for (int i = index * MESH_ATTRIBUTE_SIZE; i < v_hairVertices.size(); i += 30) {
    //    // First vertex position
    //    v_hairVertices.at(i) = (node1->position.x + halfWidth);
    //    v_hairVertices.at((size_t)i + 1) = node1->position.y;
    //    v_hairVertices.at((size_t)i + 2) = node1->position.z;

    //    // Second vertex position
    //    v_hairVertices.at((size_t)i + 5) = (node2->position.x + halfWidth);
    //    v_hairVertices.at((size_t)i + 6) = node2->position.y;
    //    v_hairVertices.at((size_t)i + 7) = node2->position.z;
    //}
    // First vertex position
    v_hairVertices.at((size_t)index * MESH_ATTRIBUTE_SIZE) = (node1->position.x - halfWidth);
    v_hairVertices.at((size_t)index * MESH_ATTRIBUTE_SIZE + 1) = node1->position.y;
    v_hairVertices.at((size_t)index * MESH_ATTRIBUTE_SIZE + 2) = node1->position.z;

    // Second vertex position
    v_hairVertices.at((size_t)index * MESH_ATTRIBUTE_SIZE + 5) = (node2->position.x - halfWidth);
    v_hairVertices.at((size_t)index * MESH_ATTRIBUTE_SIZE + 6) = node2->position.y;
    v_hairVertices.at((size_t)index * MESH_ATTRIBUTE_SIZE + 7) = node2->position.z;

    // Third vertex position
    v_hairVertices.at((size_t)index * MESH_ATTRIBUTE_SIZE + 10) = (node2->position.x + halfWidth);
    v_hairVertices.at((size_t)index * MESH_ATTRIBUTE_SIZE + 11) = node2->position.y;
    v_hairVertices.at((size_t)index * MESH_ATTRIBUTE_SIZE + 12) = node2->position.z;
    
    // First vertex position
    v_hairVertices.at((size_t)index * MESH_ATTRIBUTE_SIZE + 15) = (node1->position.x - halfWidth);
    v_hairVertices.at((size_t)index * MESH_ATTRIBUTE_SIZE + 16) = node1->position.y;
    v_hairVertices.at((size_t)index * MESH_ATTRIBUTE_SIZE + 17) = node1->position.z;

    // Third vertex position
    v_hairVertices.at((size_t)index * MESH_ATTRIBUTE_SIZE + 20) = (node2->position.x + halfWidth);
    v_hairVertices.at((size_t)index * MESH_ATTRIBUTE_SIZE + 21) = node2->position.y;
    v_hairVertices.at((size_t)index * MESH_ATTRIBUTE_SIZE + 22) = node2->position.z;

    // Fourth vertex position
    v_hairVertices.at((size_t)index * MESH_ATTRIBUTE_SIZE + 25) = (node1->position.x + halfWidth);
    v_hairVertices.at((size_t)index * MESH_ATTRIBUTE_SIZE + 26) = node1->position.y;
    v_hairVertices.at((size_t)index * MESH_ATTRIBUTE_SIZE + 27) = node1->position.z;
}

void Hair::DrawHair(Shader& shader, unsigned int textureID)
{
    glBindVertexArray(hairVAO);
    glBindTexture(GL_TEXTURE_2D, hairTextureID);
    for (int i = 0; i < hairLinks.size() * 6; i += 6) {
        glDrawArrays(GL_TRIANGLES, i, i + 6);
    }  
}

void Hair::DeleteLink(size_t index)
{
    //Check if the cut location is out of bounds
    if (index >= hairLinks.size()) {
        printf("Out of bounds\n");
        return;
    }

    delete hairLinks.at(index);
    hairLinks.erase(hairLinks.begin()+ index);
    v_hairVertices.erase(v_hairVertices.begin() + index * MESH_ATTRIBUTE_SIZE, v_hairVertices.begin() + (index * MESH_ATTRIBUTE_SIZE) + MESH_ATTRIBUTE_SIZE);

    //Nodes always have 1 more than links
    for (size_t i = index + 1; i < hairNodes.size(); i++) {
        HairNode* hairNode = (HairNode*)hairNodes.at(i);
        hairNode->UnpinHair();
    }
    UpdateBufferData();
}

void Hair::PushHairVerticies(float value)
{
    v_hairVertices.push_back(value);
}

//Hair physics 
void Hair::UpdatePhysics(float fixedDeltaTimeS)
{
    for (int i = 0; i < hairNodes.size(); i++) {
        HairNode* hairNode = (HairNode*)hairNodes.at(i);
        hairNode->UpdatePhysics(fixedDeltaTimeS);
    }
    for (int i = 0; i < hairLinks.size(); i++) {
        //Update mesh along the way
        HairLink* hairLink = (HairLink*)hairLinks.at(i);
        UpdateHairMesh(hairLink->GetNode1(), hairLink->GetNode2(), cardWidth, i);
        hairLink->UpdateBoundingBox(cardWidth/2);
    }
    //for (HairLink* link : hairLinks)
    //{
    //    //Update mesh along the way
    //    UpdateHairMesh(link->GetNode1(), link->GetNode2(), cardWidth);
    //}
    UpdateBufferData();
}

void Hair::AABB_Test(vec3 rayOrigin, vec3 rayDir)
{
    for (HairLink* link : hairLinks)
    {
        if (link->AABB_Test(rayOrigin, rayDir)) {
            DeleteLink(link->GetID());
        }
    }
}

