#include "CollisionManager.h"

CollisionManagerCollision::CollisionManagerCollision(int unitId, int bodyId, int fixtureId, b2Vec2 normalForce)
{
	_unitId = unitId;
	_bodyId = bodyId;
	_fixtureId = fixtureId;
	_normalForce = normalForce;

	const float MAX_FLOOR_SLOPE_THRESHOLD = 0.5f; // This is the maximum slope of a floor before it is considered a wall and you can't walk up it anymore.
	const float floorNormalSlopeThreshold = 1.0f / MAX_FLOOR_SLOPE_THRESHOLD;
	if (_normalForce.y < 0) {
		_isCeiling = true;
		_isFloor = false;
		if (_normalForce.x > 0) {
			_isLeftWall = true;
			_isRightWall = false;
		} else {
			_isLeftWall = false;
			_isRightWall = true;
		}
	} else {
		_isCeiling = false;
		if (_normalForce.x == 0) {
			_isFloor = true;
			_isLeftWall = false;
			_isRightWall = false;
		} else if (fabsf(_normalForce.y / _normalForce.x) >= floorNormalSlopeThreshold) {
			_isFloor = true;
			_isLeftWall = false;
			_isRightWall = false;
		} else {
			_isFloor = false;
			if (_normalForce.x > 0) {
				_isLeftWall = true;
				_isRightWall = false;
			} else {
				_isLeftWall = false;
				_isRightWall = true;
			}
		}
	}
}

bool CollisionManagerCollision::is(int unitId, int bodyId, int fixtureId)
{
	return _unitId == unitId && _bodyId == bodyId && _fixtureId == fixtureId;
}

bool CollisionManagerCollision::isFloor()
{
	return _isFloor;
}

bool CollisionManagerCollision::isLeftWall()
{
	return _isLeftWall;
}

bool CollisionManagerCollision::isRightWall()
{
	return _isRightWall;
}

bool CollisionManagerCollision::isCeiling()
{
	return _isCeiling;
}

CollisionManager::CollisionManager()
{

}

void CollisionManager::addCollision(int unitId, int bodyId, int fixtureId, b2Vec2 normalForce)
{
	_collisionList.push_back(CollisionManagerCollision(unitId, bodyId, fixtureId, normalForce));
}

void CollisionManager::removeCollision(int unitId, int bodyId, int fixtureId)
{
	for (auto it = _collisionList.begin(); it != _collisionList.end(); ) {
		if ((*it).is(unitId, bodyId, fixtureId)) {
			_collisionList.erase(it);
			return;
		} else {
			it++;
		}
	}
}

bool CollisionManager::isOnFloor()
{
	for (auto& collision : _collisionList) {
		if (collision.isFloor()) {
			return true;
		}
	}
	return false;
}

bool CollisionManager::isOnLeftWall()
{
	for (auto& collision : _collisionList) {
		if (collision.isLeftWall()) {
			return true;
		}
	}
	return false;
}

bool CollisionManager::isOnRightWall()
{
	for (auto& collision : _collisionList) {
		if (collision.isRightWall()) {
			return true;
		}
	}
	return false;
}

bool CollisionManager::isOnCeiling()
{
	for (auto& collision : _collisionList) {
		if (collision.isCeiling()) {
			return true;
		}
	}
	return false;
}
