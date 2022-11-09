#pragma once
#include "box2d/box2d.h"
#include "DrawInstructions.h"
#include "Fixture.h"
#include "Body.h"
#include "UnitEnum.h"
#include "SoundMap.h"
#include "DrawableObject.h"
#include "TimableObject.h"
#include "DumpableObject.h"
#include "UnitBodyIdHolder.h"
#include "Effects/Effect.h"
#include "CollisionManager.h"
#include <vector>
#include <string>
#include <list>
#include <memory>

class Unit : public DrawableObject, public TimableObject, public CollisionManager
{
public:
	Unit(UnitType unitType, b2World* world, int unitId);
	virtual std::list<DrawInstructions*>* getDrawInstructions();
	virtual void onContactBegin(Unit* otherUnit, int otherBodyId, int otherFixtureId, int thisBodyId, int thisFixtureId, b2Vec2 contactNormal);
	virtual void onContactEnd(Unit* otherUnit, int otherBodyId, int otherFixtureId, int thisBodyId, int thisFixtureId);
	virtual void timeElapsed(long long microseconds) override;
	Body* getBody(int bodyId);
	void destroy();
	UnitType getUnitType();
	bool hasSoundInQueue();
	SoundEnum getSoundInQueue();
	void addSoundToQueue(SoundEnum sound);
	virtual void timeStepForGraphics(int microseconds);
	void setDebugDraw(bool turnOn);
	void setZIndex(int zIndex);
	void regulate();
	void push(b2Vec2 direction, float force);
	void setToDisable();
	bool shouldDisable();
	void disable();
	bool hasEffectInQueue();
	Effect* getEffectInQueue();
	int getId();
	void setGravity(b2Vec2 gravity);

public:
	bool FlagPlayerControlled = false; // Set to true if the player should control this unit with the keyboard.
	bool FlagDestroy = false; // Set this to true when the unit should be destroyed.
	bool FlagIsStatic = false;

protected:
	int createBody(b2BodyType type, float x, float y, int id);
	int getNewBodyId();
	UnitBodyIdHolder* getNewUnitBodyIdHolder(int unitId, int bodyId);
	void addEffectToQueue(Effect* effect);

protected:
	b2World* _world;
	std::map<int, Body> _bodies;
	int _bodyIdIncrementer = 0;
	UnitType _unitType;
	std::list<SoundEnum> _soundIdQueue;
	bool _debugDraw;
	std::list<std::unique_ptr<UnitBodyIdHolder>> _idHolderList;
	bool _flagShouldDisable;
	std::list<Effect*> _effectList;
	int _unitId;
};