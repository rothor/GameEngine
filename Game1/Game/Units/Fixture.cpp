#include "Fixture.h"


Fixture::Fixture()
{

}

Fixture::Fixture(b2Fixture* pFixture, int pColor, float pAnimationX, float pAnimationY, float pAnimationScale,
	float pAnimationAngle, float pAnimationAlpha, std::vector<std::vector<GraphicsEnum>> pAnimation) :
	_fixture(pFixture),
	_color(pColor),
	AnimatableObject(pAnimationX, pAnimationY, pAnimationScale, pAnimationAngle, pAnimationAlpha, pAnimation)
{
	AnimatableObject::setAnimations(pAnimation);
	if (AnimatableObject::hasAnimation()) {
		AnimatableObject::chooseAnimation(0); // Automatically start using the first animation by default. Can be changed at any time.
	}
}

std::list<DrawInstructions*>* Fixture::getDrawInstructions()
{
	if (AnimatableObject::hasAnimation()) {
		return new std::list<DrawInstructions*>{ getDrawInstructionsImage() };
	} else {
		return new std::list<DrawInstructions*>{ getDrawInstructionsShape() };
	}
}

void Fixture::setColor(int color)
{
	_color = color;
}

DrawInstructions* Fixture::getDrawInstructionsImage()
{
	b2Vec2 pos = _fixture->GetBody()->GetWorldPoint(b2Vec2(_animationX, _animationY));
	float angle = 0;
	if (!AnimatableObject::getAnimationAngleIsFixed()) {
		angle = (_animationAngle - _fixture->GetBody()->GetAngle()) / b2_pi * 180;
	}
	return new DrawInstructionsImage(
		DrawInstructionsPoint(pos.x, pos.y),
		_fixture->GetShape()->GetType() == b2Shape::Type::e_circle ? _animationScale * 2 : _animationScale,
		angle,
		GraphicsMap::idMap[AnimatableObject::getAnimationImage()],
		_animationAlpha,
		false
	);
}

DrawInstructions* Fixture::getDrawInstructionsShape()
{
	b2Shape* shape = _fixture->GetShape();
	b2Shape::Type type = shape->GetType();
	if (type == b2Shape::Type::e_polygon) {
		b2PolygonShape* poly = static_cast<b2PolygonShape*>(shape);
		std::vector<DrawInstructionsPoint>* vertexes = new std::vector<DrawInstructionsPoint>();
		vertexes->reserve(poly->m_count);
		for (int i = 0; i < poly->m_count; i++) {
			b2Vec2 bodyVec = poly->m_vertices[i];
			b2Vec2 worldVec = _fixture->GetBody()->GetWorldPoint(bodyVec);
			vertexes->push_back(DrawInstructionsPoint(worldVec.x, worldVec.y));
		}
		return new DrawInstructionsPolygon(
			vertexes,
			_color,
			_animationAlpha
		);
	}
	else if (type == b2Shape::Type::e_circle) {
		b2CircleShape* circle = static_cast<b2CircleShape*>(shape);
		b2Vec2 pos = _fixture->GetBody()->GetWorldPoint(b2Vec2(_animationX, _animationY));
		return new DrawInstructionsEllipse(
			DrawInstructionsPoint(pos.x, pos.y),
			circle->m_radius * 1,
			circle->m_radius * 1,
			(_animationAngle - _fixture->GetBody()->GetAngle()) / b2_pi * 180,
			_color,
			_animationAlpha
		);
	}
}

b2Fixture* Fixture::getFixturePtr()
{
	return _fixture;
}

void Fixture::setToDestroy(bool destroy)
{
	_destroy = destroy;
}

bool Fixture::isSetToDestroy()
{
	return _destroy;
}

