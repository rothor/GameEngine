#include "Body.h"

Body::Body()
{

}

Body::Body(b2Body* body)
{
	_body = body;
}

int Body::addBox(float x, float y, float w, float h, float angle,
	float density, float friction, float restitution,
	int color, float animationScale, float animationAngle, float animationAlpha,
	std::vector<std::vector<GraphicsEnum>> animation, bool isSensor,
	float animationAdjustX, float animationAdjustY)
{
	b2PolygonShape shape;
	auto points = RectangleMaker::getPointsFromRect(x, y, w, h, angle);
	shape.Set(&(*points)[0], 4);
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &shape;
	fixtureDef.density = density;
	fixtureDef.friction = friction;
	fixtureDef.restitution = restitution;
	fixtureDef.isSensor = isSensor;
	b2Fixture* fixture = _body->CreateFixture(&fixtureDef);
	int fixtureId = getNewFixtureId();
	fixture->SetUserData((void*)fixtureId);
	_fixtures[fixtureId] =
		Fixture(
			fixture, color, x + animationAdjustX, y + animationAdjustY, animationScale, animationAngle, animationAlpha, animation
		);
	delete points;
	return fixtureId;
}

int Body::addEllipse(float x, float y, float radius, float angle,
	float density, float friction, float restitution,
	int color, float animationScale, float animationAngle, float animationAlpha,
	std::vector<std::vector<GraphicsEnum>> animation, bool isSensor)
{
	b2CircleShape shape;
	shape.m_radius = radius;
	shape.m_p = b2Vec2(x, y);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &shape;
	fixtureDef.density = density;
	fixtureDef.friction = friction;
	fixtureDef.restitution = restitution;
	fixtureDef.isSensor = isSensor;
	b2Fixture* fixture = _body->CreateFixture(&fixtureDef);
	int fixtureId = getNewFixtureId();
	fixture->SetUserData((void*)fixtureId);
	_fixtures[fixtureId] =
		Fixture(
			fixture, color, x, y, animationScale, animationAngle, animationAlpha, animation
		);
	return fixtureId;
}

Fixture* Body::getFixture(int fixtureId)
{
	return &(_fixtures[fixtureId]);
}

int Body::getNewFixtureId()
{
	int id = _fixtureIdIncrementer;
	_fixtureIdIncrementer++;
	return id;
}

std::list<DrawInstructions*>* Body::getDrawInstructions()
{
	std::list<DrawInstructions*>* diList = new std::list<DrawInstructions*>();
	for (auto& pair : _fixtures) {
		if (pair.second.hasAnimation()) {
			if (_debugDraw) {
				auto a = pair.second.getDrawInstructionsShape();
				diList->push_back(a);
			}
			if (pair.second.shouldDraw()) {
				auto a = pair.second.getDrawInstructionsImage();
				diList->push_back(a);
			}
		} else {
			auto a = pair.second.getDrawInstructionsShape();
			diList->push_back(a);
		}
	}
	if (_debugDraw) {
		for (auto di : *diList) {
			if (di->type == DrawInstructions::DrawType::image) {
				DrawInstructionsImage* dii = static_cast<DrawInstructionsImage*>(di);
				if (dii->alpha > 0.7f) {
					dii->alpha *= 0.7f;
				}
			} else if (di->type == DrawInstructions::DrawType::text) {
				DrawInstructionsText* dit = static_cast<DrawInstructionsText*>(di);
				if (dit->alpha > 0.7f) {
					dit->alpha *= 0.7f;
				}
			} else if (di->type == DrawInstructions::DrawType::polygon) {
				DrawInstructionsPolygon* dip = static_cast<DrawInstructionsPolygon*>(di);
				dip->alpha = 1.0f;
			} else if (di->type == DrawInstructions::DrawType::ellipse) {
				DrawInstructionsEllipse* die = static_cast<DrawInstructionsEllipse*>(di);
				die->alpha = 1.0f;
			}
		}
	}
	return diList;
}

void Body::timeStepForGraphics(int microseconds)
{
	for (auto& fixture : _fixtures) {
		fixture.second.timeStepForGraphics(microseconds);
	}
}

void Body::push(bool left, bool right, bool up, bool down, float force)
{
	b2Vec2 forceVec((right - left) * force, (up - down) * force);
	_body->ApplyForceToCenter(forceVec, true);
}

void Body::push(float hori, float vert, float force)
{
	float m = sqrt(pow(hori, 2) + pow(vert, 2));
	if (m == 0)
		return;
	b2Vec2 forceVec(hori / m * force, vert / m * force);
	_body->ApplyForceToCenter(forceVec, true);
}

void Body::spin(float hori, float vert, float force)
{
	float m = sqrt(pow(hori, 2) + pow(vert, 2));
	if (m == 0)
		return;
	b2Vec2 forceVec(hori / m * force, vert / m * force);
	b2Vec2 pos = _body->GetWorldCenter();
	pos.x -= 0.2f;
	pos.y -= 0.0f;
	_body->ApplyForce(forceVec, pos, true);
}

void Body::megaPush(float hori, float vert, float force)
{
	float m = sqrt(pow(hori, 2) + pow(vert, 2));
	if (m == 0)
		return;
	b2Vec2 forceVec(hori / m * force, vert / m * force);
	_body->ApplyLinearImpulseToCenter(forceVec, true);
}

void Body::testMegaPush()
{
	_body->ApplyLinearImpulseToCenter(b2Vec2(10000, 10000), true);
}

void Body::setVelocity(float vX, float vY)
{
	_body->SetLinearVelocity(b2Vec2(vX, vY));
}

void Body::setHorizontalVelocity(float vX)
{
	b2Vec2 v = _body->GetLinearVelocity();
	v.x = vX;
	_body->SetLinearVelocity(v);
}

void Body::setVerticalVelocity(float vY)
{
	b2Vec2 v = _body->GetLinearVelocity();
	v.y = vY;
	_body->SetLinearVelocity(v);
}

// Currently, this only moves the first body in the list.
// You should not call this on Units with more than one body.
void Body::placeAt(float x, float y)
{
	_body->SetTransform(b2Vec2(x, y), 0);
}

void Body::regulate()
{
	const float MAX_ANGULAR_VELOCITY = 10.0f;
	if (_body->GetAngularVelocity() > MAX_ANGULAR_VELOCITY) {
		_body->SetAngularVelocity(MAX_ANGULAR_VELOCITY);
	} else if (_body->GetAngularVelocity() < -MAX_ANGULAR_VELOCITY) {
		_body->SetAngularVelocity(-MAX_ANGULAR_VELOCITY);
	}
	/*const float MAX_LINEAR_VELOCITY = 10.0f;
	b2Vec2 v = _body->GetLinearVelocity();
	if (v.Length() > MAX_LINEAR_VELOCITY) {
		v.Normalize();
		v.x *= MAX_LINEAR_VELOCITY;
		v.y *= MAX_LINEAR_VELOCITY;
		_body->SetLinearVelocity(v);
	}*/
}

// Returns the position of the first body in the list.
b2Vec2 Body::getPos()
{
	return _body->GetWorldPoint(b2Vec2(0, 0));
}

// Returns the velocity of the first body in the list.
b2Vec2 Body::getVelocity()
{
	return _body->GetLinearVelocity();
}

void Body::setDebugDraw(bool turnOn)
{
	_debugDraw = turnOn;
}

void Body::destroy()
{
	_body->GetWorld()->DestroyBody(_body);
}

void Body::setFixedRotation(bool fixedRotation)
{
	_body->SetFixedRotation(fixedRotation);
}

b2Body* Body::getBodyPtr()
{
	return _body;
}

void Body::disable()
{
	_body->SetEnabled(false);
}

void Body::enable()
{
	_body->SetEnabled(true);
}

void Body::setFixtureToDestroy(int fixtureId)
{
	getFixture(fixtureId)->setToDestroy(true);
}

void Body::destroyFixturesSetToDestroy()
{
	for (auto it = _fixtures.begin(); it != _fixtures.end(); ) {
		if (it->second.isSetToDestroy()) {
			b2Fixture* fixturePtr = it->second.getFixturePtr();
			_body->DestroyFixture(fixturePtr);
			it = _fixtures.erase(it);
		} else {
			it++;
		}
	}
}

void Body::overrideGravity(b2Vec2 gravity)
{
	_gravityIsOverridden = true;
	_overriddenGravity = gravity;
	_body->SetGravityScale(0.0f);
}

void Body::resetGravityToNormal()
{
	_gravityIsOverridden = false;
	_body->SetGravityScale(1.0f);
}

bool Body::getIsGravityOverridden()
{
	return _gravityIsOverridden;
}

void Body::applyOverriddenGravityToSelf()
{
	_body->ApplyForce(_overriddenGravity, b2Vec2(0, 0), false);
}
