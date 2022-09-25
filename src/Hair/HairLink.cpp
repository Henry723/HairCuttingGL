#include "HairLink.h"



HairLink::HairLink(HairNode* node1, HairNode* node2, bool showLine)
{
	n1 = node1;
	n2 = node2;

	length = GetDistance(node1, node2);
	//std::cout << "Link length: " << length << std::endl;

	//std::cout << showLine << std::endl;
}

HairLink::~HairLink()
{
	std::cout << "Link Deleted" << std::endl;
	//delete this;
}

float HairLink::GetLength()
{
	return length;
}

float HairLink::GetStartU()
{
	return startU;
}

float HairLink::GetEndU()
{
	return endU;
}

float HairLink::GetStartV()
{
	return startV;
}

float HairLink::GetEndV()
{
	return endV;
}

HairNode* HairLink::GetNode1()
{
	return n1;
}

HairNode* HairLink::GetNode2()
{
	return n2;
}

void HairLink::SetStartUV(float u, float v)
{
	startU = u;
	startV = v;
}

void HairLink::SetEndUV(float u, float v)
{
	endU = u;
	endV = v;
}

void HairLink::SetID(int id)
{
	ID = id;
}

float HairLink::GetDistance(HairNode* node1, HairNode* node2)
{
	float diffX = node1->position.x - node2->position.x;
	float diffY = node1->position.y - node2->position.y;
	float diffZ = node1->position.z - node2->position.z;
	return sqrt(diffX * diffX + diffY * diffY + diffZ * diffZ);;
}

// Solves all the link constraints
void HairLink::Solve()
{
	// Calculate the distance between two hair nodes
	//float
}

