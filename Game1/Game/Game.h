#pragma once
#include "GameClass/GameClass.h"
#include "AudioEngine.h"
#include "DrawableObject.h"
#include "GameObject.h"
#include "World.h"
#include "TimableObject.h"
#include <memory>
#include <map>

class Game : public GameClass, public TimableObject
{
public:
	Game(GraphicsInterface* giIn, KeyStateLogger* kslIn, AudioEngine* audioEngine);
	int tick(long long microsecondsEllapsed);
	void draw();
	virtual void engineMessage(std::wstring msg) override;
	virtual void doEvent(std::wstring event) override;

protected:
	void loadLevel1();
	void loadLevel2();
	void loadLevel3();

protected:
	World _world;
	bool _debugDraw{ false };
	bool _builderModeSwitch{ false };
	int _playerUnitId{ -1 };
	int _builderUnitId{ -1 };
};