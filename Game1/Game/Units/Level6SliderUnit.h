#pragma once
#include "Unit.h"

class Level6SliderUnit : public Unit
{
public:
	Level6SliderUnit(b2World* world, float x, float y, int id, std::wstring title);
	virtual void onContactBegin(Unit* unit, int fixtureId) override;
	virtual void onContactEnd(Unit* unit, int fixtureId) override;
	virtual std::list<DrawInstructions*>* getDrawInstructions() override;
	void moveSlider(bool positiveDirection);
	// getValue() returns a value between 0.0 and 1.0
	float getValue();
	void setValue(float value);
	bool isActive();

protected:
	float _sliderValue = 0;
	const float _sliderMinValue = 0.0f;
	const float _sliderMaxValue = 12000.0f;
	const float _sliderModifyValue = 100.0f;
	std::wstring _title;
	bool _isActive = false;
};

