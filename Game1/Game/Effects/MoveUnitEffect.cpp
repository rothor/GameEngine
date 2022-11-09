#include "MoveUnitEffect.h"

MoveUnitEffect::MoveUnitEffect(int unitId, float x, float y) :
	Effect(EffectType::MoveUnit)
{
	_unitId = unitId;
	_x = x;
	_y = y;
}

int MoveUnitEffect::getUnitId()
{
	return _unitId;
}

float MoveUnitEffect::getX()
{
	return _x;
}

float MoveUnitEffect::getY()
{
	return _y;
}
