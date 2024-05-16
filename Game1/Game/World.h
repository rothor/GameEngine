#pragma once
#include <map>
#include "box2d/box2d.h"
//#include "Units/Unit1.h"
#include "Units/BouncyCircle.h"
#include "Units/Wall.h"
#include "Units/Player.h"
#include "Units/ColorFlyer.h"
//#include "Units/CircleUnit.h"
#include "Units/TestJoint.h"
//#include "Units/Square.h"
//#include "Units/FlipperUnit.h"
//#include "Units/ScoreText.h"
//#include "Units/Person1BackgroundUnit.h"
//#include "Units/BlackHoleUnit.h"
//#include "Units/ShotForLevel6.h"
//#include "Units/Level6BasketUnit.h"
//#include "Units/Level6SliderUnit.h"
#include "Units/BlackHoleShot.h"
#include "Units/StaticImageVisualUnit.h"
#include "Units/KillZone.h"
#include "Units/WallBuilderUnit.h"
#include "Units/NiceGroundUnit.h"
#include "Units/BubbleUnit.h"
#include "AudioEngine.h"
#include "DrawableObject.h"
#include "GameObject.h"
#include "PhysicsObject.h"
#include "GraphicsInterface.h"
#include "KeyStateLogger.h"
#include "SoundMap.h"
#include "Lib/RandomColorGetter.h"

class World : public b2ContactListener
{
public:
	World(GraphicsInterface* giIn, AudioEngine* audioEngine);
	int tick(long long microsecondsElapsed);
	void draw();
	void reloadGraphics();
	void BeginContact(b2Contact* contact);
	void EndContact(b2Contact* contact);
	void CreateNewWorld();
	void SetGravity(float x, float y);
	void createForceExplosion(float x, float y, float radius, float force);
	int getNewUnitId();
	void disposeAll();
	void setDebugStr(std::wstring newDebugStr);
	void moveUnit(int unitId, bool left, bool right, bool up, bool down);
	void setDebugDrawOn(bool debugDrawOn);
	void rotateColorFlyers();
	bool isUnitInArea(float left, float top, float right, float bottom);
	void setMousePos(int mouseScreenX, int mouseScreenY);
	Unit* getUnit(int unitId);
	void toggleDrawImages();

	int addUnitWall(float x, float y, float w, float h, float angle, int color, bool bouncy = false, bool visible = true);
	int addUnitKillZone(float x, float y, float w, float h, float angle, int color);
	int addUnitNiceGround(float x, float y, float w, float h, float angle);
	//void addUnitUnit1(float x, float y, SoundEnum sound);
	void addUnitBouncyCircle(float x, float y, int color, SoundEnum sound);
	void addUnitBouncyCirclePlayer(float x, float y, int color);
	int addUnitPlayer(float x, float y);
	int addUnitColorFlyer(float x, float y, int color);
	int addUnitBubble(float x, float y);
	int addUnitBlackHoleShot(float x, float y);
	//void addUnitCircleUnit(float x, float y, int color);
	void addUnitTestJoint(float x, float y, float motorSpeedInRadians);
	int addUnitWallBuilder();
	//int addUnitSquare(float x, float y);
	//void addUnitFlipperUnit(float x, float y, bool right);
	//void addUnitBlackHoleUnit(float x, float y, SoundEnum sound);
	//int addUnitShotForLevel6(float x, float y);
	//int addUnitLevel6BasketUnit(float x, float y);
	//int addUnitLevel6SliderUnit(float x, float y, std::wstring title);
	int addUnitStaticImageVisualUnit(float x, float y, float scale, float alpha, std::vector<std::vector<GraphicsEnum>> animationSet);

protected:
	void loadImage(std::wstring file, GraphicsEnum graphic);
	void loadImageGroup(std::wstring folder, GraphicsEnum startingImageEnum, int finalImageNum);

protected:
	GraphicsInterface* _graphics;
	AudioEngine* _audioEngine;
	b2World* _world;
	std::map<int, Unit*> _unitList{};
	std::map<int, VisualUnit*> _visualUnitList{};
	bool _globalDebugDraw{ false };
	bool _globalDrawImages{ true };
	std::wstring _debugStr;
	int _unitIdIncrementer{ 0 };
	int _mouseScreenX;
	int _mouseScreenY;
	b2Vec2 _mouseWorldPos;
};

