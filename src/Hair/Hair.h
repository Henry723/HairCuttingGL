#pragma once
#include "HairLink.h"
#include <vector>
using namespace std;

class Hair
{
public:
	Hair(float width, float length, int numLinks, const char* texSource);
	~Hair();

	//void LoadTexture(texSource);
private:
	vector<HairLink> links;
	float width;
	float length;
	int numLinks;

};

Hair::Hair(float w, float l, int numL, const char* texSource)
{
	width = w;
	length = l;
	numLinks = numL;

	//LoadTexture(texSource);
}

Hair::~Hair()
{
}