#include "BlackHoleShot.h"

BlackHoleShot::BlackHoleShot(b2World* world, float x, float y, int id) :
	Unit(UnitType::BlackHoleShot, world, id)
{
	_bodyId = Unit::createBody(b2BodyType::b2_dynamicBody, x, y, id);
	Body* body = Unit::getBody(_bodyId);
	float width = 1.0f;
	float height = 1.0f;
	float density = 1.0f;
	float friction = 0.3f;
	float angle = 0.0f;
	float bounciness = 0.0f;
	std::vector<std::vector<GraphicsEnum>> animation = {
		{
			GraphicsEnum::BlackHole1, GraphicsEnum::BlackHole2, GraphicsEnum::BlackHole3, GraphicsEnum::BlackHole4,
			GraphicsEnum::BlackHole5, GraphicsEnum::BlackHole6, GraphicsEnum::BlackHole7, GraphicsEnum::BlackHole8
		}
	};
	_fixtureId = body->addBox(0, 0, width, height, angle, density, friction, bounciness, 0xFFFFFF, 0.01f, 0, 0.0f, animation, true);
	body->getFixture(_fixtureId)->setMillisecondsPerFrame(60);
	body->disable();
	_launchTimerId = TimableObject::addTimer(0.01f, L"launch");
	TimableObject::destroyTimerAfterAmountTime(_launchTimerId, 1.0f);
}

void BlackHoleShot::onContactBegin(Unit* otherUnit, int otherBodyId, int otherFixtureId, int thisBodyId, int thisFixtureId, b2Vec2 contactNormal)
{
	if (otherUnit->getUnitType() == UnitType::ColorFlyer) {
		Unit::setToDisable();
		TimableObject::addTimer(0.1f, L"fade");
	} else if (otherUnit->getUnitType() == UnitType::KillZone || otherUnit->getUnitType() == UnitType::WallBuilder) {
		FlagDestroy = true;
	}
}

void BlackHoleShot::doEvent(std::wstring event)
{
	if (event == L"fade") {
		_fadeCount--;
		Unit::getBody(_bodyId)->getFixture(_fixtureId)->setAlpha(_fadeCount / 3.0f);
		if (_fadeCount == 0) {
			FlagDestroy = true;
		}
	} else if (event == L"launch") {
		_launchCounter++;
		float alpha = _launchCounter / 100.f;
		setAlpha(alpha);
	}
}

void BlackHoleShot::onDestroy(std::wstring event)
{
	if (event == L"launch") {
		launchProjectile();
	}
}

void BlackHoleShot::setAlpha(float alpha)
{
	Unit::getBody(_bodyId)->getFixture(_fixtureId)->setAlpha(alpha);
	//Unit::getBody(_bodyId)->getFixture(_fixtureId)->setScale(alpha * 0.01f);
}

void BlackHoleShot::launchProjectile()
{
	Body* body = Unit::getBody(_bodyId);
	body->enable();
	body->setVelocity(0, 40);
	body->getBodyPtr()->SetGravityScale(0.0f);
}

void BlackHoleShot::moveTo(float x, float y)
{
	Unit::getBody(_bodyId)->placeAt(x, y);
}
