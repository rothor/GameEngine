#include "KeyStateLogger.h"


KeyStateLogger::KeyStateLogger()
{
	
}

void KeyStateLogger::notifyKeyPressed(int keyCodeIn)
{
	keyState[keyCodeIn] = true;
	if (keysPressedThisFrameCount < 5) {
		keysPressedThisFrame[keysPressedThisFrameCount] = keyCodeIn;
		keysPressedThisFrameCount++;
	}
}

void KeyStateLogger::notifyKeyReleased(int keyCodeIn)
{
	keyState[keyCodeIn] = false;
}

bool KeyStateLogger::getKeyState(int keyCodeIn)
{
	return keyState[keyCodeIn];
}

bool KeyStateLogger::wasKeyPressed(int keyCodeIn)
{
	for (int i{ 0 }; i < keysPressedLastFrameCount; i++) {
		if (keysPressedLastFrame[i] == keyCodeIn)
			return true;
	}

	return false;
}



void KeyStateLogger::notifyMousePressed(mouseButton mbIn, int x, int y)
{
	if (mbIn == mouseButton::left) {
		lButtonPressedThisFrame = true;
		lButtonState = true;
		lButtonClickXThisFrame = x;
		lButtonClickYThisFrame = y;
	}
	else if (mbIn == mouseButton::right) {
		rButtonPressedThisFrame = true;
		rButtonState = true;
		rButtonClickXThisFrame = x;
		rButtonClickYThisFrame = y;
	}
	else if (mbIn == mouseButton::middle) {
		mButtonPressedThisFrame = true;
		mButtonState = true;
		mButtonClickXThisFrame = x;
		mButtonClickYThisFrame = y;
	}
}

void KeyStateLogger::notifyMouseReleased(mouseButton mbIn, int x, int y)
{
	if (mbIn == mouseButton::left) {
		lButtonState = false;
	}
	else if (mbIn == mouseButton::right) {
		rButtonState = false;
	}
	else if (mbIn == mouseButton::middle) {
		mButtonState = false;
	}
}

bool KeyStateLogger::getMouseState(mouseButton mbIn)
{
	if (mbIn == mouseButton::left) {
		return lButtonState;
	}
	else if (mbIn == mouseButton::right) {
		return rButtonState;
	}
	else if (mbIn == mouseButton::middle) {
		return mButtonState;
	}
}

bool KeyStateLogger::wasMousePressed(mouseButton mbIn)
{
	if (mbIn == mouseButton::left) {
		return lButtonPressedLastFrame;
	}
	else if (mbIn == mouseButton::right) {
		return rButtonPressedLastFrame;
	}
	else if (mbIn == mouseButton::middle) {
		return mButtonPressedLastFrame;
	}
}

MouseMovePoint KeyStateLogger::getMouseClickPos(mouseButton mbIn)
{
	if (mbIn == mouseButton::left) {
		return MouseMovePoint(lButtonClickXLastFrame, lButtonClickYLastFrame);
	}
	else if (mbIn == mouseButton::right) {
		return MouseMovePoint(rButtonClickXLastFrame, rButtonClickYLastFrame);
	}
	else if (mbIn == mouseButton::middle) {
		return MouseMovePoint(mButtonClickXLastFrame, mButtonClickYLastFrame);
	}
}



void KeyStateLogger::notifyMouseMove(int xIn, int yIn)
{
	mouseXThisFrame = xIn;
	mouseYThisFrame = yIn;
}

MouseMovePoint KeyStateLogger::getMousePos()
{
	return MouseMovePoint(mouseXLastFrame, mouseYLastFrame);
}



void KeyStateLogger::newFrame()
{
	keysPressedLastFrame[0] = keysPressedThisFrame[0];
	keysPressedLastFrame[1] = keysPressedThisFrame[1];
	keysPressedLastFrame[2] = keysPressedThisFrame[2];
	keysPressedLastFrame[3] = keysPressedThisFrame[3];
	keysPressedLastFrame[4] = keysPressedThisFrame[4];
	keysPressedLastFrameCount = keysPressedThisFrameCount;
	keysPressedThisFrameCount = 0;

	lButtonPressedLastFrame = lButtonPressedThisFrame;
	mButtonPressedLastFrame = mButtonPressedThisFrame;
	rButtonPressedLastFrame = rButtonPressedThisFrame;
	lButtonPressedThisFrame = false;
	mButtonPressedThisFrame = false;
	rButtonPressedThisFrame = false;
	lButtonClickXLastFrame = lButtonClickXThisFrame;
	lButtonClickYLastFrame = lButtonClickYThisFrame;
	rButtonClickXLastFrame = rButtonClickXThisFrame;
	rButtonClickYLastFrame = rButtonClickYThisFrame;
	mButtonClickXLastFrame = mButtonClickXThisFrame;
	mButtonClickYLastFrame = mButtonClickYThisFrame;

	mouseXLastFrame = mouseXThisFrame;
	mouseYLastFrame = mouseYThisFrame;
}
