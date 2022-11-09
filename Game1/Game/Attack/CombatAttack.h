#pragma once
#include "UnitEnum.h"
#include "box2d/box2d.h"

class CombatAttack
{
public:
	enum Trajectory { StraightAhead, Up, Down };

public:
	CombatAttack(float castTime, float recoveryTime,
		UnitType unitToCreate, b2Vec2 projectileInitialVelocity,
		b2Vec2 projectileGravity);
	float getCastTime();
	UnitType getUnitType();
	b2Vec2 getProjectileGravity();
	b2Vec2 getProjectileInitialVelocity();
	float getRecoveryTime();

protected:
	float _castTime;
	UnitType _unitToCreate;
	b2Vec2 _projectileGravity;
	b2Vec2 _projectileInitialVelocity;
	float _recoveryTime;
};