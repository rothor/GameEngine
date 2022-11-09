#include "SummonBlackHoleShotEffect.h"

SummonBlackHoleShotEffect::SummonBlackHoleShotEffect(float x, float y) :
	Effect(EffectType::SummonBlackHoleShot)
{
	_x = x;
	_y = y;
}

float SummonBlackHoleShotEffect::getX()
{
	return _x;
}

float SummonBlackHoleShotEffect::getY()
{
	return _y;
}
