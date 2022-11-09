#include "TestJoint.h"
#include "Lib/RandomColorGetter.h"

TestJoint::TestJoint(b2World* world, float x, float y, float motorSpeedRps, int id) :
	Unit(UnitType::TestJoint, world, id)
{
	// Create fan
	int _fanBodyId = Unit::createBody(b2BodyType::b2_dynamicBody, x, y, id);
	Body* fanBody = Unit::getBody(_fanBodyId);
	float density = 1.0f;
	float friction = 0.3f;
	float bounciness = 0.3f;
	float w = 1.0f;
	float h = 5.0f;
	fanBody->addBox(x, y - 0.5f + h / 2, 1, 5, 0, density, friction, bounciness, 0x808080);
	fanBody->addBox(x, y + 0.5f - h / 2, 1, 5, 0, density, friction, bounciness, 0x808080);
	fanBody->addBox(x - 0.5f + h / 2, y, 1, 5, b2_pi / 2, density, friction, bounciness, 0x808080);
	fanBody->addBox(x + 0.5f - h / 2, y, 1, 5, b2_pi / 2, density, friction, bounciness, 0x808080);

	// Create notch
	int notchBodyId = Unit::createBody(b2BodyType::b2_staticBody, x, y, id);
	Body* notchBody = Unit::getBody(notchBodyId);
	notchBody->setFixedRotation(true);
	notchBody->addBox(x, y, 0.2, 0.2, 0, 1.0, 0.0, 0.0, 0x808080);

	// Create joint
	b2RevoluteJointDef jointDef;
	jointDef.Initialize(fanBody->getBodyPtr(), notchBody->getBodyPtr(), notchBody->getPos());
	jointDef.motorSpeed = motorSpeedRps;
	jointDef.maxMotorTorque = 50000.0f;
	if (motorSpeedRps != 0) {
		jointDef.enableMotor = true;
	}
	_joint = (b2RevoluteJoint*)world->CreateJoint(&jointDef);
}

void TestJoint::onContactBegin(Unit* otherUnit, int otherBodyId, int otherFixtureId, int thisBodyId, int thisFixtureId, b2Vec2 contactNormal)
{
	if (otherUnit->getUnitType() == UnitType::Wall || true) {
		Unit::getBody(_fanBodyId)->getFixture(otherFixtureId)->setColor(RandomColorGetter::GetRandomColor());
	}
}
