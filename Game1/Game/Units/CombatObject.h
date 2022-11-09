#pragma once
#include "DrawInstructionsPolygon.h"
#include "Lib/RectangleMaker.h"
#include <list>

class CombatObject
{
public:
	CombatObject(float maxHp);
	void takeDamage(float dmg);
	virtual void onCurrentHpChanged(); // Units can choose how to respond to HP change by overriding this.
	float getMaxHp();
	float getCurrentHp();
	std::list<DrawInstructions*>* getDrawInstructionsForHealthBar(float xTranslation, float yTranslation);

protected:
	float _maxHp;
	float _currentHp;
};