#pragma once
#include "Effect.h"

class LaunchPendingProjectileEffect : public Effect
{
public:
	LaunchPendingProjectileEffect(int unitId);
	int getUnitId();

protected:
	int _unitId;
};