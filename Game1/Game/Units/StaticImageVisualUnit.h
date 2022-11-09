#pragma once
#include "../VisualUnit.h"

class StaticImageVisualUnit : public VisualUnit
{
public:
	StaticImageVisualUnit(float x, float y, float scale, float alpha, std::vector<std::vector<GraphicsEnum>> animationVector);
	virtual std::list<DrawInstructions*>* getDrawInstructions() override;

protected:
	float _x;
	float _y;
	float _scale;
	float _alpha;
};

