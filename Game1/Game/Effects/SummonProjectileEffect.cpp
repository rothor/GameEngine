#include "SummonProjectileEffect.h"

SummonProjectileEffect::SummonProjectileEffect(b2Vec2 projectilePos, UnitType unitToCreate, b2Vec2 projectileGravity,
	b2Vec2 launchVelocity) :
	Effect(EffectType::SummonProjectile)
{
	_projectilePos = projectilePos;
	_unitToCreate = unitToCreate;
	_projectileGravity = projectileGravity;
	_launchVelocity = launchVelocity;
}

UnitType SummonProjectileEffect::getUnitType()
{
	return _unitToCreate;
}

b2Vec2 SummonProjectileEffect::getProjectileGravity()
{
	return _projectileGravity;
}

b2Vec2 SummonProjectileEffect::getProjectilePos()
{
	return _projectilePos;
}

b2Vec2 SummonProjectileEffect::getLaunchVelocity()
{
	return _launchVelocity;
}
