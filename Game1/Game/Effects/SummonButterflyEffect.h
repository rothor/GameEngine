#pragma once
#include "Effect.h"

class SummonButterflyEffect : public Effect
{
public:
	SummonButterflyEffect(float x, float y, float v);
	float getX();
	float getY();
	float getV();

protected:
	float _x;
	float _y;
	float _v;
};