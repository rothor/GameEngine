#include "GameObject.h"

GameObject::GameObject(UnitType unitType, bool isAnimatableObject, bool isPhysicsObject, bool isSoundableObject) :
	_unitType(unitType),
	_isAnimatableObject(isAnimatableObject),
	_isPhysicsObject(isPhysicsObject),
	_isSoundableObject(isSoundableObject)
{

}

UnitType GameObject::getUnitType()
{
	return _unitType;
}

bool GameObject::isAnimatableObject()
{
	return _isAnimatableObject;
}

bool GameObject::isPhysicsObject()
{
	return _isPhysicsObject;
}

bool GameObject::isSoundableObject()
{
	return _isSoundableObject;
}
