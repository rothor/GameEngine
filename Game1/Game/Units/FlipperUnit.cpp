#include "FlipperUnit.h"

FlipperUnit::FlipperUnit(b2World* world, float x, float y, bool right, int id) :
	Unit(UnitType::FlipperUnit),
	_isRight(right)
{
	/*b2BodyDef bodyDef;
	bodyDef.type = b2BodyType::b2_dynamicBody;
	bodyDef.position.Set(x, y);
	_flipperB2Body = world->CreateBody(&bodyDef);
	_flipperB2Body->SetUserData((void*)id);

	b2PolygonShape shape;
	float w = 1;
	float h = 5;
	float angle = right ? -b2_pi * 2 * 0.65 : b2_pi * 2 * 0.65;
	auto points = Unit::getPointsFromRectRotateLast(0, 0, w, h, 0, h / 2 - 0.5, angle);
	shape.Set(&(*points)[0], 4);
	delete points;
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &shape;
	float density = 1.0f;
	float friction = 0.3f;
	float bounciness = 0.3f;
	fixtureDef.density = density;
	fixtureDef.friction = friction;
	fixtureDef.restitution = bounciness;
	b2Fixture* fixture = _flipperB2Body->CreateFixture(&fixtureDef);
	int fixtureId = getNewFixtureId();
	_fixtures[fixtureId] = 
		Fixture(
			fixture, 0x00FF00, x, y, w, h, 1, angle, 0, 1
		);

	// Create static box
	bodyDef.type = b2BodyType::b2_staticBody;
	bodyDef.position.Set(x, y);
	bodyDef.fixedRotation = true;
	_flipperStaticB2Body = world->CreateBody(&bodyDef);
	std::vector<b2Vec2> points2 {
		b2Vec2(-0.1, -0.1),
		b2Vec2(-0.1, 0.1),
		b2Vec2(0.1, 0.1),
		b2Vec2(0.1, -0.1)
	};
	shape.Set(&points2[0], 4);
	fixtureDef.shape = &shape;
	fixtureDef.density = 1.0;
	fixtureDef.friction = 0.0;
	fixtureDef.restitution = 0.0;
	fixture = _flipperStaticB2Body->CreateFixture(&fixtureDef);
	fixtureId = getNewFixtureId();
	_fixtures[fixtureId] = Fixture(fixture, 0x808080, 0, 0, 0.2f, 0.2f, 1, 0, 0, 1);

	b2RevoluteJointDef jointDef;
	jointDef.Initialize(_flipperB2Body, _flipperStaticB2Body, _flipperStaticB2Body->GetWorldPoint(b2Vec2(0, 0)));
	jointDef.motorSpeed = 0;
	jointDef.maxMotorTorque = 10000.0f;
	jointDef.enableMotor = true;
	jointDef.lowerAngle = _isRight ? 0 : -b2_pi / 2;
	jointDef.upperAngle = _isRight ? b2_pi / 2 : 0;
	jointDef.enableLimit = true;
	_flipperJoint = (b2RevoluteJoint*)world->CreateJoint(&jointDef);*/
}

void FlipperUnit::turnOn()
{
	_flipperJoint->SetMotorSpeed(_isRight ? 100 : -100);
}

void FlipperUnit::turnOff()
{
	_flipperJoint->SetMotorSpeed(_isRight ? -100 : 100);
}