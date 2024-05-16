#include "Game.h"
#include "Lib/RandomColorGetter.h"
#include "GraphicsMap.h"

Game::Game(GraphicsInterface* giIn, KeyStateLogger* kslIn, AudioEngine* audioEngine) :
	GameClass(giIn, kslIn, audioEngine),
	_world(giIn, audioEngine)
{
	loadLevel1();
}

int Game::tick(long long microsecondsElapsed)
{
	// Camera
	_graphics->cameraMove(_userInput->getKeyState(VK_LEFT), _userInput->getKeyState(VK_RIGHT), _userInput->getKeyState(VK_UP), _userInput->getKeyState(VK_DOWN));
	if (_userInput->wasKeyPressed(VK_HOME))
		_graphics->cameraZoomIn();
	if (_userInput->wasKeyPressed(VK_END))
		_graphics->cameraZoomOut();

	// Mouse pos
	auto mousePos = _userInput->getMousePos();
	_world.setMousePos(mousePos.x, mousePos.y);

	// Move player
	if (_playerUnitId != -1) {
		int left = _userInput->getKeyState('A');
		int right = _userInput->getKeyState('D');
		//int up = _userInput->getKeyState('W');
		//int down = _userInput->getKeyState('S');
		bool jump = _userInput->wasKeyPressed(VK_SPACE);
		Unit* unit = _world.getUnit(_playerUnitId);
		Player* player = static_cast<Player*>(unit);
		player->setMoveInput(left, right, jump);
		bool moonJump = _userInput->getKeyState('M');
		player->setMoonJumpSwitch(moonJump);
	}

	// Player attack
	if (_playerUnitId != -1) {
		if (_userInput->getKeyState('Q')) {
			Unit* unit = _world.getUnit(_playerUnitId);
			Player* player = static_cast<Player*>(unit);
			std::shared_ptr<CombatAttack> combatAttack(
				new CombatAttack(
				0.1f, 0.1f, UnitType::Bubble,
				b2Vec2(7.0f, -0.2f), b2Vec2(-0.2f, 132.0f)
			)
			);
			player->attack(combatAttack);
		}
		if (_userInput->getKeyState('B')) {
			Unit* unit = _world.getUnit(_playerUnitId);
			Player* player = static_cast<Player*>(unit);
			player->butterflyAttack();
		}
	}

	// Toggle debug draw
	if (_userInput->wasKeyPressed('L')) {
		_debugDraw = !_debugDraw;
		_world.setDebugDrawOn(_debugDraw);
	}
	if (_userInput->wasKeyPressed('K')) {
		_world.toggleDrawImages();
	}

	// Build wall
	if (_userInput->wasMousePressed(mouseButton::left)) {
		auto screenPos = _userInput->getMouseClickPos(mouseButton::left);
		auto worldPos = _graphics->convertScreenToWorldPoint(DrawInstructionsPoint(screenPos.x, screenPos.y));
		Unit* unit = _world.getUnit(_builderUnitId);
		WallBuilderUnit* wbUnit = static_cast<WallBuilderUnit*>(unit);
		if (!_builderModeSwitch) {
			wbUnit->setFirstPoint(b2Vec2(worldPos.x, worldPos.y));
		} else {
			wbUnit->createWall();
		}
		_builderModeSwitch = !_builderModeSwitch;
	}
	if (_builderModeSwitch) {
		auto screenPos = _userInput->getMousePos();
		auto worldPos = _graphics->convertScreenToWorldPoint(DrawInstructionsPoint(screenPos.x, screenPos.y));
		Unit* unit = _world.getUnit(_builderUnitId);
		WallBuilderUnit* wbUnit = static_cast<WallBuilderUnit*>(unit);
		wbUnit->setSecondPoint(b2Vec2(worldPos.x, worldPos.y));
	}

	// Load level
	if (_userInput->wasKeyPressed(VK_F1)) {
		loadLevel1();
	} else if (_userInput->wasKeyPressed(VK_F2)) {
		loadLevel2();
	}

	// Rotate color fliers
	_world.rotateColorFlyers();

	// Timers
	TimableObject::timeElapsed(microsecondsElapsed);

	_world.tick(microsecondsElapsed);
	return 0;
}

void Game::draw()
{
	// Move camera
	if (_playerUnitId != -1) {
		b2Vec2 pos = _world.getUnit(_playerUnitId)->getBody(0)->getPos();
		_graphics->cameraMoveTo(pos.x, pos.y + 10);
	}

	// Draw world
	_world.draw();
}

void Game::engineMessage(std::wstring msg)
{
	if (msg == L"reload graphics") {
		_world.reloadGraphics();
	}
}

void Game::doEvent(std::wstring event)
{
	if (event == L"level2summonbutterfly") {
		_world.addUnitColorFlyer(0, 12 + 13, 0xFF0000);
	}
}

void Game::loadLevel1()
{
	_world.CreateNewWorld();
	TimableObject::clearTimers();
	_playerUnitId = -1;
	_builderUnitId = -1;
	_world.SetGravity(0, 0);
	_builderUnitId = _world.addUnitWallBuilder();
	_world.addUnitBouncyCircle(0, 0, 0xFF0000, SoundEnum::PianoA6);
	_world.addUnitBouncyCircle(0.1F, 0.1F, 0x00FF00, SoundEnum::PianoB6);
	_playerUnitId = _world.addUnitPlayer(0, 1);
	_world.addUnitWall(-15, 0, 0.5, 30, 0, 0x00FF00, true);
	_world.addUnitWall(15, 0, 0.5, 30, 0, 0x00FF00, true);
	_world.addUnitWall(0, -15, 30, 0.5, 0, 0x00FF00, true);
	_world.addUnitWall(0, 15, 30, 0.5, 0, 0x00FF00, true);
	_world.addUnitTestJoint(0, 0, 1);
	_graphics->cameraMoveTo(0, 0);
	_graphics->cameraSetZoomNotch(0);
}

void Game::loadLevel2()
{
	_world.CreateNewWorld();
	TimableObject::clearTimers();
	_playerUnitId = -1;
	_builderUnitId = -1;
	_world.SetGravity(0, -9.81f);
	int timerId = TimableObject::addTimer(1.0f, L"level2summonbutterfly");

	_builderUnitId = _world.addUnitWallBuilder();
	_playerUnitId = _world.addUnitPlayer(0, 1);

	_world.addUnitStaticImageVisualUnit(0, 0, 0.015f, 1.0f, { { GraphicsEnum::SpaceBg } });
	_world.addUnitStaticImageVisualUnit(-7655.0f * 0.015f, 0, 0.015f, 1.0f, { { GraphicsEnum::SpaceBg } });
	_world.addUnitStaticImageVisualUnit(7655.0f * 0.015f, 0, 0.015f, 1.0f, { { GraphicsEnum::SpaceBg } });
	_world.addUnitStaticImageVisualUnit(-10, -7.8, 0.08f, 1.0f, { { GraphicsEnum::Tree } });
	//_world.addUnitStaticImageVisualUnit(0, 35, 0.05f, 1.0f, { { GraphicsEnum::GrimCathedral } });

	_world.addUnitNiceGround(0, -27, 200, 18, 0.0f);

	_world.addUnitKillZone(-50, 0, 1.0f, 100, 0, 0x0000FF);
	_world.addUnitKillZone(50, 0, 1.0f, 100, 0, 0x0000FF);
	_world.addUnitKillZone(0, 40, 100, 1.0f, 0, 0x0000FF);
	_world.addUnitKillZone(0, -20, 100, 1.0f, 0, 0x0000FF);

	/*_world.addUnitColorFlyer(2, 2, 0xFF0000);
	_world.addUnitColorFlyer(0, 2, 0xFF0000);
	_world.addUnitColorFlyer(-2, 2, 0xFF0000);
	_world.addUnitColorFlyer(-2, 4, 0xFF0000);
	_world.addUnitColorFlyer(2, 4, 0xFF0000);*/

	//_graphics->cameraMoveTo(0, 0);
	_graphics->cameraSetZoomNotch(-1);
}

void Game::loadLevel3()
{
	_world.CreateNewWorld();
	TimableObject::clearTimers();
	_playerUnitId = -1;
	_builderUnitId = -1;
	_world.SetGravity(0, -9.81f);
	
}
