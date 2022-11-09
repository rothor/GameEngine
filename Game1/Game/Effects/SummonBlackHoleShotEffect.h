#pragma once
#include "Effect.h"

class SummonBlackHoleShotEffect : public Effect
{
public:
	SummonBlackHoleShotEffect(float x, float y);
	float getX();
	float getY();

protected:
	float _x;
	float _y;
};