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

	//printf("Node: (%f, %f, %f) \n", pos.x, pos.y, pos.z);
}

HairNode::~HairNode()
{
}

//This fuction is for updating the physics of the HairNode.
//Apply motion using verlet integration instead as euler integration for better accuracy.
//TimeStep should be in seconds.
void HairNode::UpdatePhysics(float timeStep)
{
	if (!isPinned) {
		//printf("Acc: (%f, %f, %f) \n", acceleration.x, acceleration.y, acceleration.z);
		ApplyForce(vec3(0, mass * -9.8f, 0));
		//printf("Next Acc: (%f, %f, %f) \n", acceleration.x, acceleration.y, acceleration.z);

		//position = vec3(2,4,6);
		//prevPosition = vec3(1, 1, 3);

		vec3 velocity = position - prevPosition;

		//Dampen velocity
		//printf("Velocity: (%f, %f, %f) \n", velocity.x, velocity.y, velocity.z);
		velocity *= damping;
		//printf("Velocity after: (%f, %f, %f) \n", velocity.x, velocity.y, velocity.z);

		float timeStepSq = timeStep * timeStep;
		// Calculate next position using Verlet Integration
		vec3 nextposition = position + velocity + 0.5f * acceleration * timeStepSq;
		//printf("Nextposition: (%f, %f, %f) \n", Nextposition.x, Nextposition.y, Nextposition.z);
		//printf("\n");

		// Reset variables
		prevPosition = position;
		position = nextposition;

		acceleration = vec3(0, 0, 0);
	}
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
	isPinned = true;
}

void HairNode::UnpinHair()
{
	isPinned = false;
}

void HairNode::DrawHair()
{
}


