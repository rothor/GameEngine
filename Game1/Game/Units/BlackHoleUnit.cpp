#include "BlackHoleUnit.h"

BlackHoleUnit::BlackHoleUnit(b2World* world, float x, float y, int id, SoundEnum sound) :
	_soundToPlay(sound),
	Unit::Unit(UnitType::BlackHole)
{
	float radius = 1.0f;
	float density = 1.0f;
	float friction = 0.3f;
	float angle = 0.0f;
	float bounciness = 0.0f;
	Unit::addEllipse(0, 0, radius, angle, density, friction, bounciness, 0x00DEEF, 1.0, GraphicsMap::idMap[GraphicsEnum::PurpleArrow], 0.008f, true);
	_fixtures[0].setMillisecondsPerFrame(83);
	_bodies[0]->SetFixedRotation(true);

	std::vector<std::vector<GraphicsEnum>> animationVector = {
		{
			GraphicsEnum::BlackHole1, GraphicsEnum::BlackHole2, GraphicsEnum::BlackHole3, GraphicsEnum::BlackHole4,
			GraphicsEnum::BlackHole5, GraphicsEnum::BlackHole6, GraphicsEnum::BlackHole7, GraphicsEnum::BlackHole8
		}
	};
	_fixtures[0].setAnimations(animationVector);
	_fixtures[0].chooseAnimation(0);
	
}

void BlackHoleUnit::onContactBegin(Unit* unit, int fixtureId)
{
	if (unit->getUnitType() == UnitType::Player) {
		_soundIsOn = !_soundIsOn;
	}
}

std::vector<DrawInstructions*> BlackHoleUnit::getDrawInstructions()
{
	return Unit::getDrawInstructions();
}

void BlackHoleUnit::timeStepForGraphics(int microseconds)
{
	if (_soundIsOn)
		Unit::timeStepForGraphics(microseconds);
}

bool BlackHoleUnit::getSoundIsOn()
{
	return _soundIsOn;
}

SoundEnum BlackHoleUnit::getSound()
{
	return _soundToPlay;
}
