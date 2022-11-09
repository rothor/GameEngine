#include "Unit.h"

Unit::Unit(UnitType unitType, b2World* world, int unitId) :
	_unitType(unitType),
	_world(world),
	_debugDraw(false),
	_unitId(unitId)
{
	
}

int Unit::createBody(b2BodyType type, float x, float y, int id)
{
	b2BodyDef bodyDef;
	bodyDef.type = type;
	bodyDef.position.Set(x, y);
	b2Body* body = _world->CreateBody(&bodyDef);
	int bodyId = getNewBodyId();
	_bodies[bodyId] = Body(body);
	UnitBodyIdHolder* a = getNewUnitBodyIdHolder(id, bodyId);
	body->SetUserData(a);
	return bodyId;
}

Body* Unit::getBody(int bodyId)
{
	return &(_bodies[bodyId]);
}

int Unit::getNewBodyId()
{
	int id = _bodyIdIncrementer;
	_bodyIdIncrementer++;
	return id;
}

UnitBodyIdHolder* Unit::getNewUnitBodyIdHolder(int unitId, int bodyId)
{
	UnitBodyIdHolder* a = new UnitBodyIdHolder();
	a->bodyId = bodyId;
	a->unitId = unitId;
	_idHolderList.push_back(std::unique_ptr<UnitBodyIdHolder>(a));
	return a;
}

void Unit::addEffectToQueue(Effect* effect)
{
	_effectList.push_back(effect);
}

std::list<DrawInstructions*>* Unit::getDrawInstructions()
{
	std::list<DrawInstructions*>* diList = new std::list<DrawInstructions*>{};
	for (auto& pair : _bodies) {
		auto bodyDi = pair.second.getDrawInstructions();
		diList->insert(diList->end(), bodyDi->begin(), bodyDi->end());
		delete bodyDi;
	}
	return diList;
}

void Unit::destroy()
{
	for (auto& pair : _bodies) {
		pair.second.destroy();
	}
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

void Unit::timeStepForGraphics(int microseconds)
{
	for (auto& pair : _bodies) {
		pair.second.timeStepForGraphics(microseconds);
	}
}

void Unit::setDebugDraw(bool turnOn)
{
	_debugDraw = turnOn;
	for (auto& pair : _bodies) {
		pair.second.setDebugDraw(turnOn);
	}
}

void Unit::setZIndex(int zIndex)
{
	_zIndex = zIndex;
}

void Unit::onContactBegin(Unit* otherUnit, int otherBodyId, int otherFixtureId, int thisBodyId, int thisFixtureId, b2Vec2 contactNormal)
{
	// do nothing be default
}

void Unit::onContactEnd(Unit* otherUnit, int otherBodyId, int otherFixtureId, int thisBodyId, int thisFixtureId)
{
	// do nothing be default
}

void Unit::timeElapsed(long long microseconds)
{
	TimableObject::timeElapsed(microseconds);

	for (auto& pair : _bodies) {
		pair.second.destroyFixturesSetToDestroy();
		if (pair.second.getIsGravityOverridden()) {
			pair.second.applyOverriddenGravityToSelf();
		}
	}
}

void Unit::regulate()
{
	for (auto& pair : _bodies) {
		pair.second.regulate();
	}
}

void Unit::push(b2Vec2 direction, float force)
{
	getBody(0)->push(direction.x, direction.y, force);
}

void Unit::setToDisable()
{
	_flagShouldDisable = true;
}

bool Unit::shouldDisable()
{
	return _flagShouldDisable;
}

void Unit::disable()
{
	for (auto& pair : _bodies) {
		pair.second.disable();
	}
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

void Unit::setGravity(b2Vec2 gravity)
{
	for (auto& pair : _bodies) {
		pair.second.overrideGravity(gravity);
	}
}
