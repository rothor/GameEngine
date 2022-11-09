#pragma once
#include "Effect.h"

class SetUnitAlphaEffect : public Effect
{
public:
	SetUnitAlphaEffect(int unitId, float alpha);
	float getAlpha();
	int getUnitId();

protected:
	int _unitId;
	float _alpha;
};