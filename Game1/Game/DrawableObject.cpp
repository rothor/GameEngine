#include "DrawableObject.h"

int DrawableObject::getZIndex()
{
	return _zIndex;
}

void DrawableObject::setZIndex(int zIndex)
{
	_zIndex = zIndex;
}
