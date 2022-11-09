#include "RectangleMaker.h"

std::vector<b2Vec2>* RectangleMaker::getPointsFromRect(float x, float y, float w, float h, float angle)
{
	std::vector<b2Vec2> points{
		b2Vec2(x - w / 2, y - h / 2),
		b2Vec2(x - w / 2, y + h / 2),
		b2Vec2(x + w / 2, y + h / 2),
		b2Vec2(x + w / 2, y - h / 2)
	};
	std::vector<b2Vec2>* points2 = new std::vector<b2Vec2>{ };
	for (b2Vec2 point : points) {
		double a = x;
		double b = y;
		double c = point.x;
		double d = point.y;
		double m = sqrt(pow((a - c), 2) + pow((b - d), 2));
		double f = asin((d - b) / m);
		if (c < a)
			f = b2_pi - f;
		double t = angle;
		double g = f + t;
		double x = m * cos(g) + a;
		double y = m * sin(g) + b;
		points2->push_back(b2Vec2(x, y));
	}
	return points2;
}

std::vector<b2Vec2>* RectangleMaker::getPointsFromRectRotateLast(float x, float y, float w, float h, float translateX, float translateY, float angle)
{
	std::vector<b2Vec2> points{
		b2Vec2(-w / 2, -h / 2),
		b2Vec2(-w / 2, h / 2),
		b2Vec2(w / 2, h / 2),
		b2Vec2(w / 2, -h / 2)
	};
	for (auto& point : points) {
		point.x += translateX;
		point.y += translateY;
	}
	std::vector<b2Vec2>* points2 = new std::vector<b2Vec2>{ };
	for (b2Vec2 point : points) {
		double a = 0;
		double b = 0;
		double c = point.x;
		double d = point.y;
		double m = sqrt(pow((a - c), 2) + pow((b - d), 2));
		double f = asin((d - b) / m);
		if (c < a)
			f = b2_pi - f;
		double t = angle;
		double g = f + t;
		double x2 = m * cos(g) + a;
		double y2 = m * sin(g) + b;
		points2->push_back(b2Vec2(x2, y2));
	}
	for (auto& point : *points2) {
		point.x += x;
		point.y += y;
	}
	return points2;
}

std::vector<b2Vec2>* RectangleMaker::getPointsFromDraggableRect(b2Vec2 leftSideCenter, b2Vec2 rightSideCenter, float height)
{
	b2Vec2 center;
	float width;
	float angle;
	RectangleMaker::getDimensionsFromDraggableRect(leftSideCenter, rightSideCenter, center, width, angle);
	return RectangleMaker::getPointsFromRect(center.x, center.y, width, height, angle);
}

void RectangleMaker::getDimensionsFromDraggableRect(b2Vec2 leftSideCenter, b2Vec2 rightSideCenter, b2Vec2& center, float& width, float& angle)
{
	center.x = (leftSideCenter.x + rightSideCenter.x) / 2;
	center.y = (leftSideCenter.y + rightSideCenter.y) / 2;
	b2Vec2 diff;
	diff.x = rightSideCenter.x - leftSideCenter.x;
	diff.y = rightSideCenter.y - leftSideCenter.y;
	width = diff.Length();
	if (diff.x != 0) {
		angle = atanf(diff.y / diff.x);
	} else {
		if (diff.y > 0) {
			angle = b2_pi / 2;
		} else {
			angle = b2_pi * 3 / 2;
		}
	}
}
