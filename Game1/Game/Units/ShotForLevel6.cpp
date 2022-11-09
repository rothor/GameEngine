#include "ShotForLevel6.h"

ShotForLevel6::ShotForLevel6(b2World* world, float x, float y, int id) :
	Unit(UnitType::ShotForLevel6)
{
	Unit::addBox(0, 0, 0.5f, 0.5f, 0, 2.0f, 0.3f, 0.3f, 0xFF0000, 1.0f, 0, 0.002f);
	_fixtures[0].setAnimations(
		{
			{
				GraphicsEnum::PoorlyDrawnSmile
			}
		}
	);
}

void ShotForLevel6::onContactBegin(Unit* unit, int fixtureId)
{
	if (unit->getUnitType() == UnitType::Wall) {
		FlagDestroy = true;
	}
}
