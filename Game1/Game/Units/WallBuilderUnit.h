#pragma once
#include "Unit.h"

class WallBuilderUnit : public Unit
{
public:
	WallBuilderUnit(b2World* world, int unitId);
	virtual std::list<DrawInstructions*>* getDrawInstructions();
	virtual void onContactBegin(Unit* otherUnit, int otherBodyId, int otherFixtureId, int thisBodyId, int thisFixtureId, b2Vec2 contactNormal) override;
	void setFirstPoint(b2Vec2 firstPoint);
	void setSecondPoint(b2Vec2 secondPoint);
	void createWall();

protected:
	int _bodyId{ -1 };
	bool _isChoosingSecondPoint{ true };
	b2Vec2 _firstPoint;
	bool _isFirstPointChosen{ false };
	b2Vec2 _secondPoint;
	bool _isSecondPointChosen{ false };
};