#pragma once
#include "Unit.h"

class BubbleUnit : public Unit
{
public:
	BubbleUnit(b2World* world, float x, float y, int id);
	virtual std::list<DrawInstructions*>* getDrawInstructions() override;
	virtual void onContactBegin(Unit* otherUnit, int otherBodyId, int otherFixtureId, int thisBodyId, int thisFixtureId, b2Vec2 contactNormal) override;
	virtual void doEvent(std::wstring event) override;
	void setVelocity(float vX, float vY);

protected:
	int _animatedFixtureId{ 0 };
	int _animatedBodyId{ 0 };
	bool _fading{ false };
	int _fadeCount{ 3 };
	bool _isShot{ false };
};
