#pragma once
#include <vector>
#include "box2d/box2d.h"

class RectangleMaker
{
public:
	static std::vector<b2Vec2>* getPointsFromRect(float x, float y, float w, float h, float angle);
	static std::vector<b2Vec2>* getPointsFromRectRotateLast(float x, float y, float w, float h, float translateX, float translateY, float angle);
	static std::vector<b2Vec2>* getPointsFromDraggableRect(b2Vec2 leftSideCenter, b2Vec2 rightSideCenter, float height);
	static void getDimensionsFromDraggableRect(b2Vec2 leftSideCenter, b2Vec2 rightSideCenter, b2Vec2& center, float& width, float& angle);
};

