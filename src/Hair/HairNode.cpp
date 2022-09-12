#include "HairNode.h"
#include<iostream>
HairNode::HairNode(glm::vec3 pos)
{
	position = pos;
	prevPosition = position;
	/*accX = 0;
	accY = 0;
	accZ = 0;*/
	acceleration = glm::vec3(0,0,0);

	std::cout << "Starting acc:" << acceleration.x << acceleration.y << acceleration.z <<std::endl;
	mass = MASS;
	damping = DAMPING;
}

HairNode::~HairNode()
{
}

//This fuction is for updating the physics of the HairNode.
//Apply motion using verlet integration instead as euler integration for better accuracy.
//TimeStep should be in seconds.
void HairNode::UpdatePhysics(float timeStep)
{
	ApplyForce(glm::vec3(0, mass * 9.8f, 0));
}


void HairNode::ApplyForce(glm::vec3 force)
{
	// F = ma
	// acceleration = force / mass
	// acceleration = (1/mass) * force
	/*accX += force.x / mass;
	accY += force.y / mass;
	accZ += force.z / mass;*/
	acceleration += force / mass;
}

void HairNode::PinHairTo(glm::vec3 rootPos)
{
	isRoot = true;
	rootPosition = rootPos;
}

void HairNode::DrawHair()
{
}


