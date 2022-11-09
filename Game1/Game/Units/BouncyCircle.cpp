#include "BouncyCircle.h"

BouncyCircle::BouncyCircle(b2World* world, float x, float y, float radius, int color, int id, SoundEnum sound) :
	Unit(UnitType::BouncyCircle, world, id),
	_sound(sound)
{
	int bodyId = Unit::createBody(b2BodyType::b2_dynamicBody, x, y, id);
	Unit::getBody(bodyId)->addEllipse(0, 0, radius, 0, 1.0f, 0.0f, 1.0f, color, 1.0f, 0.0f, 1.0f, {}, false);
}

void BouncyCircle::onContactBegin(Unit* otherUnit, int otherBodyId, int otherFixtureId, int thisBodyId, int thisFixtureId, b2Vec2 contactNormal)
{
	if (otherUnit->getUnitType() == UnitType::Wall) {
		addSoundToQueue(_sound);
	}
}