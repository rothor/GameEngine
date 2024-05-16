#include "Unit.h"

Unit::Unit(UnitType unitType, b2World* world, int unitId) :
	_unitType(unitType),
	_debugDraw(false),
	_unitId(unitId),
	FlagDestroy(false),
	FlagIsStatic(false),
	FlagPlayerControlled(false),
	_flagShouldDisable(false),
	PhysicsObject(world)
{
	
}

void Unit::addEffectToQueue(Effect* effect)
{
	_effectList.push_back(effect);
}

void Unit::onContactBeginBasicBehavior(Unit* otherUnit, int otherBodyId, int otherFixtureId, int thisBodyId, int thisFixtureId, b2Vec2 contactNormal)
{
	CollisionManager::addCollision(otherUnit->getId(), otherBodyId, otherFixtureId, contactNormal);
	Unit::onContactBegin(otherUnit, otherBodyId, otherFixtureId, thisBodyId, thisFixtureId, contactNormal);
}

void Unit::onContactBegin(Unit* otherUnit, int otherBodyId, int otherFixtureId, int thisBodyId, int thisFixtureId, b2Vec2 contactNormal)
{
	// Do nothing by default. Child classes can override this.
}

void Unit::onContactEndBasicBehavior(Unit* otherUnit, int otherBodyId, int otherFixtureId, int thisBodyId, int thisFixtureId)
{
	CollisionManager::removeCollision(otherUnit->getId(), otherBodyId, otherFixtureId);
	Unit::onContactEnd(otherUnit, otherBodyId, otherFixtureId, thisBodyId, thisFixtureId);
}

void Unit::onContactEnd(Unit* otherUnit, int otherBodyId, int otherFixtureId, int thisBodyId, int thisFixtureId)
{
	// Do nothing by default. Child classes can override this.
}

std::list<DrawInstructions*>* Unit::getDrawInstructions()
{
	return PhysicsObject::getDrawInstructions();
}

UnitType Unit::getUnitType()
{
	return _unitType;
}

bool Unit::hasSoundInQueue()
{
	return _soundIdQueue.size() > 0;
}

SoundEnum Unit::getSoundInQueue()
{
	SoundEnum soundId = _soundIdQueue.front();
	_soundIdQueue.pop_front();
	return soundId;
}

void Unit::addSoundToQueue(SoundEnum sound)
{
	_soundIdQueue.push_back(sound);
}

void Unit::setDebugDraw(bool turnOn)
{
	_debugDraw = turnOn;
	for (int i = 0; i < PhysicsObject::getNumBodies(); i++) {
		PhysicsObject::getBody(i)->setDebugDraw(turnOn);
	}
}

void Unit::timeElapsed(long long microseconds)
{
	TimableObject::timeElapsed(microseconds);

	for (int i = 0; i < PhysicsObject::getNumBodies(); i++) {
		Body* body = PhysicsObject::getBody(i);
		// Destroy fixtures
		body->destroyFixturesSetToDestroy();
		// Apply overridden gravity
		if (body->getIsGravityOverridden()) {
			body->applyOverriddenGravityToSelf();
		}
		// Advance animations
		body->timeStepForGraphics(microseconds);
	}

	timeElapsedExtraBehavior(microseconds);
}

void Unit::timeElapsedExtraBehavior(long long microseconds)
{
	// Do nothing by default. Child classes can override this.
}

bool Unit::hasEffectInQueue()
{
	return _effectList.size() > 0;
}

Effect* Unit::getEffectInQueue()
{
	Effect* effect = _effectList.front();
	_effectList.pop_front();
	return effect;
}

int Unit::getId()
{
	return _unitId;
}

void Unit::setToDisable()
{
	_flagShouldDisable = true;
}

bool Unit::shouldDisable()
{
	return _flagShouldDisable;
}