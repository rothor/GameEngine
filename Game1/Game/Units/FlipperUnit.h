#pragma once
#include "Unit.h"

class FlipperUnit : public Unit
{
public:
	FlipperUnit(b2World* world, float x, float y, bool right, int id);
	void turnOn();
	void turnOff();

public:
	b2Body* _flipperB2Body;
	b2Body* _flipperStaticB2Body;
	b2RevoluteJoint* _flipperJoint;
	bool _isRight;
};

