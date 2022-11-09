#pragma once
#include "box2d/box2d.h"
#include <list>

class CollisionManagerCollision
{
public:
	CollisionManagerCollision(int unitId, int bodyId, int fixtureId, b2Vec2 normalForce);
	bool is(int unitId, int bodyId, int fixtureId);
	bool isFloor();
	bool isLeftWall();
	bool isRightWall();
	bool isCeiling();

private:
	int _unitId{ -1 };
	int _bodyId{ -1 };
	int _fixtureId{ -1 };
	b2Vec2 _normalForce;
	bool _isFloor{ false };
	bool _isLeftWall{ false };
	bool _isRightWall{ false };
	bool _isCeiling{ false };
};

class CollisionManager
{
public:
	CollisionManager();
	void addCollision(int unitId, int bodyId, int fixtureId, b2Vec2 normalForce);
	void removeCollision(int unitId, int bodyId, int fixtureId);
	bool isOnFloor();
	bool isOnLeftWall();
	bool isOnRightWall();
	bool isOnCeiling();

private:
	std::list<CollisionManagerCollision> _collisionList;
};

