#include "BubbleUnit.h"

BubbleUnit::BubbleUnit(b2World* world, float x, float y, int id) :
	Unit(UnitType::Bubble, world, id)
{
	_animatedBodyId = PhysicsObject::createBody(b2BodyType::b2_dynamicBody, x, y, id);
	Body* body = Unit::getBody(_animatedBodyId);

	std::vector<std::vector<GraphicsEnum>> animation = {
		{
			GraphicsEnum::Bubble
		}
	};
	float width = 1.0f;
	float height = 1.0f;
	float density = 3.7f;
	float friction = 0.0f;
	float angle = 0.0f;
	float bounciness = 0.3f;
	float radius = 1.1f;
	float animationScale = 0.003f;
	float animationAlpha = 0.6;

	_animatedFixtureId = body->addEllipse(
		0, 0, radius, angle, density, friction, bounciness, 0xFF8000,
		animationScale, 0, animationAlpha, animation, false
	);
	//body->getFixture(_animatedFixtureId)->setAnimationAngleFixed(true);
	body->setFixedRotation(true);
}

std::list<DrawInstructions*>* BubbleUnit::getDrawInstructions()
{
	auto v = Unit::getDrawInstructions();
	return v;
}

void BubbleUnit::onContactBegin(Unit* otherUnit, int otherBodyId, int otherFixtureId, int thisBodyId, int thisFixtureId, b2Vec2 contactNormal)
{
	if (otherUnit->getUnitType() == UnitType::KillZone) {
		if (!_fading && !_isShot) {
			FlagDestroy = true;
			return;
			_fading = true;
			TimableObject::addTimer(0.1f, L"fade");
			//Unit::setToDisable();
		}
	} else if (otherUnit->getUnitType() == UnitType::BlackHoleShot) {
		if (!_fading && !_isShot) {
			_isShot = true;
			Unit::getBody(_animatedBodyId)->setVelocity(0, -5);
			TimableObject::addTimer(0.1f, L"fade");
			Unit::setToDisable();
		}
	}
}

void BubbleUnit::doEvent(std::wstring event)
{
	if (event == L"fade") {
		_fadeCount--;
		Unit::getBody(_animatedBodyId)->getFixture(_animatedFixtureId)->setAlpha(_fadeCount / 3.0f);
		if (_fadeCount == 0) {
			FlagDestroy = true;
		}
	}
}

void BubbleUnit::setVelocity(float vX, float vY)
{
	getBody(0)->setVelocity(vX, vY);
}