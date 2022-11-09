#pragma once
#include "Effect.h"
#include "UnitEnum.h"
#include "box2d/box2d.h"

class SummonProjectileEffect : public Effect
{
public:
	SummonProjectileEffect(b2Vec2 projectilePos, UnitType unitToCreate, b2Vec2 projectileGravity,
		b2Vec2 launchVelocity);
	UnitType getUnitType();
	b2Vec2 getProjectileGravity();
	b2Vec2 getProjectilePos();
	b2Vec2 getLaunchVelocity();

protected:
	b2Vec2 _projectilePos;
	UnitType _unitToCreate;
	b2Vec2 _projectileGravity;
	b2Vec2 _launchVelocity;
};