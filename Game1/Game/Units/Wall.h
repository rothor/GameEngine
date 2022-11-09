#pragma once
#include "Unit.h"

class Wall : public Unit
{
public:
	Wall(b2World* world, float x, float y, float w, float h, float angle, int color, int id, bool bouncy = false, bool visible = true);

protected:

};