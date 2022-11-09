#pragma once
#include "Effect.h"

class CancelProjectileEffect : public Effect
{
public:
	CancelProjectileEffect(int unitId);
	int getUnitId();

protected:
	int _unitId;
};