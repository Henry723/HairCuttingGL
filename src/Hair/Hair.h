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

//const float cardWidth = 3.0f;
const static int MESH_ATTRIBUTE_SIZE = 30;

class Hair
{
public:
    unsigned int hairVAO;
    unsigned int hairTextureID;

    vec3 hairPosition;
    //int nodeCount = 0;

	Hair(vec3 contolPos1, vec3 contolPos2, vec3 contolPos3, vec3 contolPos4, int numLinks, const char* texSource);
	~Hair();

    void PushHairVerticies(float value);
    void DrawHair(Shader& shader, unsigned int textureID);
    void DeleteLink(size_t index);

    void UpdatePhysics(float fixedDeltaTimeS);

    void AABB_Test(vec3 rayOrigin, vec3 rayDir);

private:
    vector<HairNode*> hairNodes;
	vector<HairLink*> hairLinks;
	vec3 cPos1, cPos2, cPos3, cPos4;
	int nLinks;
    int linkCount = 0;

    unsigned int hairVBO;

    //float hairVertices[30] = {
    //    // positions         // texture Coords (swapped y coordinates because texture is flipped upside down)
    //    0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
    //    0.0f, -0.5f,  0.0f,  0.0f,  1.0f,
    //    0.25f, -0.5f,  0.0f,  1.0f,  1.0f,

    //    0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
    //    0.25f, -0.5f,  0.0f,  1.0f,  1.0f,
    //    0.25f,  0.5f,  0.0f,  1.0f,  0.0f
    //};

    //float hairVertices[30] = {
    //    // positions         // texture Coords (swapped y coordinates because texture is flipped upside down)
    //    -0.25f,  1.0f,  0.0f,  0.0f,  0.0f,
    //    -0.25f, -1.0f,  0.0f,  0.0f,  1.0f,
    //    0.25f, -1.0f,  0.0f,  1.0f,  1.0f,

    //    -0.25f,  1.0f,  0.0f,  0.0f,  0.0f,
    //    0.25f, -1.0f,  0.0f,  1.0f,  1.0f,
    //    0.25f,  1.0f,  0.0f,  1.0f,  0.0f
    //};

    ////UV here correct
    //float hairVertices[30] = {
    //    // positions         // texture Coords (swapped y coordinates because texture is flipped upside down)
    //    -0.25f,  1.0f,  0.0f,  0.0f,  0.0f,
    //    -0.25f, 0.0f,  0.0f,  0.0f,  0.5f,
    //    0.25f, 0.0f,  0.0f,  1.0f,  0.5f,

    //    -0.25f,  1.0f,  0.0f,  0.0f,  0.0f,
    //    0.25f, 0.0f,  0.0f,  1.0f,  0.5f,
    //    0.25f,  1.0f,  0.0f,  1.0f,  0.0f,

    //};

    //float hairVertices[60] = {
    //    // positions         // texture Coords (swapped y coordinates because texture is flipped upside down)
    //    -0.25f,  1.0f,  0.0f,  0.0f,  0.0f,
    //    -0.25f, 0.0f,  0.0f,  0.0f,  0.5f,
    //    0.25f, 0.0f,  0.0f,  1.0f,  0.5f,

    //    -0.25f,  1.0f,  0.0f,  0.0f,  0.0f,
    //    0.25f, 0.0f,  0.0f,  1.0f,  0.5f,
    //    0.25f,  1.0f,  0.0f,  1.0f,  0.0f,

    //    // positions         // texture Coords (swapped y coordinates because texture is flipped upside down)
    //    -0.25f,  0.0f,  0.0f,  0.0f,  0.5f,
    //    -0.25f, -1.0f,  0.0f,  0.0f,  1.0f,
    //    0.25f, -1.0f,  0.0f,  1.0f,  1.0f,

    //    -0.25f,  0.0f,  0.0f,  0.0f,  0.5f,
    //    0.25f, -1.0f,  0.0f,  1.0f,  1.0f,
    //    0.25f,  0.0f,  0.0f,  1.0f,  0.5f
    //};

    vector<float> v_hairVertices; //Try to do detection with this vertices, OR viewport check the depth buffer

    // World space positions of hair
	float cardWidth = 0;
	vec3 pos, rot;

    void Setup();
	void LoadTexture(const char* texSource, unsigned int& textureID);
    void CubicBezier(vec3 contolPos1, vec3 contolPos2, vec3 contolPos3, vec3 contolPos4, float t);
	void GenBezNode(vec3 nodePos);
    void LinkNodes(HairNode* node1, HairNode* node2);
    void CreateNormalizedLinks(vector<HairLink*>& hairLinks);
    void CreateHairMesh(HairNode* node1, HairNode* node2, float length, float width, float startU, float startV, float endU, float endV);
    void UpdateHairMesh(HairNode* node1, HairNode* node2, float width, int index);
    void UpdateBufferData();
};
