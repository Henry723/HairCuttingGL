#include "HairNode.h"

HairNode::HairNode(vec3 pos)
{
	position = pos;
	prevPosition = position;
	/*accX = 0;
	accY = 0;
	accZ = 0;*/
	acceleration = vec3(0,0,0);

	//cout << "Starting acc:" << acceleration.x << acceleration.y << acceleration.z <<endl;
	mass = MASS;
	damping = DAMPING;
	//UpdatePhysics(1);

	printf("Node: (%f, %f, %f) \n", pos.x, pos.y, pos.z);
}

HairNode::~HairNode()
{
}

//This fuction is for updating the physics of the HairNode.
//Apply motion using verlet integration instead as euler integration for better accuracy.
//TimeStep should be in seconds.
void HairNode::UpdatePhysics(float timeStep)
{
	//printf("Acc: (%f, %f, %f) \n", acceleration.x, acceleration.y, acceleration.z);
	ApplyForce(vec3(0, mass * 9.8f, 2));
	//printf("Next Acc: (%f, %f, %f) \n", acceleration.x, acceleration.y, acceleration.z);

	//position = vec3(2,4,6);
	//prevPosition = vec3(1, 1, 3);

	vec3 velocity = position - prevPosition;
	//printf("Velocity: (%f, %f, %f) \n", velocity.x, velocity.y, velocity.z);

	//Dampen
}


void HairNode::ApplyForce(vec3 force)
{
	// F = ma
	// acceleration = force / mass
	// acceleration = (1/mass) * force
	/*accX += force.x / mass;
	accY += force.y / mass;
	accZ += force.z / mass;*/
	acceleration += force / mass;
}

void HairNode::PinHair()
{
	isRoot = true;
}

void HairNode::DrawHair()
{
}


