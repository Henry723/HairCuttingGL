#include "HairLink.h"

HairLink::HairLink(HairNode node1, HairNode node2)
{
	n1 = node1;
	n2 = node2;
	//GenPlane(node1, node2);
}

HairLink::~HairLink()
{
}
