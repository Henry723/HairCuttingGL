#include "Hair.h"

Hair::Hair(vec3 contolPos1, vec3 contolPos2, vec3 contolPos3, vec3 contolPos4, int numLinks, const char* texSource = NULL)
{
	cPos1 = contolPos1;
	cPos2 = contolPos2;
	cPos3 = contolPos3;
	cPos4 = contolPos4;

	nLinks = numLinks;
	cardWidth = 0.0f;

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
            CubicBezier(cPos1, cPos2, cPos3, cPos4, 1);
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

	Setup();
	LoadTexture(texSource, hairTextureID);
}

Hair::~Hair()
{
    cout << "Hair destructor was called"<< endl;
    for (HairNode* node : hairNodes)
    {
        nodeCount--;//delete soon
        delete node;
    }
   hairNodes.clear();
}

void Hair::Setup()
{
    glGenVertexArrays(1, &hairVAO);
    glGenBuffers(1, &hairVBO);
    glBindVertexArray(hairVAO);
    glBindBuffer(GL_ARRAY_BUFFER, hairVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(hairVertices), hairVertices, GL_STATIC_DRAW);
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
        hairNodes.push_back(new HairNode(nodePos));

        //If there are 2 or more nodes, start linking the previous node and the current node
        if (hairNodes.size() >= 2) {
            LinkNodes(hairNodes.at((size_t)currNodeSize - 1), hairNodes.at(currNodeSize));
            linkCount++;//delete soon
        }
    }
    nodeCount++;//delete soon
}

void Hair::LinkNodes(HairNode* node1, HairNode* node2)
{
    hairLinks.push_back(new HairLink(node1, node2, true));
}

void Hair::DrawHair(Shader& shader, unsigned int textureID)
{
    glBindVertexArray(hairVAO);
    glBindTexture(GL_TEXTURE_2D, hairTextureID);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}
