#pragma once
#include "HairNode.h"
#include<iostream>
class HairLink
{
public:
	float restingDistance;
	bool showLine = true;

	HairLink(HairNode node1, HairNode node2, bool showLine);
	~HairLink();

private:
	HairNode n1;
	HairNode n2;
	
};
