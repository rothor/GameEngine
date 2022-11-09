#pragma once
#include "Unit.h"

class BlackHoleShot : public Unit
{
public:
	BlackHoleShot(b2World* world, float x, float y, int id);
	virtual void onContactBegin(Unit* otherUnit, int otherBodyId, int otherFixtureId, int thisBodyId, int thisFixtureId, b2Vec2 contactNormal) override;
	virtual void doEvent(std::wstring event) override;
	virtual void onDestroy(std::wstring event) override;
	void setAlpha(float alpha);
	void launchProjectile();
	void moveTo(float x, float y);

protected:
	int _bodyId;
	int _fixtureId;
	bool _prevWasIdle{ false };
	bool _facingLeft{ false };
	int _fadeCount{ 3 };
	int _launchCounter{ 0 };
	int _launchTimerId{ -1 };
};