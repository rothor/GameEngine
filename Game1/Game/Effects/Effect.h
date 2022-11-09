#pragma once

enum class EffectType
{
	SummonBlackHoleShot, SetUnitAlpha, LaunchPendingProjectile, CancelProjectile, MoveUnit,
	SummonButterfly, SummonProjectile
};

class Effect
{
public:
	Effect(EffectType type);
	EffectType getType();

protected:
	EffectType _type;
};