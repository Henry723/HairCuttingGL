#pragma once
#include <glad/glad.h> // Always include glad first to get the OpenGL headers required by glfw

// Stbi includes
#include "../Renderer/stb_image.h"

#include "HairLink.h"
#include <vector>

using namespace std;
using glm::vec3;

/**
To Dos: 
1. Create HairNodes from 4 control points to begin with using cubic bezier curve, step determined by number of links
eg. A long hair with 2 links, each segment of hair is 1/2 (links). Hence generating nodes at position 0 , 1/5, 1 on a straight line.
The more links the hair has the better the curve

2. Link HairNodes with HairLink while generating using bezier curve.

3. Method of drawing at HairLinks, HairNodes(optional)

4. Links will create mesh

5. Attach texture based off of uv space (0 to 1) of each rectangular mesh.
**/

const float cardWidth = 3.0f;
class Hair
{
public:
    unsigned int hairVAO;
    unsigned int hairTextureID;
    vector<vec3> hairPosition{
        vec3(-1.5f, 0.0f, -0.48f),
        vec3(1.5f, 0.0f, 0.51f),
        vec3(0.5f, 0.0f, 0.7f),
        vec3(-0.3f, 0.0f, -2.3f),
        vec3(0.5f, 0.0f, -0.6f)
    };

	Hair(vec3 contolPos1, vec3 contolPos2, vec3 contolPos3, vec3 contolPos4, int numLinks, const char* texSource);
	~Hair();

private:
	vector<HairLink> links;
	vec3 cPos1, cPos2, cPos3, cPos4;
	int nLinks;

    unsigned int hairVBO;

    float hairVertices[30] = {
        // positions         // texture Coords (swapped y coordinates because texture is flipped upside down)
        0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
        0.0f, -0.5f,  0.0f,  0.0f,  1.0f,
        0.25f, -0.5f,  0.0f,  1.0f,  1.0f,

        0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
        0.25f, -0.5f,  0.0f,  1.0f,  1.0f,
        0.25f,  0.5f,  0.0f,  1.0f,  0.0f
    };
    // World space positions of hair
	float cardWidth = 0;
	vec3 pos, rot;

    void Setup();
	void LoadTexture(const char* texSource, unsigned int& textureID);
	void GenBezNode();
};

inline Hair::Hair(vec3 contolPos1, vec3 contolPos2, vec3 contolPos3, vec3 contolPos4, int numLinks = 1, const char* texSource = NULL)
{
    

	cPos1 = contolPos1; 
	cPos2 = contolPos2;
	cPos3 = contolPos3;
	cPos4 = contolPos4;

	nLinks = numLinks;
	cardWidth = 0.0f;

    Setup();

    LoadTexture(texSource, hairTextureID);
}

inline Hair::~Hair()
{
}



inline void Hair::Setup()
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

inline void Hair::LoadTexture(const char* texSource, unsigned int& textureID)
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

inline void Hair::GenBezNode()
{

}
