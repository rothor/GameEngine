#include "CombatObject.h"

CombatObject::CombatObject(float maxHp)
{
	_maxHp = maxHp;
	_currentHp = _maxHp;
}

void CombatObject::takeDamage(float dmg)
{
	_currentHp -= dmg;
	if (_currentHp < 0.0f) {
		_currentHp = 0.0f;
	}
}

void CombatObject::onCurrentHpChanged()
{

}

float CombatObject::getMaxHp()
{
	return _maxHp;
}

float CombatObject::getCurrentHp()
{
	return _currentHp;
}

std::list<DrawInstructions*>* CombatObject::getDrawInstructionsForHealthBar(float xTranslation, float yTranslation)
{
	std::list<DrawInstructions*>* diList = new std::list<DrawInstructions*>{};

	auto points = RectangleMaker::getPointsFromRect(xTranslation, yTranslation, 4.0f, 0.5f, 0);
	auto pointsFormatted = new std::vector<DrawInstructionsPoint>{};
	for (auto it = points->begin(); it != points->end(); it++) {
		pointsFormatted->push_back(DrawInstructionsPoint((*it).x, (*it).y));
	}
	auto redBar = new DrawInstructionsPolygon(pointsFormatted, 0xFF0000, 1.0f);
	delete points;
	diList->push_back(redBar);

	float greenX = (1.0f - (_currentHp / _maxHp)) / 2.0f * 4.0f;
	auto points2 = RectangleMaker::getPointsFromRect(-greenX + xTranslation, yTranslation, 4.0f * _currentHp / _maxHp, 0.5f, 0);
	auto pointsFormatted2 = new std::vector<DrawInstructionsPoint>{};
	for (auto it = points2->begin(); it != points2->end(); it++) {
		pointsFormatted2->push_back(DrawInstructionsPoint((*it).x, (*it).y));
	}
	auto greenBar = new DrawInstructionsPolygon(pointsFormatted2, 0x00FF00, 1.0f);
	delete points2;
	diList->push_back(greenBar);

	return diList;
}

