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

int HairLink::GetID()
{
	return ID;
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

void HairLink::SetBoxMinMax(float halfWidth)
{
	float minX, minY, minZ, maxX, maxY, maxZ;

	float minX1 = n1->position.x - halfWidth;
	float minX2 = n2->position.x - halfWidth;

	float maxX1 = n1->position.x + halfWidth;
	float maxX2 = n2->position.x + halfWidth;

	float Y1 = n1->position.y;
	float Y2 = n2->position.y;

	float Z1 = n1->position.z;
	float Z2 = n2->position.z;

	if (minX1 <= minX2)
		minX = minX1;
	else
		minX = minX2;

	if (maxX1 >= maxX2)
		maxX = maxX1;
	else
		maxX = maxX2;

	if (Y1 <= Y2) {
		minY = Y1;
		maxY = Y2;
	}	
	else
	{
		minY = Y2;
		maxY = Y1;
	}
	
	if (Z1 <= Z2) {
		minZ = Z1;
		maxZ = Z2;
	}
	else
	{
		minZ = Z2;
		maxZ = Z1;
	}

	boxMin = vec3(minX, minY, minZ);
	boxMax = vec3(maxX, maxY, maxZ);
}

//https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-box-intersection
//https://www.youtube.com/watch?v=USjbg5QXk3g
bool HairLink::AABB_Test(vec3 rayOrigin, vec3 rayDir)
{
	float temp;

	// X test
	float txMin = (boxMin.x - rayOrigin.x) / rayDir.x;
	float txMax = (boxMax.x - rayOrigin.x) / rayDir.x;

	if (txMax < txMin) { temp = txMax; txMax = txMin; txMin = temp; };

	// Y test
	float tyMin = (boxMin.y - rayOrigin.y) / rayDir.y;
	float tyMax = (boxMax.y - rayOrigin.y) / rayDir.y;

	if (tyMax < tyMin) { temp = tyMax; tyMax = tyMin; tyMin = temp; };

	// Z test
	float tzMin = (boxMin.z - rayOrigin.z) / rayDir.z;
	float tzMax = (boxMax.z - rayOrigin.z) / rayDir.z;

	if (tzMax < tzMin) { temp = tzMax; tzMax = tzMin; tzMin = temp; };

	// Get greatest min and smallest max
	float tMin = (txMin > tyMin) ? txMin : tyMin;
	float tMax = (txMax < tyMax) ? txMax : tyMax;

	if (txMin > tyMax || tyMin > txMax) 
		return false;

	if (tMin > tzMax || tzMin > tMax) 
		return false;

	if (tzMin > tMin) 
		tMin = tzMin;

	if (tzMax < tMax) 
		tMax = tzMax;

	return true;
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

