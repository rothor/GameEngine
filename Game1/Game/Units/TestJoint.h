#pragma once
#include "Unit.h"

class TestJoint : public Unit
{
public:
	TestJoint(b2World* world, float x, float y, float motorSpeedInRadians, int id);
	virtual void onContactBegin(Unit* otherUnit, int otherBodyId, int otherFixtureId, int thisBodyId, int thisFixtureId, b2Vec2 contactNormal) override;

protected:
	b2RevoluteJoint* _joint;
	int _fanBodyId;
};