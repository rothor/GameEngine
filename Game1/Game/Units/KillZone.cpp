#include "KillZone.h"


KillZone::KillZone(b2World* world, float x, float y, float w, float h, float angle, int color, int id) :
	Unit(UnitType::KillZone, world, id)
{
	int bodyId = Unit::createBody(b2BodyType::b2_staticBody, x, y, id);
	float density = 1.0f;
	float friction = 1.0f;
	float bounciness = 1.0f;
	Unit::getBody(bodyId)->addBox(0, 0, w, h, angle, density, friction, bounciness, color, 0.0f, 0.0f, 1.0f, { {GraphicsEnum::None} }, true);
}