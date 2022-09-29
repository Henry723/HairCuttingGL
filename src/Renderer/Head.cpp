#include "Head.h"

Head::Head(const char* modelSource, const char* texSource)
{
	LoadHeadModel(modelSource);
    LoadTexture(texSource, textureID);
}

Head::~Head()
{
}

void Head::Draw(Shader& shader, unsigned int textureID)
{
    for (unsigned int i = 0; i < meshes.size(); i++)
        meshes[i].Draw(shader, textureID);
}

void Head::LoadHeadModel(const char* modelSource)
{
    //cout << "Loading model" << endl;
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(modelSource, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		cout << "ERROR::ASSIMP::" << importer.GetErrorString() << endl;
		return;
	}

	ProcessNode(scene->mRootNode, scene);
    //cout << "Completed" << endl;
}

void Head::ProcessNode(aiNode* node, const aiScene* scene)
{
    //cout << "node num mesh:" << node->mNumMeshes << endl;
    // process each mesh located at the current node
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        // the node object only contains indices to index the actual objects in the scene. 
        // the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(ProcessMesh(mesh, scene));
    }
    // after we've processed all of the meshes (if any) we then recursively process each of the children nodes
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        ProcessNode(node->mChildren[i], scene);
    }
}

Mesh Head::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
    //cout << "processing mesh" << endl;
    //Data to fill in
    vector<Vertex> vertices;
    vector<unsigned int> indices;
    //vector<Texture> textures;

    // walk through each of the mesh's vertices
    for (unsigned int i = 0; i < mesh->mNumVertices; i++) 
    {
        Vertex vertex;
        glm::vec3 vector; // we declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
        // positions
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.Position = vector;
        //cout << "vector x:" << vector.x << endl;
        // normals
        if (mesh->HasNormals())
        {
            //cout << "get normals:"<< endl;
            vector.x = mesh->mNormals[i].x;
            vector.y = mesh->mNormals[i].y;
            vector.z = mesh->mNormals[i].z;
            vertex.Normal = vector;
            //cout << vector.x << " " << vector.y << " " << vector.z << endl;
        }
        // texture coordinates
        if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
        {
            glm::vec2 vec;
            // a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
            // use models where a vertex can have multiple texture coordinates so we always take the first set (0).
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.TexCoord = vec;
        }
        else {
            vertex.TexCoord = glm::vec2(0.0f, 0.0f);
        }
        vertices.push_back(vertex);
    }

    // now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        // retrieve all indices of the face and store them in the indices vector
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }
    cout << "vertices size: " << vertices.size() << endl;
    cout << "Finish processing mesh" << endl;
    return Mesh(vertices, indices);
}

void Head::LoadTexture(const char* texSource, unsigned int& textureID)
{
    // Load image, create texture and generate mipmaps
    int width, height, nrChannels;

    glGenTextures(1, &textureID); // Takes in how many textures are required, stores them in an unsigned int array
    glActiveTexture(GL_TEXTURE0); // Activate the texture unit before binding it. Default is 0.
    glBindTexture(GL_TEXTURE_2D, textureID);
    
    // Set how textures will be wrapped if a vertex falls outside the given coordinates
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // Set how texels are interpolated when scaling the image up or down
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST); // Textures downscaled
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // Textures upscaled
    
    unsigned char* textureData = stbi_load(texSource, &width, &height, &nrChannels, 0);
    if (textureData)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(textureData);
}
