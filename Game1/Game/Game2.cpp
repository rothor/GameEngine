#include "Game2.h"
#include <random>
#include <functional>

int GetRandomColor();

Game2::Game2(GraphicsInterface* giIn, KeyStateLogger* kslIn) :
	GameClass(giIn, kslIn, 0),
	_unitId(0)
{
	_graphics->createBmpFromFile(L"arrow.png");
	_graphics->createBmpFromFile(L"smile.png");
	//loadLevel1();
}
/*
int Game2::tick()
{
	if (_userInput->wasKeyPressed(VK_F1))
		loadLevel1();
	else if (_userInput->wasKeyPressed(VK_F2))
		loadLevel2();
	else if (_userInput->wasKeyPressed(VK_F3))
		loadLevel3();
	else if (_userInput->wasKeyPressed(VK_F4))
		loadLevel4();

	if (_level == 1)
		tickLevel1();
	else if (_level == 2)
		tickLevel2();
	else if (_level == 3)
		tickLevel3();
	else if (_level == 4)
		tickLevel4();

	// World step
	_world->Step(1.0f / 60.0f, 16, 6);
	// Camera
	_graphics->cameraMove(_userInput->getKeyState(VK_LEFT), _userInput->getKeyState(VK_RIGHT), _userInput->getKeyState(VK_UP), _userInput->getKeyState(VK_DOWN));
	if (_userInput->wasKeyPressed(VK_HOME))
		_graphics->cameraZoomIn();
	if (_userInput->wasKeyPressed(VK_END))
		_graphics->cameraZoomOut();
	//b2Vec2 playerPos = _player->getPos();
	//_graphics->cameraMoveTo(playerPos.x, playerPos.y);

	return 0;
}

void Game2::draw()
{
	if (_clearDrawing)
		_graphics->fillScreen(0x000000);

	if (_playerHightLineOn && _player) {
		auto pos = _player->getPos();
		_graphics->drawRect(0, pos.y, 500, 1, 0x00DEEF, 0.25f);
	}

	for (auto unit : _wallList) {
		auto diArray = unit.second.getDrawInstructions();
		for (auto di : diArray) {
			_graphics->draw(di);
		}
	}

	for (auto unit : _unit1List) {
		auto diArray = unit.second.getDrawInstructions();
		for (auto di : diArray) {
			_graphics->draw(di);
		}
	}

	for (auto unit : _colorFlyerList) {
		auto diArray = unit.second.getDrawInstructions();
		for (auto di : diArray) {
			_graphics->draw(di);
		}
	}

	for (auto unit : _circleUnitList) {
		auto diArray = unit.second.getDrawInstructions();
		for (auto di : diArray) {
			_graphics->draw(di);
		}
	}

	for (auto unit : _testJointList) {
		auto diArray = unit.second.getDrawInstructions();
		for (auto di : diArray) {
			_graphics->draw(di);
		}
	}

	for (auto unit : _squareList) {
		auto diArray = unit.second.getDrawInstructions();
		for (auto di : diArray) {
			_graphics->draw(di);
		}
	}

	for (auto unit : _flipperUnitList) {
		auto diArray = unit.second.getDrawInstructions();
		for (auto di : diArray) {
			_graphics->draw(di);
		}
	}

	if (_player) {
		auto diArray = _player->getDrawInstructions();
		for (auto di : diArray) {
			_graphics->draw(di);
		}
	}
}



void Game2::disposeAll()
{
	if (_world != 0) {
		delete _world;
		_world = 0;
	}
	_player = 0;
	_wallList.clear();
	_unit1List.clear();
	_colorFlyerList.clear();
	_circleUnitList.clear();
	_testJointList.clear();
	_squareList.clear();
	_flipperUnitList.clear();
}

void Game2::loadLevel1()
{
	_level = 1;
	CreateNewWorld(0.0f, 0.0f);
	_playerHightLineOn = false;

	//b2ContactListener* listener;
	//_world->SetContactListener(listener);


	// Player
	addUnitPlayer(0.0f, 5.0f);

	// Walls
	addUnitWall(0.0f, 0.0f, 20.0f, 0.25f, 0x22FF88);
	addUnitWall(0.0f, 20.0f, 20.0f, 0.25f, 0x22FF88);
	addUnitWall(-10.0f, 10.0f, 0.25f, 20.0f, 0x22FF88);
	addUnitWall(10.0f, 10.0f, 0.25f, 20.0f, 0x22FF88);

	// Other units
	addUnitUnit1(4.0f, 4.0f);
	for (int i = 0; i < 1; i++) {
		addUnitUnit1(-2.0f + i / 20.0f, 2.0f);
	}

	_graphics->cameraMoveTo(0.0f, 5.0f);
}

void Game2::tickLevel1()
{
	if (_userInput->wasKeyPressed('Q'))
		_clearDrawing = !_clearDrawing;
	if (_player) {
		if (_userInput->wasKeyPressed('R'))
			_player->placeAt(0, 2);
		_player->push(_userInput->getKeyState('A'), _userInput->getKeyState('D'), _userInput->getKeyState('W'), _userInput->getKeyState('S'), 20.0f);
		//_player->regulate();
	}

	if (_userInput->wasMousePressed(mouseButton::left)) {
		auto pt = _userInput->getMouseClickPos(mouseButton::left);
		auto pt2 = _graphics->convertScreenToWorldPoint(DrawInstructionsPoint(pt.x, pt.y));
		addUnitCircleUnit(pt2.x, pt2.y, 0xFF0000);
	}
	if (_userInput->wasMousePressed(mouseButton::right)) {
		auto pt = _userInput->getMouseClickPos(mouseButton::right);
		auto pt2 = _graphics->convertScreenToWorldPoint(DrawInstructionsPoint(pt.x, pt.y));
	}

	for (auto unit : _unit1List)
		;//unit.regulate();
	for (auto unit : _circleUnitList)
		;// unit.regulate();
}

void Game2::loadLevel2()
{
	_level = 2;
	CreateNewWorld(0.0f, -10.0f);
	_playerHightLineOn = true;

	_graphics->cameraZoomOut();
	_graphics->cameraMoveTo(0, 10000);

	// Player
	addUnitPlayer(0.0f, 3.0f);

	// Walls
	addUnitWall(0.0f, 0.0f, 20.0f, 0.25f, 0x22FF88);
	addUnitWall(-10.0f, 500.0f, 0.25f, 1000.0f, 0x22FF88);
	addUnitWall(10.0f, 500.0f, 0.25f, 1000.0f, 0x22FF88);

	// Other units
	addUnitUnit1(4.0f, 4.0f);
	addUnitUnit1(-2.0f, 2.0f);

	_graphics->cameraMoveTo(0.0f, 5.0f);
}

void Game2::tickLevel2()
{
	tickLevel1();
}

void Game2::loadLevel3()
{
	_level = 3;
	CreateNewWorld(0.0f, -5.0f);
	_playerHightLineOn = false;

	// Player
	addUnitPlayer(0.0f, 3.0f);

	// Walls
	addUnitWall(0.0f, 0.0f, 20.0f, 0.25f, 0x22FF88);
	addUnitWall(-10.0f, 500.0f, 0.25f, 1000.0f, 0x22FF88);
	addUnitWall(10.0f, 500.0f, 0.25f, 1000.0f, 0x22FF88);

	// Other units
	addUnitColorFlyer(-5.0f, 2.0f, 0xFF0000);
	addUnitColorFlyer(-5.0f, 4.0f, 0xFFA500);
	addUnitColorFlyer(-5.0f, 6.0f, 0xFFFF00);
	addUnitColorFlyer(5.0f, 2.0f, 0x00FF00);
	addUnitColorFlyer(5.0f, 4.0f, 0x00DEEF);
	addUnitColorFlyer(5.0f, 6.0f, 0x800080);

	_graphics->cameraSetZoomFactor(8);
	_graphics->cameraMoveTo(0.0f, 21.0f);
}

void Game2::tickLevel3()
{
	tickLevel1();
}

void Game2::BeginContact(b2Contact* contact)
{
	b2Fixture* fixtureA = contact->GetFixtureA();
	b2Fixture* fixtureB = contact->GetFixtureB();
}

class Game2ContactListener : public b2ContactListener
{
public:
	Game2ContactListener(std::function<void(b2Contact*)> beginContactCallback)
	{
		BeginContactCallback = beginContactCallback;
	}

	void BeginContact(b2Contact* contact)
	{
		BeginContactCallback(contact);
	}

	std::function<void(b2Contact*)> BeginContactCallback;
};

void Game2::loadLevel4()
{
	_level = 4;
	CreateNewWorld(0.0f, -5.0f);
	_playerHightLineOn = false;
	_contactListener = new Game2ContactListener([this](b2Contact* c) { BeginContact(c); });
	_world->SetContactListener(_contactListener);

	// Player
	//addUnitPlayer(0.0f, 3.0f);

	// Walls
	//addUnitWall(0.0f, 0.0f, 20.0f, 0.25f, 0, 0x22FF88);
	//addUnitWall(-10.0f, 500.0f, 0.25f, 1000.0f, 0.5f, 0x22FF88);
	//addUnitWall(10.0f, 500.0f, 0.25f, 1000.0f, 0.5f, 0x22FF88);

	// Other units
	//addUnitColorFlyer(-5.0f, 2.0f, 0xFF0000);
	//addUnitColorFlyer(-5.0f, 4.0f, 0xFFA500);
	//addUnitColorFlyer(-5.0f, 6.0f, 0xFFFF00);
	//addUnitColorFlyer(5.0f, 2.0f, 0x00FF00);
	//addUnitColorFlyer(5.0f, 4.0f, 0x00DEEF);
	//addUnitColorFlyer(5.0f, 6.0f, 0x800080);
	addUnitTestJoint(2.7, 1, 1);
	addUnitTestJoint(-2.7, 1, -1);
	addUnitTestJoint(0, -4, 0);
	addUnitTestJoint(5, -9, -1);
	addUnitTestJoint(-5, -9, 1);

	_graphics->cameraSetZoomFactor(12);
	_graphics->cameraMoveTo(0.0f, 1.0f);
}

void Game2::tickLevel4()
{
	static int counter = 0;
	if (counter++ % 10 == 0) {
		addUnitSquare(0, 25);
	}

	if (_userInput->getKeyState('Z')) {
		for (auto flipper : _flipperUnitList)
			if (!flipper.second._isRight)
				flipper.second.turnOn();
	}
	else {
		for (auto flipper : _flipperUnitList)
			if (!flipper.second._isRight)
				flipper.second.turnOff();
	}

	if (_userInput->getKeyState(191)) {
		for (auto flipper : _flipperUnitList)
			if (flipper.second._isRight)
				flipper.second.turnOn();
	}
	else {
		for (auto flipper : _flipperUnitList)
			if (flipper.second._isRight)
				flipper.second.turnOff();
	}

	if (_userInput->wasMousePressed(mouseButton::left)) {
		auto pt = _userInput->getMouseClickPos(mouseButton::left);
		auto pt2 = _graphics->convertScreenToWorldPoint(DrawInstructionsPoint(pt.x, pt.y));
		addUnitFlipperUnit(pt2.x, pt2.y, false);
	}
	if (_userInput->wasMousePressed(mouseButton::right)) {
		auto pt = _userInput->getMouseClickPos(mouseButton::right);
		auto pt2 = _graphics->convertScreenToWorldPoint(DrawInstructionsPoint(pt.x, pt.y));
		addUnitFlipperUnit(pt2.x, pt2.y, true);
	}

	for (auto iter = _squareList.begin(); iter != _squareList.end(); iter++) {
		auto pos = (*iter).second.getPos();
		if (pos.x < -200 || pos.x > 200 || pos.y < -200 || pos.y > 200) {
			(*iter).second.destroy();
			iter = _squareList.erase(iter);
		}
		if (iter == _squareList.end())
			break;
	}
}

void Game2::CreateNewWorld(float x, float y)
{
	disposeAll();
	_world = new b2World(b2Vec2(x, y));
}

void Game2::SetGravity(float x, float y)
{
	b2Vec2 gravity(0.0f, -5.0f);
	_world->SetGravity(gravity);
}

void Game2::addUnitWall(float x, float y, float w, float h, int color)
{
	auto unit = Wall(&*_world, x, y, w, h, 0, color, _unitId);
	_wallList.insert(std::pair<int, Wall>(_unitId++, unit));
}

void Game2::addUnitWall(float x, float y, float w, float h, float angle, int color)
{
	auto unit = Wall(&*_world, x, y, w, h, angle, color, _unitId);
	_wallList.insert(std::pair<int, Wall>(_unitId++, unit));
}

void Game2::addUnitUnit1(float x, float y)
{
	auto unit = Unit1(&*_world, x, y, _unitId);
	_unit1List.insert(std::pair<int, Unit1>(_unitId++, unit));
}

void Game2::addUnitPlayer(float x, float y)
{
	_player = new Player(&*_world, x, y, -1);
}

void Game2::addUnitColorFlyer(float x, float y, int color)
{
	auto unit = ColorFlyer(&*_world, x, y, color, _unitId);
	_colorFlyerList.insert(std::pair<int, ColorFlyer>(_unitId++, unit));
}

void Game2::addUnitCircleUnit(float x, float y, int color)
{
	auto unit = CircleUnit(&*_world, x, y, color, _unitId);
	_circleUnitList.insert(std::pair<int, CircleUnit>(_unitId++, unit));
}

void Game2::addUnitTestJoint(float x, float y, float motorSpeedInRadians)
{
	auto unit = TestJoint(&*_world, x, y, motorSpeedInRadians, _unitId);
	_testJointList.insert(std::pair<int, TestJoint>(_unitId++, unit));
}

void Game2::addUnitSquare(float x, float y)
{
	auto unit = Square(&*_world, x, y, 1, 1, GetRandomColor(), _unitId);
	_squareList.insert(std::pair<int, Square>(_unitId++, unit));
}

void Game2::addUnitFlipperUnit(float x, float y, bool right)
{
	auto unit = FlipperUnit(&*_world, x, y, right, _unitId);
	_flipperUnitList.insert(std::pair<int, FlipperUnit>(_unitId++, unit));
}

static std::default_random_engine generator;
static std::uniform_int_distribution<int> distribution(0, 255);
static auto shade = std::bind(distribution, generator);
static int GetRandomColor()
{
	return shade() * 0x10000 + shade() * 0x100 + shade();
}*/