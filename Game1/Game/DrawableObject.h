#pragma once
#include "DrawInstructions.h"
#include <list>

class DrawableObject
{
public:
	virtual std::list<DrawInstructions*>* getDrawInstructions() = 0;
	int getZIndex();
	void setZIndex(int zIndex);

protected:
	int _zIndex = 0;
};
