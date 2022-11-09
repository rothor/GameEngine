#include "Wall.h"


Wall::Wall(b2World* world, float x, float y, float w, float h, float angle, int color, int id, bool bouncy, bool visible) :
	Unit(UnitType::Wall, world, id)
{
	int bodyId = Unit::createBody(b2BodyType::b2_staticBody, x, y, id);
	float density = 1.0f;
	float friction = bouncy ? 0.0f : 0.3f;
	float bounciness = bouncy ? 1.0f : 0.0f;
	std::vector<std::vector<GraphicsEnum>> animation;
	if (visible) {
		animation = {};
	} else {
		animation = { { GraphicsEnum::None } };
	}
	Unit::getBody(bodyId)->addBox(0, 0, w, h, angle, density, friction, bounciness, color, 0.0f, 0.0f, 1.0f, animation, false);
	Unit::FlagIsStatic = true;
}