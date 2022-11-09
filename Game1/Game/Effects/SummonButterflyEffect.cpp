#include "SummonButterflyEffect.h"

SummonButterflyEffect::SummonButterflyEffect(float x, float y, float v) :
	Effect(EffectType::SummonButterfly)
{
	_x = x;
	_y = y;
	_v = v;
}

float SummonButterflyEffect::getX()
{
	return _x;
}

float SummonButterflyEffect::getY()
{
	return _y;
}

float SummonButterflyEffect::getV()
{
	return _v;
}
