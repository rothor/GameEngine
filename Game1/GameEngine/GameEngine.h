#pragma once
#define WIN32_LEAN_AND_MEAN
#include <memory>
#include <thread>
#include <Windows.h>
#include <string>
#include "GraphicsInterface.h"
#include "KeyStateLogger.h"
#include "GameClass/GameClass.h"


class GameEngine
{
public:
	GameEngine(HINSTANCE);
	~GameEngine();

	GraphicsInterface* getPGraphicsInterface();
	KeyStateLogger* getPKeyStateLogger();
	bool createWindowDefault(int width, int height, std::wstring title);
	void setGameObj(GameClass* objIn);
	int startGameLoop();
	void endGameLoop();
	
private:
	GameClass* gameObj;
	int ticksPerSec;
	int framesPerSec;
	std::unique_ptr<GraphicsInterface> gi;
	HWND gameHwnd;
	std::thread gameThread;
	bool stopThread;
	HINSTANCE hInst;
	KeyStateLogger ksl;

private:
	static void game_loop(LPVOID ptr);
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};
