#include "DrawInstructionsEllipse.h"

DrawInstructionsEllipse::DrawInstructionsEllipse(DrawInstructionsPoint pCenter, float pWidth, float pHeight, float pAngle, int pColor, float pAlpha) :
	DrawInstructions(DrawType::ellipse),
	center(pCenter),
	width(pWidth),
	height(pHeight),
	angle(pAngle),
	color(pColor),
	alpha(pAlpha)
{

}