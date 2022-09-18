#include "HairLink.h"



HairLink::HairLink(HairNode* node1, HairNode* node2, bool showLine)
{
	n1 = node1;
	n2 = node2;

	length = GetDistance(node1, node2);
	std::cout << "Link length: " << length << std::endl;

	CreateHairMesh(node1, node2);
	//std::cout << showLine << std::endl;
}

HairLink::~HairLink()
{
	delete this;
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

void HairLink::CreateHairMesh(HairNode* node1, HairNode* node2)
{
	printf("Creating Hair Mesh \n");
}
