#pragma once
#include "GraphicsInterface.h"
#include "KeyStateLogger.h"
#include "AudioEngine.h"

class GameClass
{
public:
	GameClass(GraphicsInterface* giIn, KeyStateLogger* kslIn, AudioEngine* audioEngine);
	~GameClass();
	virtual int tick(long long microsecondsEllapsed) = 0;
	virtual void draw() = 0;
	virtual void engineMessage(std::wstring msg) = 0;

protected:
	GraphicsInterface* _graphics;
	KeyStateLogger* _userInput;
	AudioEngine* _audioEngine;
};

