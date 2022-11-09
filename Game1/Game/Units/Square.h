#pragma once
#include "Unit.h"

class Square : public Unit
{
public:
	Square(b2World* world, float x, float y, float w, float h, int color, int id);
	virtual void onContact(Unit* unit);
};

