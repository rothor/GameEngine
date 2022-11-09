#include "Level6BasketUnit.h"
#include "../../GameEngine/GraphicsInterface/DrawInstructionsText.h"

Level6BasketUnit::Level6BasketUnit(b2World* world, float x, float y, int id) :
	Unit(UnitType::Level6Basket)
{
	_passedInX = x;
	_passedInY = y;
	Unit::addBox(0, -1.5f, 3.0f, 0.5f, 0, 2, 0.3f, 0.3f, 0x00FF00);
	Unit::addBox(-1.5f, 0, 0.5f, 3.5f, 0, 2, 0.3f, 0.3f, 0x00FF00);
	Unit::addBox(1.5f, 0, 0.5f, 3.5f, 0, 2, 0.3f, 0.3f, 0x00FF00);
	_sensorFixtureId = Unit::addBox(0, 0, 1.5f, 1.5f, 0, 2, 0.3f, 0.3f, 0x0000FF, 0.0f, 0, 1.0f, true);
}

void Level6BasketUnit::onContactBegin(Unit* unit, int fixtureId)
{
	if (fixtureId == _sensorFixtureId) {
		if (unit->getUnitType() == UnitType::ShotForLevel6) {
			unit->FlagDestroy = true;
			_score++;
		}
	}
}

std::vector<DrawInstructions*> Level6BasketUnit::getDrawInstructions()
{
	auto inst = Unit::getDrawInstructions();
	inst.push_back(new DrawInstructionsEllipse(DrawInstructionsPoint(_passedInX, _passedInY), 0.1f, 0.1f, 0, 0x000000, 1.0f));
	auto pts = Unit::getPointsFromRect(_passedInX, _passedInY + 0.25f, 2.5f, 3.0f, 0);
	std::vector<DrawInstructionsPoint>* ptsVec = new std::vector<DrawInstructionsPoint>{
		DrawInstructionsPoint(pts->at(0).x, pts->at(0).y),
		DrawInstructionsPoint(pts->at(1).x, pts->at(1).y),
		DrawInstructionsPoint(pts->at(2).x, pts->at(2).y),
		DrawInstructionsPoint(pts->at(3).x, pts->at(3).y)
	};
	delete pts;
	inst.push_back(new DrawInstructionsPolygon(ptsVec, 0x0000FF, 1.0f));
	inst.push_back(new DrawInstructionsText(DrawInstructionsPoint(getPos().x - 1.04f, getPos().y - 2.0f), std::to_wstring(_score), L"Georgia", 1.5f, 0.0f, 0xFFFFFF, 1.0f));
	return inst;
}
