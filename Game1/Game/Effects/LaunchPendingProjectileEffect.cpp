#include "LaunchPendingProjectileEffect.h"

LaunchPendingProjectileEffect::LaunchPendingProjectileEffect(int unitId) :
	Effect(EffectType::LaunchPendingProjectile)
{
	_unitId = unitId;
}

int LaunchPendingProjectileEffect::getUnitId()
{
	return _unitId;
}
