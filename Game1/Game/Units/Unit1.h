#pragma once
#include "box2d/box2d.h"
#include "Unit.h"


class Unit1 : public Unit
{
public:
	Unit1(b2World* world, float x, float y, int id, SoundEnum sound);
	virtual void onContactBegin(Unit* otherUnit, int otherBodyId, int otherFixtureId, int thisBodyId, int thisFixtureId, b2Vec2 contactNormal) override;

protected:
	SoundEnum _sound;
};