#pragma once
#include "Effect.h"

class MoveUnitEffect : public Effect
{
public:
	MoveUnitEffect(int unitId, float x, float y);
	int getUnitId();
	float getX();
	float getY();

protected:
	int _unitId;
	float _x;
	float _y;
};

