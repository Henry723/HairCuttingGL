#pragma once
// GL Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Default node values
const float MASS = 1.0f;
const float DAMPING = 20.0f;

//This class acts as a particle within strands of hair
class HairNode
{
public:
	glm::vec3 position;
	glm::vec3 prevPosition; //Calculating positional change (velocity)
	glm::vec3 acceleration;

	float mass;
	float damping; //coefficent k

	bool isRoot = false; //The root hair will not going to move/affected by gravity + other forces
	glm::vec3 rootPosition;

	HairNode() = default;
	HairNode(glm::vec3 pos);
	~HairNode();


	void UpdatePhysics(float timeStep);
	void ApplyForce(glm::vec3 force);

	void PinHairTo(glm::vec3 rootPos);

	void DrawHair();
	
private:
	
};
