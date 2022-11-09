#include "CancelProjectileEffect.h"

CancelProjectileEffect::CancelProjectileEffect(int unitId) :
	Effect(EffectType::LaunchPendingProjectile)
{
	_unitId = unitId;
}

int CancelProjectileEffect::getUnitId()
{
	return _unitId;
}
