#pragma once
// GL Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using glm::vec3;

// Default node values
const float MASS = 3.0f;
const float DAMPING = 0.99f;

//This class acts as a particle within strands of hair
class HairNode
{
public:
	vec3 position;
	vec3 prevPosition; //Calculating positional change (velocity)
	vec3 acceleration;

	float mass;
	float damping; //coefficent k

	bool isPinned = false; //The root hair will not going to move/affected by gravity + other forces

	HairNode() = default;
	HairNode(vec3 pos);
	~HairNode();


	void UpdatePhysics(float timeStep);
	void ApplyForce(vec3 force);

	void PinHair();

	void DrawHair();
	
private:
	
};
