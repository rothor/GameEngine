#pragma once
#include "DrawInstructions.h"
#include "DrawInstructionsPoint.h"

class DrawInstructionsEllipse : public DrawInstructions
{
public:
	DrawInstructionsEllipse(DrawInstructionsPoint pCenter, float pWidth, float pHeight, float pAngle, int pColor, float pAlpha);

public:
	DrawInstructionsPoint center;
	float width;
	float height;
	float angle;
	int color;
	float alpha;
};