#include "PhysicsObject.h"

PhysicsObject::PhysicsObject(b2World* world)
{
	_world = world;
}

std::list<DrawInstructions*>* PhysicsObject::getDrawInstructions()
{
	std::list<DrawInstructions*>* diList = new std::list<DrawInstructions*>{};
	for (auto& pair : _bodies) {
		auto bodyDi = pair.second.getDrawInstructions();
		diList->insert(diList->end(), bodyDi->begin(), bodyDi->end());
		delete bodyDi;
	}
	return diList;
}

int PhysicsObject::createBody(b2BodyType type, float x, float y, int id)
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

Body* PhysicsObject::getBody(int bodyId)
{
	return &(_bodies[bodyId]);
}

int PhysicsObject::getNumBodies()
{
	return _bodies.size();
}

int PhysicsObject::getNewBodyId()
{
	int id = _bodyIdIncrementer;
	_bodyIdIncrementer++;
	return id;
}

UnitBodyIdHolder* PhysicsObject::getNewUnitBodyIdHolder(int unitId, int bodyId)
{
	UnitBodyIdHolder* a = new UnitBodyIdHolder();
	a->bodyId = bodyId;
	a->unitId = unitId;
	_idHolderList.push_back(std::unique_ptr<UnitBodyIdHolder>(a));
	return a;
}

void PhysicsObject::destroy()
{
	for (auto& pair : _bodies) {
		pair.second.destroy();
	}
}

void PhysicsObject::regulate()
{
	for (auto& pair : _bodies) {
		pair.second.regulate();
	}
}

void PhysicsObject::push(b2Vec2 direction, float force)
{
	getBody(0)->push(direction.x, direction.y, force);
}

void PhysicsObject::disable()
{
	for (auto& pair : _bodies) {
		pair.second.disable();
	}
}

void PhysicsObject::setGravity(b2Vec2 gravity)
{
	for (auto& pair : _bodies) {
		pair.second.overrideGravity(gravity);
	}
}
