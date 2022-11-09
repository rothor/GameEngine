#include "DrawInstructionsPolygon.h"

DrawInstructionsPolygon::DrawInstructionsPolygon(std::vector<DrawInstructionsPoint>* pVertexes, int pColor, float pAlpha) :
	DrawInstructions(DrawType::polygon),
	color(pColor),
	alpha(pAlpha)
{
	vertexes = std::unique_ptr<std::vector<DrawInstructionsPoint>>(pVertexes);
}
