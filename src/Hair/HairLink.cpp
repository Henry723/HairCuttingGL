#include "HairLink.h"



HairLink::HairLink(HairNode node1, HairNode node2, bool showLine)
{
	n1 = node1;
	n2 = node2;

	restingDistance = 0;
	//GenPlane(node1, node2);
	std::cout << showLine << std::endl;
}

HairLink::~HairLink()
{
}
