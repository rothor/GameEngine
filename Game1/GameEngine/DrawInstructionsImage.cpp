#include "DrawInstructionsImage.h"

DrawInstructionsImage::DrawInstructionsImage(DrawInstructionsPoint pCenter, float pScale, float pAngle, int pResourceId, float pAlpha,
	bool pFlipHorizontally) :
	DrawInstructions(DrawType::image),
	center(pCenter),
	scale(pScale),
	angle(pAngle),
	resourceId(pResourceId),
	alpha(pAlpha),
	flipHorizontally(pFlipHorizontally)
{

}