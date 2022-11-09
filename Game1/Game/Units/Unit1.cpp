#include "Unit1.h"
#include "Player.h"

Unit1::Unit1(b2World* world, float x, float y, int id, SoundEnum sound) :
	Unit(UnitType::Unit1)
{
	_sound = sound;
	float width = 1.0f;
	float height = 1.0f;
	float density = 1.0f;
	float friction = 0.3f;
	float angle = 0.0f;
	float bounciness = 0.3f;
	Unit::addBox(-0.25, -0.5, width, height, angle, density, friction, bounciness, 0x800000, 0.5, 1);

	float radius = 1.0f;
	Unit::addEllipse(0.25, 0, radius, angle, density, friction, bounciness, 0x40C040);
}

void Unit1::onContactBegin(Unit* unit, int fixtureId)
{
	if (unit->getUnitType() == UnitType::Player) {
		_soundIdQueue.push_back(_sound);
	}
}