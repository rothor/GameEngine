#pragma once
#include "Unit.h"

class CircleUnit : public Unit
{
public:
	CircleUnit(b2World* world, float x, float y, int color, int id);
};