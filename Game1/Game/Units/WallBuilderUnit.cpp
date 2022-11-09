#include "WallBuilderUnit.h"
#include "Lib/RectangleMaker.h"

WallBuilderUnit::WallBuilderUnit(b2World* world, int unitId) :
	Unit(UnitType::WallBuilder, world, unitId)
{
	_bodyId = Unit::createBody(b2BodyType::b2_staticBody, 0, 0, unitId);
	Unit::FlagIsStatic = true;
}

std::list<DrawInstructions*>* WallBuilderUnit::getDrawInstructions()
{
	auto diList = Unit::getDrawInstructions();
	if (_isFirstPointChosen) {
		DrawInstructionsEllipse* die = new DrawInstructionsEllipse(DrawInstructionsPoint(_firstPoint.x, _firstPoint.y), 0.5f, 0.5f, 0.0f, 0xFF0000, 1.0f);
		diList->push_back(die);
	}
	if (_isFirstPointChosen && _isSecondPointChosen) {
		DrawInstructionsEllipse* die = new DrawInstructionsEllipse(DrawInstructionsPoint(_secondPoint.x, _secondPoint.y), 0.5f, 0.5f, 0.0f, 0xFF0000, 1.0f);
		diList->push_back(die);
		
		auto rectPoints = RectangleMaker::getPointsFromDraggableRect(_firstPoint, _secondPoint, 1.0f);
		Body* body = Unit::getBody(_bodyId);
		std::vector<DrawInstructionsPoint>* vertexes = new std::vector<DrawInstructionsPoint>();
		vertexes->reserve(4);
		for (int i = 0; i < 4; i++) {
			b2Vec2 worldVec = body->getBodyPtr()->GetWorldPoint(b2Vec2((*rectPoints)[i].x, (*rectPoints)[i].y));
			vertexes->push_back(DrawInstructionsPoint(worldVec.x, worldVec.y));
		}
		delete rectPoints;
		auto dip = new DrawInstructionsPolygon(
			vertexes,
			0x00DEEF,
			1.0f
		);
		diList->push_back(dip);
	}
	return diList;
}

void WallBuilderUnit::onContactBegin(Unit* otherUnit, int otherBodyId, int otherFixtureId, int thisBodyId, int thisFixtureId, b2Vec2 contactNormal)
{
	if (otherUnit->getUnitType() == UnitType::BlackHoleShot) {
		Unit::getBody(thisBodyId)->setFixtureToDestroy(thisFixtureId);
	}
}

void WallBuilderUnit::setFirstPoint(b2Vec2 firstPoint)
{
	_firstPoint = firstPoint;
	_isFirstPointChosen = true;
}

void WallBuilderUnit::setSecondPoint(b2Vec2 secondPoint)
{
	_secondPoint = secondPoint;
	_isSecondPointChosen = true;
}

void WallBuilderUnit::createWall()
{
	if (!_isFirstPointChosen || !_isSecondPointChosen) {
		return;
	}
	if (_firstPoint == _secondPoint) {
		return;
	}
	
	b2Vec2 center;
	float width;
	float angle;
	RectangleMaker::getDimensionsFromDraggableRect(_firstPoint, _secondPoint, center, width, angle);
	float height = 1.0f;
	float friction = 0.8f;
	Unit::getBody(_bodyId)->addBox(center.x, center.y, width, height, angle, 1.0f, friction, 0.0f, 0x00FF00, 1.0f, 0.0f, 1.0f, {}, false, 0.0f, 0.0f);

	_isFirstPointChosen = false;
	_isSecondPointChosen = false;
}
