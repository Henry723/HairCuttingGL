#pragma once
// Assimp Includes
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

// Stbi includes
#include "stb_image.h"

// GL Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Mesh.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>

#include "../Hair/Hair.h"
using namespace std;

class Head
{
public:
	// Head model data
	vector<Mesh> meshes;
	unsigned int textureID;
	Head(const char* modelSource, const char* texSource);
	~Head();
	void Draw(Shader& shader, unsigned int textureID);

private:
	void loadHeadModel(const char* modelSource);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	void loadTexture(const char* texSource, unsigned int& textureID);
};

