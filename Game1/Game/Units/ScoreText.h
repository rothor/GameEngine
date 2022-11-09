#pragma once
#include "../VisualUnit.h"

class ScoreText : public VisualUnit
{
public:
	virtual std::list<DrawInstructions*>* getDrawInstructions() override;
};
