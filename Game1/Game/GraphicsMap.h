#pragma once
#include <map>

enum GraphicsEnum {
	None,
	PurpleArrow,
	PoorlyDrawnSmile,
	PenguinBackground,
	SpaceBg,
	Person1Walking1, Person1Walking2, Person1Walking3, Person1Walking4, Person1Walking5, Person1Walking6,
	Person1Idle1, Person1Idle2, Person1Idle3, Person1Idle4, Person1Idle5, Person1Idle6,
	Person1Special1, Person1Special2, Person1Special3, Person1Special4, Person1Special5, Person1Special6,
	BlackHole1, BlackHole2, BlackHole3, BlackHole4, BlackHole5, BlackHole6, BlackHole7, BlackHole8,
	Butterfly1, Butterfly2, Butterfly3, Butterfly4,
	EvilWizardIdle1, EvilWizardIdle2, EvilWizardIdle3, EvilWizardIdle4, EvilWizardIdle5, EvilWizardIdle6, EvilWizardIdle7, EvilWizardIdle8,
	EvilWizardRun1, EvilWizardRun2, EvilWizardRun3, EvilWizardRun4, EvilWizardRun5, EvilWizardRun6, EvilWizardRun7, EvilWizardRun8,
	EvilWizardJump1, EvilWizardJump2,
	EvilWizardFall1, EvilWizardFall2,
	EvilWizardAttackOne1, EvilWizardAttackOne2, EvilWizardAttackOne3, EvilWizardAttackOne4,
	EvilWizardAttackOne5, EvilWizardAttackOne6, EvilWizardAttackOne7, EvilWizardAttackOne8,
	BubbleEffect0, BubbleEffect1, BubbleEffect2, BubbleEffect3, BubbleEffect4, BubbleEffect5, BubbleEffect6, BubbleEffect7,
	BubbleEffect8, BubbleEffect9, BubbleEffect10, BubbleEffect11, BubbleEffect12, BubbleEffect13, BubbleEffect14, BubbleEffect15,
	BubbleEffect16, BubbleEffect17, BubbleEffect18, BubbleEffect19, BubbleEffect20, BubbleEffect21, BubbleEffect22, BubbleEffect23,
	BubbleEffect24, BubbleEffect25, BubbleEffect26, BubbleEffect27, BubbleEffect28, BubbleEffect29, BubbleEffect30, BubbleEffect31,
	BubbleEffect32, BubbleEffect33, BubbleEffect34, BubbleEffect35, BubbleEffect36, BubbleEffect37, BubbleEffect38, BubbleEffect39,
	BubbleEffect40, BubbleEffect41, BubbleEffect42, BubbleEffect43, BubbleEffect44, BubbleEffect45, BubbleEffect46, BubbleEffect47,
	BubbleEffect48, BubbleEffect49, BubbleEffect50, BubbleEffect51, BubbleEffect52, BubbleEffect53, BubbleEffect54, BubbleEffect55,
	BubbleEffect56, BubbleEffect57, BubbleEffect58, BubbleEffect59, BubbleEffect60,
	NiceGroundTopLeft, NiceGroundTopMid, NiceGroundTopRight,
	NiceGroundMidLeft, NiceGroundMidMid, NiceGroundMidRight,
	NiceGroundBottomLeft, NiceGroundBottomMid, NiceGroundBottomRight,
	Tree,
	TwentyMiscTile,
	CircleRune1,
	SquareRune1,
	CircleRune2,
	Bubble,
	GrimCathedral
};

class GraphicsMap
{
public:
	static std::map<GraphicsEnum, int> idMap;
};