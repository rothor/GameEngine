#include "Level6SliderUnit.h"
#include "Player.h"
#include "../GameEngine/GraphicsInterface/DrawInstructionsText.h"

Level6SliderUnit::Level6SliderUnit(b2World* world, float x, float y, int id, std::wstring title) :
	Unit(UnitType::Level6Basket)
{
	_title = title;
	Unit::addBox(0, 0, 4.0f, 3.0f, 0, 2, 0.3f, 0.3f, 0xFFFFFF, 1.0f, 0, 1.0f, true);
}

void Level6SliderUnit::onContactBegin(Unit* unit, int fixtureId)
{
	if (unit->getUnitType() == UnitType::Player) {
		_isActive = true;
		_soundIdQueue.push_back(SoundEnum::PianoA6);
		Player* player = static_cast<Player*>(unit);
	}
}

void Level6SliderUnit::onContactEnd(Unit* unit, int fixtureId)
{
	if (unit->getUnitType() == UnitType::Player) {
		_isActive = false;
		_soundIdQueue.push_back(SoundEnum::PianoB6);
		Player* player = static_cast<Player*>(unit);
	}
}

std::vector<DrawInstructions*> Level6SliderUnit::getDrawInstructions()
{
	b2Vec2 center = _fixtures[0]._fixture->GetBody()->GetWorldPoint(b2Vec2(0, 0));
	std::vector<DrawInstructions*> inst = {};
	// Background
	auto pts = Unit::getPointsFromRect(center.x, center.y + 1.5f, 4.0f, 6.0f, 0);
	std::vector<DrawInstructionsPoint>* drawPts = new std::vector<DrawInstructionsPoint> {};
	for (auto& pt : *pts) {
		drawPts->push_back(DrawInstructionsPoint(pt.x, pt.y));
	}
	DrawInstructionsPolygon* bg = new DrawInstructionsPolygon(drawPts, 0x000000, 1.0f);
	inst.push_back(bg);
	// Title
	DrawInstructionsText* title = new DrawInstructionsText(DrawInstructionsPoint(center.x - 1.2f, center.y + 4), _title, L"Georgia", 0.7f, 0, 0xFFFFFF, 1.0f);
	inst.push_back(title);
	// square
	pts = Unit::getPointsFromRect(center.x, center.y + 2, 2.2f, 0.3f, 0);
	drawPts = new std::vector<DrawInstructionsPoint>{};
	for (auto& pt : *pts) {
		drawPts->push_back(DrawInstructionsPoint(pt.x, pt.y));
	}
	DrawInstructionsPolygon* holder = new DrawInstructionsPolygon(drawPts, 0x808080, 1.0f);
	inst.push_back(holder);
	// notch
	pts = Unit::getPointsFromRect(center.x - 2.2f / 2 + 2.2f * getValue(), center.y + 2, 1.0f, 1.0f, 0);
	drawPts = new std::vector<DrawInstructionsPoint>{};
	for (auto& pt : *pts) {
		drawPts->push_back(DrawInstructionsPoint(pt.x, pt.y));
	}
	DrawInstructionsPolygon* notch = new DrawInstructionsPolygon(drawPts, 0xB0B0B0, 1.0f);
	inst.push_back(notch);
	// hitbox
	if (_debugDraw) {
		inst.push_back(_fixtures[0].getDrawInstructionsShape());
	}
	return inst;
}

void Level6SliderUnit::moveSlider(bool positiveDirection)
{

	if (positiveDirection) {
		_sliderValue += _sliderModifyValue;
		if (_sliderValue > _sliderMaxValue)
			_sliderValue = _sliderMaxValue;
	} else {
		_sliderValue -= _sliderModifyValue;
		if (_sliderValue < _sliderMinValue)
			_sliderValue = _sliderMinValue;
	}
}

float Level6SliderUnit::getValue()
{
	return _sliderValue / _sliderMaxValue;
}

void Level6SliderUnit::setValue(float value)
{
	_sliderValue = _sliderMinValue + (_sliderMaxValue - _sliderMinValue) * value;
}

bool Level6SliderUnit::isActive()
{
	return _isActive;
}

