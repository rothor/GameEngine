#pragma once
#include "GameClass/GameClass.h"
#include "box2d/box2d.h"
#include "Units/Unit1.h"
#include "Units/Wall.h"
#include "Units/Player.h"
#include "Units/ColorFlyer.h"
#include "Units/CircleUnit.h"
#include "Units/TestJoint.h"
#include "Units/Square.h"
#include "Units/FlipperUnit.h"
#include <memory>
#include <map>


class Game2 : public GameClass
{
public:
	Game2(GraphicsInterface* giIn, KeyStateLogger* kslIn);
	/*int tick();
	void draw();

protected:
	void disposeAll();
	void loadLevel1();
	void tickLevel1();
	void BeginContact(b2Contact* contact);

	void CreateNewWorld(float x, float y);
	void SetGravity(float x, float y);
	void addUnitWall(float x, float y, float w, float h, int color);
	void addUnitWall(float x, float y, float w, float h, float angle, int color);
	void addUnitUnit1(float x, float y);
	void addUnitPlayer(float x, float y);
	void addUnitColorFlyer(float x, float y, int color);
	void addUnitCircleUnit(float x, float y, int color);
	void addUnitTestJoint(float x, float y, float motorSpeedInRadians);
	void addUnitSquare(float x, float y);
	void addUnitFlipperUnit(float x, float y, bool right);*/

protected:
	b2World* _world;
	Player* _player;
	b2ContactListener* _contactListener;
	int _level;
	bool _playerHightLineOn = false;
	bool _clearDrawing = true;
	std::map<int, Unit1> _unit1List{};
	std::map<int, Wall> _wallList{};
	std::map<int, ColorFlyer> _colorFlyerList{};
	std::map<int, CircleUnit> _circleUnitList{};
	std::map<int, TestJoint> _testJointList{};
	std::map<int, Square> _squareList{};
	std::map<int, FlipperUnit> _flipperUnitList{};
	int _unitId;
};
