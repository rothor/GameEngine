#pragma once
#include "UnitEnum.h"

class GameObject
{
public:
	GameObject(UnitType unitType, bool isAnimatableObject, bool isPhysicsObject, bool isSoundableObject);
	UnitType getUnitType();
	bool isAnimatableObject();
	bool isPhysicsObject();
	bool isSoundableObject();

protected:
	UnitType _unitType;
	bool _isAnimatableObject;
	bool _isPhysicsObject;
	bool _isSoundableObject;
};