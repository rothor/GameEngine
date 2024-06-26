#include <Windows.h>
#include "GameEngine.h"
#include "Game/Game.h"
#include "Game/Game2.h"
#include "AudioEngine.h"

int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	HRESULT hr;
	hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
	if (FAILED(hr))
		return hr;

	// Create GameEngine instance
	GameEngine ge(hInstance);
	ge.createWindowDefault(700, 700, std::wstring(L"Game Window"));

	// Initialize audio engine
	AudioEngine audioEngine = AudioEngine();
	hr = audioEngine.Initialize();
	if (FAILED(hr))
		return hr;

	// Create Game instance
	Game game(ge.getPGraphicsInterface(), ge.getPKeyStateLogger(), &audioEngine);
	ge.setGameObj(&game);

	// Start
	int ret = ge.startGameLoop();

	return ret;
}