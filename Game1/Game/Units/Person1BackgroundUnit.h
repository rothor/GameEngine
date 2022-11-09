#pragma once
#include "../VisualUnit.h"

class Person1BackgroundUnit : public VisualUnit
{
public:
	Person1BackgroundUnit(float x, float y, float scale);
	virtual std::vector<DrawInstructions*> getDrawInstructions() override;

protected:
	float _x;
	float _y;
	float _scale;
};