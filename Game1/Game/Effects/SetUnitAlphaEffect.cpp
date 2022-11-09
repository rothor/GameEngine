#include "SetUnitAlphaEffect.h"

SetUnitAlphaEffect::SetUnitAlphaEffect(int unitId, float alpha) :
	Effect(EffectType::SetUnitAlpha)
{
	_unitId = unitId;
	_alpha = alpha;
}

float SetUnitAlphaEffect::getAlpha()
{
	return _alpha;
}

int SetUnitAlphaEffect::getUnitId()
{
	return _unitId;
}
