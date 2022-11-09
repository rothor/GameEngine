#include "Square.h"
#include "../RandomColorGetter.h"

Square::Square(b2World* world, float x, float y, float w, float h, int color, int id) :
	Unit(UnitType::Square)
{
	float density = 1.0f;
	float friction = 0.3f;
	float bounciness = 0.3f;
	Unit::addBox(0, 0, w, h, 0, density, friction, bounciness, color, 1.0f, 0);
}

void Square::onContact(Unit* unit)
{
	if (unit->getUnitType() == UnitType::FlipperUnit) {
		_fixtures[0].setColor(RandomColorGetter::GetRandomColor());
	}
}