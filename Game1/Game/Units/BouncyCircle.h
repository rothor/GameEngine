#pragma once
#include "Unit1.h"

class BouncyCircle : public Unit
{
public:
	BouncyCircle(b2World* world, float x, float y, float radius, int color, int id, SoundEnum sound);
	virtual void onContactBegin(Unit* otherUnit, int otherBodyId, int otherFixtureId, int thisBodyId, int thisFixtureId, b2Vec2 contactNormal) override;

protected:
	SoundEnum _sound;
};

