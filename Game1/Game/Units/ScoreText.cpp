#include "ScoreText.h"
#include "GraphicsInterface/DrawInstructionsImage.h"
#include "../GraphicsMap.h"

std::list<DrawInstructions*>* ScoreText::getDrawInstructions()
{
	return new std::list<DrawInstructions*>{
		new DrawInstructionsImage(DrawInstructionsPoint(0, 10), 0.02, 0, GraphicsMap::idMap[GraphicsEnum::PenguinBackground], 1.0f, false)
	};
}
