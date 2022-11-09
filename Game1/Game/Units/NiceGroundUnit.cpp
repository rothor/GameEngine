#include "NiceGroundUnit.h"

NiceGroundUnit::NiceGroundUnit(b2World* world, int unitId, float x, float y, float w, float h, float angle,
	float widthLeft, float widthMid, float widthRight, float heightTop, float heightMid, float heightBottom, float drawScale) :
	Unit(UnitType::NiceGround, world, unitId)
{
	FlagIsStatic = true;
	_center = b2Vec2(x, y);
	_angle = angle;
	_drawScale = drawScale;
	float newWidth;
	float newHeight;
	getDrawRects(_rectMap, newWidth, newHeight,
		_center.x, _center.y, w, h, _angle, widthLeft, widthMid, widthRight, heightTop, heightMid, heightBottom, _drawScale);
	_bodyId = Unit::createBody(b2BodyType::b2_staticBody, _center.x, _center.y, unitId);
	_fixtureId = Unit::getBody(_bodyId)->addBox(0, 0, newWidth, newHeight, angle,
		1.0f, 0.8f, 0.0f, 0x00FF00, drawScale, _angle, 1.0f, { { GraphicsEnum::None } }, false, 0.0f, 0.0f);
}

std::list<DrawInstructions*>* NiceGroundUnit::getDrawInstructions()
{
	if (_debugDraw) {
		return Unit::getDrawInstructions();
	}

	auto diList = new std::list<DrawInstructions*>{};
	const float DRAW_SCALE_MULT = 1.00f;
	{ // Top left
		Rectangle1& rect = *(_rectMap[std::wstring(L"topLeft")].begin());
		auto dii = new DrawInstructionsImage(DrawInstructionsPoint(rect.x, rect.y), _drawScale * DRAW_SCALE_MULT, -rect.a / b2_pi * 180,
			GraphicsMap::idMap[GraphicsEnum::NiceGroundTopLeft], 1.0f, false);
		diList->push_back(dii);
	}
	{ // Top right
		Rectangle1& rect = *(_rectMap[std::wstring(L"topRight")].begin());
		auto dii = new DrawInstructionsImage(DrawInstructionsPoint(rect.x, rect.y), _drawScale * DRAW_SCALE_MULT, -rect.a / b2_pi * 180,
			GraphicsMap::idMap[GraphicsEnum::NiceGroundTopRight], 1.0f, false);
		diList->push_back(dii);
	}
	{ // Bottom left
		Rectangle1& rect = *(_rectMap[std::wstring(L"bottomLeft")].begin());
		auto dii = new DrawInstructionsImage(DrawInstructionsPoint(rect.x, rect.y), _drawScale * DRAW_SCALE_MULT, -rect.a / b2_pi * 180,
			GraphicsMap::idMap[GraphicsEnum::NiceGroundBottomLeft], 1.0f, false);
		diList->push_back(dii);
	}
	{ // Bottom right
		Rectangle1& rect = *(_rectMap[std::wstring(L"bottomRight")].begin());
		auto dii = new DrawInstructionsImage(DrawInstructionsPoint(rect.x, rect.y), _drawScale * DRAW_SCALE_MULT, -rect.a / b2_pi * 180,
			GraphicsMap::idMap[GraphicsEnum::NiceGroundBottomRight], 1.0f, false);
		diList->push_back(dii);
	}
	{ // Mid left
		std::list<Rectangle1>& rectList = _rectMap[std::wstring(L"midLeft")];
		for (auto it = rectList.begin(); it != rectList.end(); it++) {
			Rectangle1& rect = *it;
			auto dii = new DrawInstructionsImage(DrawInstructionsPoint(rect.x, rect.y), _drawScale * DRAW_SCALE_MULT, -rect.a / b2_pi * 180,
				GraphicsMap::idMap[GraphicsEnum::NiceGroundMidLeft], 1.0f, false);
			diList->push_back(dii);
		}
	}
	{ // Mid right
		std::list<Rectangle1>& rectList = _rectMap[std::wstring(L"midRight")];
		for (auto it = rectList.begin(); it != rectList.end(); it++) {
			Rectangle1& rect = *it;
			auto dii = new DrawInstructionsImage(DrawInstructionsPoint(rect.x, rect.y), _drawScale * DRAW_SCALE_MULT, -rect.a / b2_pi * 180,
				GraphicsMap::idMap[GraphicsEnum::NiceGroundMidRight], 1.0f, false);
			diList->push_back(dii);
		}
	}
	{ // Top mid
		std::list<Rectangle1>& rectList = _rectMap[std::wstring(L"topMid")];
		for (auto it = rectList.begin(); it != rectList.end(); it++) {
			Rectangle1& rect = *it;
			auto dii = new DrawInstructionsImage(DrawInstructionsPoint(rect.x, rect.y), _drawScale * DRAW_SCALE_MULT, -rect.a / b2_pi * 180,
				GraphicsMap::idMap[GraphicsEnum::NiceGroundTopMid], 1.0f, false);
			diList->push_back(dii);
		}
	}
	{ // Bottom mid
		std::list<Rectangle1>& rectList = _rectMap[std::wstring(L"bottomMid")];
		for (auto it = rectList.begin(); it != rectList.end(); it++) {
			Rectangle1& rect = *it;
			auto dii = new DrawInstructionsImage(DrawInstructionsPoint(rect.x, rect.y), _drawScale * DRAW_SCALE_MULT, -rect.a / b2_pi * 180,
				GraphicsMap::idMap[GraphicsEnum::NiceGroundBottomMid], 1.0f, false);
			diList->push_back(dii);
		}
	}
	{ // Mid mid
		std::list<Rectangle1>& rectList = _rectMap[std::wstring(L"midMid")];
		for (auto it = rectList.begin(); it != rectList.end(); it++) {
			Rectangle1& rect = *it;
			auto dii = new DrawInstructionsImage(DrawInstructionsPoint(rect.x, rect.y), _drawScale * DRAW_SCALE_MULT, -rect.a / b2_pi * 180,
				GraphicsMap::idMap[GraphicsEnum::NiceGroundMidMid], 1.0f, false);
			diList->push_back(dii);
		}
	}

	return diList;
}

b2Vec2 getPointRotationTranslation(b2Vec2 pt, float angle)
{
	if (pt.x == 0 && pt.y == 0) {
		return b2Vec2(0, 0);
	}
	float angle1;
	if (pt.x != 0) {
		angle1 = fabsf(atanf(pt.y / pt.x));
	} else {
		if (pt.y > 0) {
			angle1 = b2_pi / 2;
		} else if (pt.y < 0) {
			angle1 = -b2_pi / 2;
		}
	}
	if (pt.x < 0) {
		if (pt.y > 0) {
			angle1 = b2_pi / 2.0f - angle1 + b2_pi / 2.0f;
		} else {
			angle1 += b2_pi;
		}
	} else if (pt.x > 0) {
		if (pt.y < 0) {
			angle1 = b2_pi / 2.0f - angle1 + 3.0f / 2.0f * b2_pi;
		}
	}

	float angle2 = angle1 + angle;
	float h = pt.Length();
	float y2 = h * sinf(angle2);
	float x2 = h * cosf(angle2);
	b2Vec2 translation = b2Vec2(x2 - pt.x, y2 - pt.y);
	return translation;
}

void NiceGroundUnit::getDrawRects(std::map<std::wstring, std::list<Rectangle1>>& rectMapOut, float& widthOut, float& heightOut,
	float x, float y, float w, float h, float angle,
	float widthLeft, float widthMid, float widthRight, float heightTop, float heightMid, float heightBottom, float drawScale)
{
	rectMapOut.clear();
	rectMapOut[std::wstring(L"topLeft")] = std::list<Rectangle1>();
	rectMapOut[std::wstring(L"topMid")] = std::list<Rectangle1>();
	rectMapOut[std::wstring(L"topRight")] = std::list<Rectangle1>();
	rectMapOut[std::wstring(L"midLeft")] = std::list<Rectangle1>();
	rectMapOut[std::wstring(L"midMid")] = std::list<Rectangle1>();
	rectMapOut[std::wstring(L"midRight")] = std::list<Rectangle1>();
	rectMapOut[std::wstring(L"bottomLeft")] = std::list<Rectangle1>();
	rectMapOut[std::wstring(L"bottomMid")] = std::list<Rectangle1>();
	rectMapOut[std::wstring(L"bottomRight")] = std::list<Rectangle1>();

	widthLeft *= drawScale;
	widthMid *= drawScale;
	widthRight *= drawScale;
	heightTop *= drawScale;
	heightMid *= drawScale;
	heightBottom *= drawScale;

	float widthMidTotal = w - widthLeft - widthRight;
	int numWidthMidSections;
	if (widthMidTotal <= 0) {
		numWidthMidSections = 0;
	} else {
		numWidthMidSections = ceilf(widthMidTotal / widthMid);
	}
	widthOut = widthLeft + widthRight + widthMid * numWidthMidSections;

	float heightMidTotal = h - heightTop - heightBottom;
	int numHeightMidSections;
	if (heightMidTotal <= 0) {
		numHeightMidSections = 0;
	} else {
		numHeightMidSections = ceilf(heightMidTotal / heightMid);
	}
	heightOut = heightTop + heightBottom + heightMid * numHeightMidSections;

	{ // Bottom left
		b2Vec2 rectCenter = b2Vec2(-widthOut / 2 + widthLeft / 2, -heightOut / 2 + heightBottom / 2);
		b2Vec2 translation = getPointRotationTranslation(rectCenter, angle);
		rectCenter.x += translation.x + x;
		rectCenter.y += translation.y + y;
		float rectWidth = widthLeft;
		float rectHeight = heightBottom;
		rectMapOut[std::wstring(L"bottomLeft")].push_back(Rectangle1(rectCenter.x, rectCenter.y, rectWidth, rectHeight, angle));
	}
	{ // Top left
		b2Vec2 rectCenter = b2Vec2(-widthOut / 2 + widthLeft / 2, heightOut / 2 - heightTop / 2);
		b2Vec2 translation = getPointRotationTranslation(rectCenter, angle);
		rectCenter.x += translation.x + x;
		rectCenter.y += translation.y + y;
		float rectWidth = widthLeft;
		float rectHeight = heightTop;
		rectMapOut[std::wstring(L"topLeft")].push_back(Rectangle1(rectCenter.x, rectCenter.y, rectWidth, rectHeight, angle));
	}
	{ // Top right
		b2Vec2 rectCenter = b2Vec2(widthOut / 2 - widthRight / 2, heightOut / 2 - heightTop / 2);
		b2Vec2 translation = getPointRotationTranslation(rectCenter, angle);
		rectCenter.x += translation.x + x;
		rectCenter.y += translation.y + y;
		float rectWidth = widthRight;
		float rectHeight = heightTop;
		rectMapOut[std::wstring(L"topRight")].push_back(Rectangle1(rectCenter.x, rectCenter.y, rectWidth, rectHeight, angle));
	}
	{ // Bottom right
		b2Vec2 rectCenter = b2Vec2(widthOut / 2 - widthRight / 2, -heightOut / 2 + heightBottom / 2);
		b2Vec2 translation = getPointRotationTranslation(rectCenter, angle);
		rectCenter.x += translation.x + x;
		rectCenter.y += translation.y + y;
		float rectWidth = widthLeft;
		float rectHeight = heightTop;
		rectMapOut[std::wstring(L"bottomRight")].push_back(Rectangle1(rectCenter.x, rectCenter.y, rectWidth, rectHeight, angle));
	}
	{ // Mid left
		b2Vec2 initRectCenter = b2Vec2(-widthOut / 2 + widthLeft / 2, -heightOut / 2 + heightBottom / 2); // bottom left rect center
		initRectCenter.y += heightBottom / 2 + heightMid / 2;
		for (int i = 0; i < numHeightMidSections; i++) {
			b2Vec2 rectCenter = b2Vec2(initRectCenter.x, initRectCenter.y + heightMid * i);
			b2Vec2 translation = getPointRotationTranslation(rectCenter, angle);
			rectCenter.x += translation.x + x;
			rectCenter.y += translation.y + y;
			float rectWidth = widthLeft;
			float rectHeight = heightTop;
			rectMapOut[std::wstring(L"midLeft")].push_back(Rectangle1(rectCenter.x, rectCenter.y, rectWidth, rectHeight, angle));
		}
	}
	{ // Mid right
		b2Vec2 initRectCenter = b2Vec2(widthOut / 2 - widthRight / 2, -heightOut / 2 + heightBottom / 2); // bottom right rect center
		initRectCenter.y += heightBottom / 2 + heightMid / 2;
		for (int i = 0; i < numHeightMidSections; i++) {
			b2Vec2 rectCenter = b2Vec2(initRectCenter.x, initRectCenter.y + heightMid * i);
			b2Vec2 translation = getPointRotationTranslation(rectCenter, angle);
			rectCenter.x += translation.x + x;
			rectCenter.y += translation.y + y;
			float rectWidth = widthLeft;
			float rectHeight = heightTop;
			rectMapOut[std::wstring(L"midRight")].push_back(Rectangle1(rectCenter.x, rectCenter.y, rectWidth, rectHeight, angle));
		}
	}
	{ // Top mid
		b2Vec2 initRectCenter = b2Vec2(-widthOut / 2 + widthLeft / 2, heightOut / 2 - heightTop / 2); // top left rect center
		initRectCenter.x += widthLeft / 2 + widthMid / 2;
		for (int i = 0; i < numWidthMidSections; i++) {
			b2Vec2 rectCenter = b2Vec2(initRectCenter.x + widthMid * i, initRectCenter.y);
			b2Vec2 translation = getPointRotationTranslation(rectCenter, angle);
			rectCenter.x += translation.x + x;
			rectCenter.y += translation.y + y;
			float rectWidth = widthLeft;
			float rectHeight = heightTop;
			rectMapOut[std::wstring(L"topMid")].push_back(Rectangle1(rectCenter.x, rectCenter.y, rectWidth, rectHeight, angle));
		}
	}
	{ // Bottom mid
		b2Vec2 initRectCenter = b2Vec2(-widthOut / 2 + widthLeft / 2, -heightOut / 2 + heightBottom / 2); // bottom left rect center
		initRectCenter.x += widthLeft / 2 + widthMid / 2;
		for (int i = 0; i < numWidthMidSections; i++) {
			b2Vec2 rectCenter = b2Vec2(initRectCenter.x + widthMid * i, initRectCenter.y);
			b2Vec2 translation = getPointRotationTranslation(rectCenter, angle);
			rectCenter.x += translation.x + x;
			rectCenter.y += translation.y + y;
			float rectWidth = widthLeft;
			float rectHeight = heightTop;
			rectMapOut[std::wstring(L"bottomMid")].push_back(Rectangle1(rectCenter.x, rectCenter.y, rectWidth, rectHeight, angle));
		}
	}
	{ // Mid mid
		b2Vec2 initRectCenter = b2Vec2(-widthOut / 2 + widthLeft / 2, -heightOut / 2 + heightBottom / 2); // bottom left rect center
		initRectCenter.x += widthLeft / 2 + widthMid / 2;
		initRectCenter.y += heightBottom / 2 + heightMid / 2;
		for (int i = 0; i < numWidthMidSections; i++) {
			for (int j = 0; j < numHeightMidSections; j++) {
				b2Vec2 rectCenter = b2Vec2(initRectCenter.x + widthMid * i, initRectCenter.y + heightMid * j);
				b2Vec2 translation = getPointRotationTranslation(rectCenter, angle);
				rectCenter.x += translation.x + x;
				rectCenter.y += translation.y + y;
				float rectWidth = widthLeft;
				float rectHeight = heightTop;
				rectMapOut[std::wstring(L"midMid")].push_back(Rectangle1(rectCenter.x, rectCenter.y, rectWidth, rectHeight, angle));
			}
		}
	}
}