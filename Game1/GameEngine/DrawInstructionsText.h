#pragma once
#include "DrawInstructions.h"
#include "DrawInstructionsPoint.h"
#include <string>

class DrawInstructionsText : public DrawInstructions
{
public:
	DrawInstructionsText(DrawInstructionsPoint pCenter, std::wstring pText, std::wstring pFont, float pSize, float pAngle, int color, float pAlpha);

public:
	DrawInstructionsPoint center;
	float size;
	float angle;
	int color;
	float alpha;
	std::wstring text;
	std::wstring font;

};

