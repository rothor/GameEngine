#pragma once

enum class mouseButton { left, middle, right };

struct MouseMovePoint
{
public:
	MouseMovePoint(int xIn, int yIn)
	{
		x = xIn;
		y = yIn;
	}

public:
	int x;
	int y;
};

class KeyStateLogger
{
public:
	KeyStateLogger();
	void notifyKeyPressed(int keyCodeIn);
	void notifyKeyReleased(int keyCodeIn);
	bool getKeyState(int keyCodeIn);
	bool wasKeyPressed(int keyCodeIn);

	void notifyMousePressed(mouseButton mbIn, int x, int y);
	void notifyMouseReleased(mouseButton mbIn, int x, int y);
	bool getMouseState(mouseButton mbIn);
	bool wasMousePressed(mouseButton mbIn);
	MouseMovePoint getMouseClickPos(mouseButton mbIn);

	void notifyMouseMove(int xIn, int yIn);
	MouseMovePoint getMousePos();

	void newFrame();

private:
	bool keyState[256]{ 0 };
	int keysPressedLastFrame[5]{ 0 };
	int keysPressedThisFrame[5]{ 0 };
	int keysPressedLastFrameCount{ 0 };
	int keysPressedThisFrameCount{ 0 };

	bool lButtonState{ false };
	bool rButtonState{ false };
	bool mButtonState{ false };
	bool lButtonPressedLastFrame{ false };
	bool rButtonPressedLastFrame{ false };
	bool mButtonPressedLastFrame{ false };
	bool lButtonPressedThisFrame{ false };
	bool rButtonPressedThisFrame{ false };
	bool mButtonPressedThisFrame{ false };
	int lButtonClickXLastFrame{ 0 };
	int lButtonClickYLastFrame{ 0 };
	int rButtonClickXLastFrame{ 0 };
	int rButtonClickYLastFrame{ 0 };
	int mButtonClickXLastFrame{ 0 };
	int mButtonClickYLastFrame{ 0 };
	int lButtonClickXThisFrame{ 0 };
	int lButtonClickYThisFrame{ 0 };
	int rButtonClickXThisFrame{ 0 };
	int rButtonClickYThisFrame{ 0 };
	int mButtonClickXThisFrame{ 0 };
	int mButtonClickYThisFrame{ 0 };

	int mouseXLastFrame{ 0 };
	int mouseYLastFrame{ 0 };
	int mouseXThisFrame{ 0 };
	int mouseYThisFrame{ 0 };
};