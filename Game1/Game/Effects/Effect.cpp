#include "Effect.h"

Effect::Effect(EffectType type)
{
	_type = type;
}

EffectType Effect::getType()
{
	return _type;
}
