#pragma once
#include <glad/glad.h> // Always include glad first to get the OpenGL headers required by glfw

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Shader.h"

#include <string>
#include <vector>
using namespace std;

struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoord;
};

class Mesh
{
public:
	//Al the mesh data will store here
	vector<Vertex> vertices;
	vector<unsigned int> indices;
	unsigned int VAO;

	Mesh(vector<Vertex> vert, vector<unsigned int> ind);
	~Mesh();

	void Draw(Shader& shader, unsigned int textureID);

private:
	unsigned int VBO, EBO;
};
