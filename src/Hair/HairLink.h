#pragma once
#include "HairNode.h"
#include "../Renderer/Shader.h"

#include<iostream>
class HairLink
{
public:
	bool showLine = true;
	int ID;

	HairLink(HairNode* node1, HairNode* node2, bool showLine);
	~HairLink();

	float GetLength();
	float GetStartU();
	float GetEndU();
	float GetStartV();
	float GetEndV();

	HairNode* GetNode1();
	HairNode* GetNode2();

	void SetStartUV(float u, float v);
	void SetEndUV(float u, float v);
	void SetID(int id);

private:
	HairNode* n1;
	HairNode* n2;

	// For plane mesh generation and making sure that physics constraints bound to this distance
	float length; 

	// 0 - 1 on uv for mapping
	float startU;
	float endU;
	float startV;
	float endV;

	float GetDistance(HairNode* node1, HairNode* node2);
	void Solve();
	
	void UpdateHairMesh();
	void DrawHair();
};
