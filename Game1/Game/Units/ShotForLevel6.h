#pragma once
#include "Unit.h"

class ShotForLevel6 : public Unit
{
public:
	ShotForLevel6(b2World* world, float x, float y, int id);
	virtual void onContactBegin(Unit* unit, int fixtureId) override;
};