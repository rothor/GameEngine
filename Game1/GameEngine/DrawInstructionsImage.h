#pragma once
#include "DrawInstructions.h"
#include "DrawInstructionsPoint.h"

class DrawInstructionsImage : public DrawInstructions
{
public:
	DrawInstructionsImage(DrawInstructionsPoint pCenter, float pScale, float pAngle, int pResourceId, float pAlpha, bool pFlipHorizontally);

public:
	DrawInstructionsPoint center;
	float scale;
	float angle;
	int resourceId;
	float alpha;
	bool flipHorizontally;
};