#pragma once
#include "Unit.h"
#include "Lib/Rectangle1.h"
#include <map>
#include <list>

class NiceGroundUnit : public Unit
{
public:
	NiceGroundUnit(b2World* world, int unitId, float x, float y, float w, float h, float angle,
		float widthLeft, float widthMid, float widthRight, float heightTop, float heightMid, float heightBottom, float drawScale);
	virtual std::list<DrawInstructions*>* getDrawInstructions() override;

public:
	void static getDrawRects(std::map<std::wstring, std::list<Rectangle1>>& rectMapOut, float& widthOut, float& heightOut,
		float x, float y, float w, float h, float angle,
		float widthLeft, float widthMid, float widthRight, float heightTop, float heightMid, float heightBottom, float drawScale);

private:
	int _bodyId{ -1 };
	int _fixtureId{ -1 };
	float _width;
	float _height;
	b2Vec2 _center;
	float _angle;
	float _drawScale;
	std::map<std::wstring, std::list<Rectangle1>> _rectMap;
};