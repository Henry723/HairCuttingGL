#include "Mesh.h"

Mesh::Mesh(vector<Vertex> vert, vector<unsigned int> ind)
{
	vertices = vert;
	indices = ind;

	// Initialization code (done once (unless your object frequently changes))
	glGenVertexArrays(1, &VAO);

	// 1. Create an ID for a new VBO & EBO to send to the Vertex Shader for rendering, stored in the GPU.
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	// ------------------ Vertex Array Object --------------------
	glBindVertexArray(VAO);

	// VBO's must be bound to a unique buffer object type, in this case GL_ARRAY_BUFFER.
	//		Any buffer calls made on GL_ARRAY_BUFFER will refer to and configure our VBO until it is re-bound.
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// Copy the previously defined vertex data into the bound buffer's memory.
	//		The graphics card will manage the data as follows:
	//			GL_STREAM_DRAW: the data is set only once, and used by the GPU at most a few times.
	//			GL_STATIC_DRAW: the data is set only once, and used many times.
	//			GL_DYNAMIC_DRAW : the data is changed a lot, and used many times.
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	// Next, we bind our index array in the same way as our VBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	// Here, I'm defining the attributes as variables to clearly label how these values are being used below.
	int positionAttributeID = 0;
	int posAttributeSize = 3; // 3 - x, y, z.

	int normalAttributeID = 1;
	int normAttributeSize = 3; // 3 - x, y, z.

	int textureAttributeID = 2;
	int texAttributeSize = 2; // 2 - Only need a 2D coord for UV's

	// Vertex Position Attribute
	glVertexAttribPointer(positionAttributeID, posAttributeSize, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0); // 0 is the offset.
	glEnableVertexAttribArray(positionAttributeID);

	// Vertex Normal Attribute
	glVertexAttribPointer(normalAttributeID, normAttributeSize, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal)); // Start at the END of position attribute
	glEnableVertexAttribArray(normalAttributeID);

	// Texture Coordinate Attribute
	glVertexAttribPointer(textureAttributeID, texAttributeSize, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoord));
	glEnableVertexAttribArray(textureAttributeID);
}

Mesh::~Mesh()
{
}

void Mesh::Draw(Shader& shader, unsigned int textureID)
{
	glBindTexture(GL_TEXTURE_2D, textureID);
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	// Set back to default when finished
	glActiveTexture(GL_TEXTURE0);
}

