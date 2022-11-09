#include "DrawInstructionsText.h"

DrawInstructionsText::DrawInstructionsText(DrawInstructionsPoint pCenter, std::wstring pText, std::wstring pFont,
	float pSize, float pAngle, int pColor, float pAlpha) :
	DrawInstructions(DrawInstructions::DrawType::text),
	center(pCenter),
	size(pSize),
	angle(pAngle),
	color(pColor),
	alpha(pAlpha),
	text(pText),
	font(pFont)
{

}
