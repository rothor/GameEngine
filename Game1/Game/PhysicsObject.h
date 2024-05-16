#pragma once
#include "box2d/box2d.h"
#include "Units/Fixture.h"
#include "Units/Body.h"
#include "DrawableObject.h"

class PhysicsObject : public DrawableObject
{
public:
	PhysicsObject(b2World* world);
	virtual std::list<DrawInstructions*>* getDrawInstructions() override;
	Body* getBody(int bodyId);
	int getNumBodies();
	void destroy();
	void regulate();
	void push(b2Vec2 direction, float force);
	void disable();
	void setGravity(b2Vec2 gravity);
	int createBody(b2BodyType type, float x, float y, int id);

private:
	int getNewBodyId();
	UnitBodyIdHolder* getNewUnitBodyIdHolder(int unitId, int bodyId);
	
private:
	b2World* _world;
	std::map<int, Body> _bodies;
	int _bodyIdIncrementer = 0;
	std::list<std::unique_ptr<UnitBodyIdHolder>> _idHolderList;
};