#include "StaticImageVisualUnit.h"
#include "DrawInstructionsInclude.h"

StaticImageVisualUnit::StaticImageVisualUnit(float x, float y, float scale, float alpha, std::vector<std::vector<GraphicsEnum>> animationVector)
{
	_x = x;
	_y = y;
	_scale = scale;
	_alpha = alpha;
	setAnimations(animationVector);
	chooseAnimation(0);
}

std::list<DrawInstructions*>* StaticImageVisualUnit::getDrawInstructions()
{
	return new std::list<DrawInstructions*>{
		new DrawInstructionsImage(DrawInstructionsPoint(_x, _y), _scale, 0, GraphicsMap::idMap[getAnimationImage()], _alpha, false)
	};
}
