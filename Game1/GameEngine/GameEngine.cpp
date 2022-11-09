//#include "Resource.h"
#include "GameEngine.h"
#include "BasicWindowMaker/BasicWindowMaker.h"
#include "KeyStateLogger.h"
#include "HighResolutionTimer/HighResolutionTimer.h"
#include <windowsx.h> // for get_x_param and get_y_param macros


GameEngine::GameEngine(HINSTANCE hInstIn) :
	hInst(hInstIn),
	stopThread(false)
{
	// Initialize values
	//framesPerSec = 120;
	ticksPerSec = 60;
}

GameEngine::~GameEngine()
{
	
}

GraphicsInterface* GameEngine::getPGraphicsInterface()
{
	return gi.get();
}

KeyStateLogger* GameEngine::getPKeyStateLogger()
{
	return &ksl;
}

bool GameEngine::createWindowDefault(int width, int height, std::wstring title)
{
	// Create window
	int exWindowStyles = 0;
	int windowStyles = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_SIZEBOX | WS_CLIPCHILDREN | WS_MAXIMIZEBOX;
	int classStyles = 0;

	RECT rect;
	rect.left = 0;
	rect.top = 0;
	rect.right = width;
	rect.bottom = height;
	AdjustWindowRectEx(&rect, windowStyles, FALSE, exWindowStyles);

	gameHwnd = BasicWindowMaker::createWindow(
		title.c_str(), L"defaultGameEngineWindowClass",
		0, 0, rect.right - rect.left, rect.bottom - rect.top,
		WndProc, hInst, SW_SHOW, exWindowStyles, windowStyles, classStyles,
		0, 0, 0
	);

	// Initialization that must be done after creating the window
	SetWindowLongPtr(gameHwnd, GWLP_USERDATA, (LONG_PTR)this);
	gi = std::unique_ptr<GraphicsInterface>(
		new GraphicsInterface(gameHwnd, width, height)
	);

	return true;
}

void GameEngine::setGameObj(GameClass* objIn)
{
	gameObj = objIn;
}

int GameEngine::startGameLoop()
{
	// Start game loop!
	gameThread = std::thread(game_loop, (LPVOID)this);

	// Start application message loop
	HACCEL hAccelTable = LoadAcceleratorsW(hInst, MAKEINTRESOURCE(109));
	MSG msg;
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}

void GameEngine::endGameLoop()
{
	if (gameThread.joinable()) {
		// Tell the thread to stop, and wait for it to join
		stopThread = true;
		gameThread.join();
	}
}

void GameEngine::game_loop(LPVOID ptr)
{
	GameEngine* pGameEngine = (GameEngine*)ptr;
	HighResolutionTimer timer;
	long long microsPerGameTick = round((double)1000 * 1000 / pGameEngine->ticksPerSec);
	long long microCounter = microsPerGameTick; // Start with this value for the first frame.
	timer.reset();

	while (true) {
		long long timerRead = timer.readMicro();
		timer.reset();
		//OutputDebugStringW(std::to_wstring(timerRead).append(L"\n").c_str());
		microCounter += timerRead;
		microCounter -= microsPerGameTick;

		pGameEngine->gi->copyBuffer();
		pGameEngine->gameObj->tick(microsPerGameTick);
		pGameEngine->ksl.newFrame();
		if (pGameEngine->gi->windowSizeChanged()) {
			pGameEngine->gi->changeWindowSize();
			pGameEngine->gameObj->engineMessage(L"reload graphics");
		}
		pGameEngine->gameObj->draw();

		if (microCounter < 0) {
			long long millisToSleep = round((-microCounter) / 1000.0f);
			Sleep(millisToSleep);
			//while (timer.readMicro() < microsPerGameTick) {} // This line is more accurate than the above line, but uses much more CPU.
		}
		
		if (pGameEngine->stopThread)
			break;
	}
}

LRESULT CALLBACK GameEngine::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	GameEngine* pGameEngine = (GameEngine*)GetWindowLongPtr(hWnd, GWLP_USERDATA);

	switch (message)
	{
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_ERASEBKGND:
		break;
	case WM_KEYDOWN:
		//OutputDebugStringW((std::to_wstring(wParam) + L"-").c_str());
		pGameEngine->ksl.notifyKeyPressed(wParam);
		break;
	case WM_KEYUP:
		pGameEngine->ksl.notifyKeyReleased(wParam);
		break;
	case WM_LBUTTONDOWN:
		pGameEngine->ksl.notifyMousePressed(mouseButton::left, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		break;
	case WM_LBUTTONUP:
		pGameEngine->ksl.notifyMouseReleased(mouseButton::left, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		break;
	case WM_RBUTTONDOWN:
		pGameEngine->ksl.notifyMousePressed(mouseButton::right, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		break;
	case WM_RBUTTONUP:
		pGameEngine->ksl.notifyMouseReleased(mouseButton::right, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		break;
	case WM_MBUTTONDOWN:
		pGameEngine->ksl.notifyMousePressed(mouseButton::middle, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		break;
	case WM_MBUTTONUP:
		pGameEngine->ksl.notifyMouseReleased(mouseButton::middle, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		break;
	case WM_MOUSEMOVE:
		pGameEngine->ksl.notifyMouseMove(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		break;
	case WM_MOUSEWHEEL:
		// to implement
		break;
	case WM_SIZE:
		if (pGameEngine != 0 && pGameEngine->gi.get() != 0)
			pGameEngine->gi->sizeChange(LOWORD(lParam), HIWORD(lParam));
		break;
	case WM_DESTROY:
		pGameEngine->endGameLoop();
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
