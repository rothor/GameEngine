#pragma once
#include "box2d/box2d.h"
#include "DrawInstructions.h"
#include "Fixture.h"
#include "../UnitEnum.h"
#include "../SoundMap.h"
#include "../DrawableObject.h"
#include "../TimableObject.h"
#include "UnitBodyIdHolder.h"
#include "Lib/RectangleMaker.h"
#include <vector>
#include <string>
#include <list>
#include <memory>

class Body : public DrawableObject
{
public:
	Body();
	Body(b2Body* body);
	virtual std::list<DrawInstructions*>* getDrawInstructions() override;
	void push(bool left, bool right, bool up, bool down, float force);
	void placeAt(float x, float y);
	void regulate();
	b2Vec2 getPos();
	void push(float hori, float vert, float force);
	void spin(float hori, float vert, float force);
	void megaPush(float hori, float vert, float force);
	void testMegaPush();
	void setVelocity(float vX, float vY);
	void setHorizontalVelocity(float vX);
	void setVerticalVelocity(float vY);
	b2Vec2 getVelocity();
	virtual void timeStepForGraphics(int microseconds);
	Fixture* getFixture(int fixtureId);
	void setDebugDraw(bool turnOn);
	void destroy();
	void setFixedRotation(bool fixedRotation);
	b2Body* getBodyPtr();
	void disable();
	void enable();
	void setFixtureToDestroy(int fixtureId);
	void destroyFixturesSetToDestroy();
	void overrideGravity(b2Vec2 gravity);
	void resetGravityToNormal();
	bool getIsGravityOverridden();
	void applyOverriddenGravityToSelf();

	int addBox(float x, float y, float w, float h, float angle,
		float density = 1.0f, float friction = 0.8f, float restitution = 0.0f,
		int color = 0xFF0000, float animationScale = 1.0f, float animationAngle = 0.0f, float animationAlpha = 1.0f,
		std::vector<std::vector<GraphicsEnum>> animation = {}, bool isSensor = false,
		float animationAdjustX = 0.0f, float animationAdjustY = 0.0f);
	int addEllipse(float x, float y, float radius, float angle,
		float density = 1.0f, float friction = 0.3f, float restitution = 0.3f,
		int color = 0xFF0000, float animationScale = 1.0f, float animationAngle = 0.0f, float animationAlpha = 1.0f,
		std::vector<std::vector<GraphicsEnum>> animation = { {GraphicsEnum::None} }, bool isSensor = false);

protected:
	int getNewFixtureId();

protected:
	b2Body* _body;
	int _fixtureIdIncrementer = 0;
	std::map<int, Fixture> _fixtures;
	bool _debugDraw{ false };
	bool _gravityIsOverridden{ false };
	b2Vec2 _overriddenGravity;
};

