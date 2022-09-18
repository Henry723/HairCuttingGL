#pragma once
#include "HairNode.h"
#include "../Renderer/Shader.h"

#include<iostream>
class HairLink
{
public:
	bool showLine = true;

	HairLink(HairNode* node1, HairNode* node2, bool showLine);
	~HairLink();

private:
	HairNode* n1;
	HairNode* n2;

	// For plane mesh generation and making sure that physics constraints bound to this distance
	float length; 
	float width;

	// 0 - 1 on uv for mapping
	float u;
	float v;

	float GetDistance(HairNode* node1, HairNode* node2);
	void Solve();
	void CreateHairMesh(HairNode* node1, HairNode* node2);
	void UpdateHairMesh();
	void DrawHair();
};
