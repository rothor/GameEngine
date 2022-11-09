#include "ColorFlyer.h"
#include "../GraphicsMap.h"

ColorFlyer::ColorFlyer(b2World* world, float x, float y, int color, int id) :
	Unit(UnitType::ColorFlyer, world, id)
{
	_animatedBodyId = Unit::createBody(b2BodyType::b2_dynamicBody, x, y, id);
	Body* body = Unit::getBody(_animatedBodyId);

	std::vector<std::vector<GraphicsEnum>> animation = {
		{
			GraphicsEnum::CircleRune2
			//GraphicsEnum::Butterfly1, GraphicsEnum::Butterfly2, GraphicsEnum::Butterfly3,
			//GraphicsEnum::Butterfly4, GraphicsEnum::Butterfly3, GraphicsEnum::Butterfly2
		}
	};
	float width = 1.0f;
	float height = 1.0f;
	float density = 3.7f;
	float friction = 0.3f;
	float angle = 0.0f;
	float bounciness = 0.3f;
	
	// Scale 0.05f
	_animatedFixtureId = body->addBox(0, 0, 1.0f, 0.75f, angle, density, friction, bounciness, 0xFF8000, 0.005f, 0, 1.0f, animation, false);
	body->getFixture(_animatedFixtureId)->setMillisecondsPerFrame(83);
}

std::list<DrawInstructions*>* ColorFlyer::getDrawInstructions()
{
	auto v = Unit::getDrawInstructions();
	for (auto di : *v) {
		if (di->type == DrawInstructions::DrawType::image) {
			DrawInstructionsImage* dii = static_cast<DrawInstructionsImage*>(di);
			dii->angle = 0;
		}
	}
	return v;
}

void ColorFlyer::onContactBegin(Unit* otherUnit, int otherBodyId, int otherFixtureId, int thisBodyId, int thisFixtureId, b2Vec2 contactNormal)
{
	if (otherUnit->getUnitType() == UnitType::KillZone) {
		if (!_fading && !_isShot) {
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

void ColorFlyer::doEvent(std::wstring event)
{
	if (event == L"fade") {
		_fadeCount--;
		Unit::getBody(_animatedBodyId)->getFixture(_animatedFixtureId)->setAlpha(_fadeCount / 3.0f);
		if (_fadeCount == 0) {
			FlagDestroy = true;
		}
	}
}

void ColorFlyer::setVelocity(float vX, float vY)
{
	getBody(0)->setVelocity(vX, vY);
}
