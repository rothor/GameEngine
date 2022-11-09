#pragma once
#include "Unit.h"

class Level6BasketUnit : public Unit
{
public:
	Level6BasketUnit(b2World* world, float x, float y, int id);
	virtual void onContactBegin(Unit* unit, int fixtureId) override;
	virtual std::vector<DrawInstructions*> getDrawInstructions() override;

protected:
	float _passedInX;
	float _passedInY;
	int _sensorFixtureId = -1;
	int _score = 0;
};

