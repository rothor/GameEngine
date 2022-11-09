#pragma once
#include "DrawInstructions.h"
#include <vector>
#include <memory>

class DrawInstructionsPolygon : public DrawInstructions
{
public:
	DrawInstructionsPolygon(std::vector<DrawInstructionsPoint>* pVertexes, int pColor, float pAlpha);

public:
	std::unique_ptr<std::vector<DrawInstructionsPoint>> vertexes;
	int color;
	float alpha;
};