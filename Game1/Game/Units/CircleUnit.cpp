#include "CircleUnit.h"
#include "../GraphicsMap.h"

CircleUnit::CircleUnit(b2World* world, float x, float y, int color, int id) :
	Unit(UnitType::CircleUnit)
{
	float density = 1.0f;
	float friction = 0.3f;
	float angle = 0.0f;
	float bounciness = 0.3f;
	float radius = 1.0f;
	Unit::addEllipse(-1, 0, radius, angle, density, friction, bounciness, color, 1.0, GraphicsMap::idMap[GraphicsEnum::PoorlyDrawnSmile]);
	Unit::addEllipse(1, 0, radius, angle, density, friction, bounciness, color);
}