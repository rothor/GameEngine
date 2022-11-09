#pragma once
#include "box2d/box2d.h"
#include "DrawInstructionsInclude.h"
#include "AnimatableObject.h"
#include "DrawableObject.h"
#include <list>

class Fixture : public AnimatableObject, public DrawableObject
{
public:
	Fixture();
	Fixture(b2Fixture* pFixture, int pColor, float pAnimationX, float pAnimationY, float pAnimationScale,
		float pAnimationAngle, float pAnimationAlpha, std::vector<std::vector<GraphicsEnum>> pAnimation);
	virtual std::list<DrawInstructions*>* getDrawInstructions() override;
	void setColor(int color);
	DrawInstructions* getDrawInstructionsImage();
	DrawInstructions* getDrawInstructionsShape();
	b2Fixture* getFixturePtr();
	void setToDestroy(bool destroy);
	bool isSetToDestroy();

protected:
	b2Fixture* _fixture;
	int _color;
	bool _destroy{ false };
};