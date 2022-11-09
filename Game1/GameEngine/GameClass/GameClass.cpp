#include "GameClass.h"



GameClass::GameClass(GraphicsInterface* giIn, KeyStateLogger* kslIn, AudioEngine* audioEngine)
{
	_graphics = giIn;
	_userInput = kslIn;
	_audioEngine = audioEngine;
}


GameClass::~GameClass()
{

}
