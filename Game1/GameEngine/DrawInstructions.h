#pragma once
#include <vector>
#include "DrawInstructionsPoint.h"


class DrawInstructions
{
public:
	enum class DrawType { polygon, ellipse, image, text };

protected:
	DrawInstructions(DrawType pType);

public:
	DrawType type;
};