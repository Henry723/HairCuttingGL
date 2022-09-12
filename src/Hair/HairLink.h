#pragma once
#include "HairNode.h"
class HairLink
{
public:
	//bool showLine;

	HairLink(HairNode node1, HairNode node2);
	~HairLink();

private:
	HairNode n1;
	HairNode n2;

};
