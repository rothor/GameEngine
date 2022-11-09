#include "CombatAttack.h"

CombatAttack::CombatAttack(float castTime, float recoveryTime,
	UnitType unitToCreate, b2Vec2 projectileInitialVelocity,
	b2Vec2 projectileGravity)
{
	_castTime = castTime;
	_unitToCreate = unitToCreate;
	_projectileGravity = projectileGravity;
	_projectileInitialVelocity = projectileInitialVelocity;
	_recoveryTime = recoveryTime;
}

float CombatAttack::getCastTime()
{
	return _castTime;
}

UnitType CombatAttack::getUnitType()
{
	return _unitToCreate;
}

b2Vec2 CombatAttack::getProjectileGravity()
{
	return _projectileGravity;
}

b2Vec2 CombatAttack::getProjectileInitialVelocity()
{
	return _projectileInitialVelocity;
}

float CombatAttack::getRecoveryTime()
{
	return _recoveryTime;
}