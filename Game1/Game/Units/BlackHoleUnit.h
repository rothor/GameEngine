#pragma once
#include "Unit.h"

class BlackHoleUnit : public Unit
{
public:
	BlackHoleUnit(b2World* world, float x, float y, int id, SoundEnum sound);
	virtual void onContactBegin(Unit* unit, int fixtureId) override;
	virtual std::vector<DrawInstructions*> getDrawInstructions() override;
	virtual void timeStepForGraphics(int microseconds) override;
	bool getSoundIsOn();
	SoundEnum getSound();

protected:
	SoundEnum _soundToPlay;
	bool _soundIsOn;
};

