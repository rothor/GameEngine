#include "Person1BackgroundUnit.h"
#include "GraphicsInterface/DrawInstructionsImage.h"

Person1BackgroundUnit::Person1BackgroundUnit(float x, float y, float scale) :
	_x(x),
	_y(y),
	_scale(scale)
{
	std::vector<std::vector<GraphicsEnum>> animationVector = {
		{
			GraphicsEnum::Person1Idle1, GraphicsEnum::Person1Idle2, GraphicsEnum::Person1Idle3,
			GraphicsEnum::Person1Idle4, GraphicsEnum::Person1Idle5, GraphicsEnum::Person1Idle6
		}
	};
	setAnimations(animationVector);
	chooseAnimation(0);
}

std::vector<DrawInstructions*> Person1BackgroundUnit::getDrawInstructions()
{
	return std::vector<DrawInstructions*>{
		new DrawInstructionsImage(DrawInstructionsPoint(_x, _y), _scale, 0, GraphicsMap::idMap[getAnimationImage()], 1.0f, false)
	};
}
