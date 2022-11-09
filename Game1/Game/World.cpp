#include "World.h"

World::World(GraphicsInterface* giIn, AudioEngine* audioEngine)
{
	_graphics = giIn;
	_audioEngine = audioEngine;

	reloadGraphics();

	// Load audio
	int soundId;
	// Piano keys
	_audioEngine->LoadSound(L"res/audio/gameover.wav", soundId);
	SoundMap::idMap[SoundEnum::GameOver] = soundId;
	_audioEngine->LoadSound(L"res/audio/piano c6.wav", soundId);
	SoundMap::idMap[SoundEnum::PianoC6] = soundId;
	_audioEngine->LoadSound(L"res/audio/piano d6.wav", soundId);
	SoundMap::idMap[SoundEnum::PianoD6] = soundId;
	_audioEngine->LoadSound(L"res/audio/piano e6.wav", soundId);
	SoundMap::idMap[SoundEnum::PianoE6] = soundId;
	_audioEngine->LoadSound(L"res/audio/piano f6.wav", soundId);
	SoundMap::idMap[SoundEnum::PianoF6] = soundId;
	_audioEngine->LoadSound(L"res/audio/piano g6.wav", soundId);
	SoundMap::idMap[SoundEnum::PianoG6] = soundId;
	_audioEngine->LoadSound(L"res/audio/piano a6.wav", soundId);
	SoundMap::idMap[SoundEnum::PianoA6] = soundId;
	_audioEngine->LoadSound(L"res/audio/piano b6.wav", soundId);
	SoundMap::idMap[SoundEnum::PianoB6] = soundId;
	_audioEngine->LoadSound(L"res/audio/piano c7.wav", soundId);
	SoundMap::idMap[SoundEnum::PianoC7] = soundId;
	// Drug war
	_audioEngine->LoadSound(L"res/audio/drugwar-beat.wav", soundId, true);
	SoundMap::idMap[SoundEnum::DrugwarBeat] = soundId;
	_audioEngine->LoadSound(L"res/audio/drugwar-organ.wav", soundId, true);
	SoundMap::idMap[SoundEnum::DrugwarOrgan] = soundId;
	_audioEngine->LoadSound(L"res/audio/drugwar-keys.wav", soundId, true);
	SoundMap::idMap[SoundEnum::DrugwarKeys] = soundId;
	_audioEngine->LoadSound(L"res/audio/drugwar-whirly.wav", soundId, true);
	SoundMap::idMap[SoundEnum::DrugwarWhirly] = soundId;
	_audioEngine->LoadSound(L"res/audio/drugwar-coconut.wav", soundId, true);
	SoundMap::idMap[SoundEnum::DrugwarCoconut] = soundId;
	_audioEngine->LoadSound(L"res/audio/drugwar-xylo.wav", soundId, true);
	SoundMap::idMap[SoundEnum::DrugwarXylo] = soundId;

	CreateNewWorld();
}

int World::tick(long long microsecondsElapsed)
{
	// World step
	float stepInSeconds = microsecondsElapsed / 1000000.0f;
	_world->Step(stepInSeconds, 16, 6);

	// Elapse time for units
	for (auto& pair : _unitList) {
		Unit* unit = pair.second;
		unit->timeElapsed(microsecondsElapsed);

		// Do effects
		while (unit->hasEffectInQueue()) {
			Effect* effect = unit->getEffectInQueue();
			if (effect->getType() == EffectType::SummonBlackHoleShot) {
				SummonBlackHoleShotEffect* sbhsEffect = static_cast<SummonBlackHoleShotEffect*>(effect);
				int unitId = addUnitBlackHoleShot(sbhsEffect->getX(), sbhsEffect->getY());
				if (unit->getUnitType() == UnitType::Player) { // This should always resolve to true, since only Player units use this effect.
					Player* player = static_cast<Player*>(unit);
					player->setBlackHoleShotId(unitId);
				}
			} else if (effect->getType() == EffectType::SetUnitAlpha) {
				SetUnitAlphaEffect* suaEffect = static_cast<SetUnitAlphaEffect*>(effect);
				Unit* blackHoleShotUncast = getUnit(suaEffect->getUnitId());
				BlackHoleShot* blackHoleShot = static_cast<BlackHoleShot*>(blackHoleShotUncast);
				blackHoleShot->setAlpha(suaEffect->getAlpha());
			} else if (effect->getType() == EffectType::LaunchPendingProjectile) {
				LaunchPendingProjectileEffect* lppEffect = static_cast<LaunchPendingProjectileEffect*>(effect);
				Unit* blackHoleShotUncast = getUnit(lppEffect->getUnitId());
				BlackHoleShot* blackHoleShot = static_cast<BlackHoleShot*>(blackHoleShotUncast);
				blackHoleShot->launchProjectile();
			} else if (effect->getType() == EffectType::MoveUnit) {
				MoveUnitEffect* muEffect = static_cast<MoveUnitEffect*>(effect);
				Unit* blackHoleShotUncast = getUnit(muEffect->getUnitId());
				BlackHoleShot* blackHoleShot = static_cast<BlackHoleShot*>(blackHoleShotUncast);
				blackHoleShot->moveTo(muEffect->getX(), muEffect->getY());
			} else if (effect->getType() == EffectType::SummonButterfly) {
				SummonButterflyEffect* sbEffect = static_cast<SummonButterflyEffect*>(effect);
				// Butterfly 1
				int unitId = addUnitColorFlyer(sbEffect->getX(), sbEffect->getY(), 0xFF0000);
				Unit* unit = World::getUnit(unitId);
				ColorFlyer* colorFlyer = static_cast<ColorFlyer*>(unit);
				colorFlyer->setVelocity(sbEffect->getV() * 8, 0);
				// Butterfly 2
				unitId = addUnitColorFlyer(sbEffect->getX() + 0.5, sbEffect->getY(), 0xFF0000);
				unit = World::getUnit(unitId);
				colorFlyer = static_cast<ColorFlyer*>(unit);
				colorFlyer->setVelocity(sbEffect->getV() * 7, 0);
				// Butterfly 3
				unitId = addUnitColorFlyer(sbEffect->getX(), sbEffect->getY() + 0.5, 0xFF0000);
				unit = World::getUnit(unitId);
				colorFlyer = static_cast<ColorFlyer*>(unit);
				colorFlyer->setVelocity(sbEffect->getV() * 6, 0);
			} else if (effect->getType() == EffectType::SummonProjectile) {
				SummonProjectileEffect* spEffect = static_cast<SummonProjectileEffect*>(effect);
				if (spEffect->getUnitType() == UnitType::Bubble) {
					int unitId = addUnitBubble(
						spEffect->getProjectilePos().x,
						spEffect->getProjectilePos().y);
					Unit* unit = World::getUnit(unitId);
					BubbleUnit* bubble = static_cast<BubbleUnit*>(unit);
					bubble->setVelocity(spEffect->getLaunchVelocity().x, spEffect->getLaunchVelocity().y);
					bubble->setGravity(spEffect->getProjectileGravity());
				}
			}
			delete effect;
		}
	}

	// Destroy units with flag set
	for (auto it = _unitList.begin(); it != _unitList.end(); ) {
		Unit* unit = (*it).second;
		if (unit->FlagDestroy) {
			unit->destroy();
			delete unit;
			it = _unitList.erase(it);
		} else {
			it++;
		}
	}
	// Disable units with flag set
	for (auto& pair : _unitList) {
		if (pair.second->shouldDisable()) {
			pair.second->disable();
		}
	}

	// Play sounds
	for (auto unit : _unitList) {
		if (unit.second->hasSoundInQueue()) {
			SoundEnum sound = unit.second->getSoundInQueue();
			_audioEngine->PlaySound(SoundMap::idMap[sound]);
		}
	}

	// Periodic release audio resources
	static int releaseAudioResourcesCounter{ 0 };
	releaseAudioResourcesCounter++;
	if (releaseAudioResourcesCounter % 600 == 0) { // 600 means once every 10 seconds (given 60 game ticks per second)
		_audioEngine->PeriodicReleaseResources();
	}

	// Regulate motion
	for (auto unit : _unitList) {
		unit.second->regulate();
	}

	// Animations
	for (auto unit : _visualUnitList) {
		unit.second->timeStepForGraphics(microsecondsElapsed);
	}
	for (auto pair : _unitList) {
		pair.second->timeStepForGraphics(microsecondsElapsed);
	}
	
	return 0;
}

enum class DrawZIndexType { Unit, Visual };
struct DrawZIndex
{
public:
	DrawZIndex(DrawZIndexType zType, int pUnitIndex, int pZIndex)
	{
		type = zType;
		unitIndex = pUnitIndex;
		zIndex = pZIndex;
	}

	DrawZIndexType type;
	int unitIndex;
	int zIndex;
};

bool compareDrawZIndex(DrawZIndex& d1, DrawZIndex& d2)
{
	return d1.zIndex < d2.zIndex;
}

void World::draw()
{
	if (true) // set to false for psycodelic mode
		_graphics->fillScreen(0x000000);
	_graphics->setWorldDraw();

	// Order units by z-index
	std::vector<DrawZIndex> drawList{};
	for (auto unit : _visualUnitList) {
		drawList.push_back(DrawZIndex(DrawZIndexType::Visual, unit.first, unit.second->getZIndex()));
	}
	for (auto unit : _unitList) {
		drawList.push_back(DrawZIndex(DrawZIndexType::Unit, unit.first, unit.second->getZIndex()));
	}
	std::sort(drawList.begin(), drawList.end(), compareDrawZIndex);

	for (auto& item : drawList) {
		DrawableObject* obj;
		if (item.type == DrawZIndexType::Unit) {
			obj = static_cast<DrawableObject*>(_unitList[item.unitIndex]);
		} else {
			obj = static_cast<DrawableObject*>(_visualUnitList[item.unitIndex]);
		}
		std::list<DrawInstructions*>* diList = obj->getDrawInstructions();
		for (auto di : *diList) {
			if (!_globalDrawImages && di->type == DrawInstructions::DrawType::image) { // Don't draw image if switch is off.
				DrawInstructionsImage* dii = static_cast<DrawInstructionsImage*>(di);
				delete dii;
				continue;
			}
			_graphics->draw(di);
		}
		delete diList;
	}

	if (_globalDebugDraw) {
		_graphics->setScreenDraw();
		_graphics->drawText(
			std::wstring() + L"Mouse world: (" + std::to_wstring(_mouseWorldPos.x) + L", " + std::to_wstring(_mouseWorldPos.y) + L")",
			L"Georgia", 20.0f, 20, 50, 500, 500, 0x808080, 1.0f);
		_graphics->drawText(
			std::wstring() + L"Mouse screen: (" + std::to_wstring(_mouseScreenX) + L", " + std::to_wstring(_mouseScreenY) + L")",
			L"Georgia", 20.0f, 20, 20, 500, 500, 0x808080, 1.0f);
		_graphics->drawText(
			std::wstring() + L"Debug value: " + _debugStr,
			L"Georgia", 20.0f, 20, 80, 500, 500, 0x808080, 1.0f);
	}
}

void World::reloadGraphics()
{
	for (auto a : GraphicsMap::idMap) {
		_graphics->releaseBmp(a.second);
	}
	GraphicsMap::idMap.clear();
	// Load graphics
	int graphicsId = _graphics->createBmpFromFile(L"res/images/arrow.png");
	GraphicsMap::idMap[GraphicsEnum::PurpleArrow] = graphicsId;
	graphicsId = _graphics->createBmpFromFile(L"res/images/smile.png");
	GraphicsMap::idMap[GraphicsEnum::PoorlyDrawnSmile] = graphicsId;
	graphicsId = _graphics->createBmpFromFile(L"res/images/penguin.jpg");
	GraphicsMap::idMap[GraphicsEnum::PenguinBackground] = graphicsId;
	graphicsId = _graphics->createBmpFromFile(L"res/images/space-bg-looped.png");
	GraphicsMap::idMap[GraphicsEnum::SpaceBg] = graphicsId;
	// Person 1 walking
	graphicsId = _graphics->createBmpFromFile(L"res/images/person 1/walking-1.png");
	GraphicsMap::idMap[GraphicsEnum::Person1Walking1] = graphicsId;
	graphicsId = _graphics->createBmpFromFile(L"res/images/person 1/walking-2.png");
	GraphicsMap::idMap[GraphicsEnum::Person1Walking2] = graphicsId;
	graphicsId = _graphics->createBmpFromFile(L"res/images/person 1/walking-3.png");
	GraphicsMap::idMap[GraphicsEnum::Person1Walking3] = graphicsId;
	graphicsId = _graphics->createBmpFromFile(L"res/images/person 1/walking-4.png");
	GraphicsMap::idMap[GraphicsEnum::Person1Walking4] = graphicsId;
	graphicsId = _graphics->createBmpFromFile(L"res/images/person 1/walking-5.png");
	GraphicsMap::idMap[GraphicsEnum::Person1Walking5] = graphicsId;
	graphicsId = _graphics->createBmpFromFile(L"res/images/person 1/walking-6.png");
	GraphicsMap::idMap[GraphicsEnum::Person1Walking6] = graphicsId;
	// Person 1 idle
	graphicsId = _graphics->createBmpFromFile(L"res/images/person 1/idle-1.png");
	GraphicsMap::idMap[GraphicsEnum::Person1Idle1] = graphicsId;
	graphicsId = _graphics->createBmpFromFile(L"res/images/person 1/idle-2.png");
	GraphicsMap::idMap[GraphicsEnum::Person1Idle2] = graphicsId;
	graphicsId = _graphics->createBmpFromFile(L"res/images/person 1/idle-3.png");
	GraphicsMap::idMap[GraphicsEnum::Person1Idle3] = graphicsId;
	graphicsId = _graphics->createBmpFromFile(L"res/images/person 1/idle-4.png");
	GraphicsMap::idMap[GraphicsEnum::Person1Idle4] = graphicsId;
	graphicsId = _graphics->createBmpFromFile(L"res/images/person 1/idle-5.png");
	GraphicsMap::idMap[GraphicsEnum::Person1Idle5] = graphicsId;
	graphicsId = _graphics->createBmpFromFile(L"res/images/person 1/idle-6.png");
	GraphicsMap::idMap[GraphicsEnum::Person1Idle6] = graphicsId;
	// Person 1 special
	graphicsId = _graphics->createBmpFromFile(L"res/images/person 1/special-1.png");
	GraphicsMap::idMap[GraphicsEnum::Person1Special1] = graphicsId;
	graphicsId = _graphics->createBmpFromFile(L"res/images/person 1/special-2.png");
	GraphicsMap::idMap[GraphicsEnum::Person1Special2] = graphicsId;
	graphicsId = _graphics->createBmpFromFile(L"res/images/person 1/special-3.png");
	GraphicsMap::idMap[GraphicsEnum::Person1Special3] = graphicsId;
	graphicsId = _graphics->createBmpFromFile(L"res/images/person 1/special-4.png");
	GraphicsMap::idMap[GraphicsEnum::Person1Special4] = graphicsId;
	graphicsId = _graphics->createBmpFromFile(L"res/images/person 1/special-5.png");
	GraphicsMap::idMap[GraphicsEnum::Person1Special5] = graphicsId;
	graphicsId = _graphics->createBmpFromFile(L"res/images/person 1/special-6.png");
	GraphicsMap::idMap[GraphicsEnum::Person1Special6] = graphicsId;
	// Black hole
	graphicsId = _graphics->createBmpFromFile(L"res/images/black hole/1.png");
	GraphicsMap::idMap[GraphicsEnum::BlackHole1] = graphicsId;
	graphicsId = _graphics->createBmpFromFile(L"res/images/black hole/2.png");
	GraphicsMap::idMap[GraphicsEnum::BlackHole2] = graphicsId;
	graphicsId = _graphics->createBmpFromFile(L"res/images/black hole/3.png");
	GraphicsMap::idMap[GraphicsEnum::BlackHole3] = graphicsId;
	graphicsId = _graphics->createBmpFromFile(L"res/images/black hole/4.png");
	GraphicsMap::idMap[GraphicsEnum::BlackHole4] = graphicsId;
	graphicsId = _graphics->createBmpFromFile(L"res/images/black hole/5.png");
	GraphicsMap::idMap[GraphicsEnum::BlackHole5] = graphicsId;
	graphicsId = _graphics->createBmpFromFile(L"res/images/black hole/6.png");
	GraphicsMap::idMap[GraphicsEnum::BlackHole6] = graphicsId;
	graphicsId = _graphics->createBmpFromFile(L"res/images/black hole/7.png");
	GraphicsMap::idMap[GraphicsEnum::BlackHole7] = graphicsId;
	graphicsId = _graphics->createBmpFromFile(L"res/images/black hole/8.png");
	GraphicsMap::idMap[GraphicsEnum::BlackHole8] = graphicsId;
	// Butterfly
	graphicsId = _graphics->createBmpFromFile(L"res/images/butterfly sprite/butterfly1.png");
	GraphicsMap::idMap[GraphicsEnum::Butterfly1] = graphicsId;
	graphicsId = _graphics->createBmpFromFile(L"res/images/butterfly sprite/butterfly2.png");
	GraphicsMap::idMap[GraphicsEnum::Butterfly2] = graphicsId;
	graphicsId = _graphics->createBmpFromFile(L"res/images/butterfly sprite/butterfly3.png");
	GraphicsMap::idMap[GraphicsEnum::Butterfly3] = graphicsId;
	graphicsId = _graphics->createBmpFromFile(L"res/images/butterfly sprite/butterfly4.png");
	GraphicsMap::idMap[GraphicsEnum::Butterfly4] = graphicsId;
	// Evil Wizard idle
	graphicsId = _graphics->createBmpFromFile(L"res/images/EVil Wizard 2/Sprites/idle/1.png");
	GraphicsMap::idMap[GraphicsEnum::EvilWizardIdle1] = graphicsId;
	graphicsId = _graphics->createBmpFromFile(L"res/images/EVil Wizard 2/Sprites/idle/2.png");
	GraphicsMap::idMap[GraphicsEnum::EvilWizardIdle2] = graphicsId;
	graphicsId = _graphics->createBmpFromFile(L"res/images/EVil Wizard 2/Sprites/idle/3.png");
	GraphicsMap::idMap[GraphicsEnum::EvilWizardIdle3] = graphicsId;
	graphicsId = _graphics->createBmpFromFile(L"res/images/EVil Wizard 2/Sprites/idle/4.png");
	GraphicsMap::idMap[GraphicsEnum::EvilWizardIdle4] = graphicsId;
	graphicsId = _graphics->createBmpFromFile(L"res/images/EVil Wizard 2/Sprites/idle/5.png");
	GraphicsMap::idMap[GraphicsEnum::EvilWizardIdle5] = graphicsId;
	graphicsId = _graphics->createBmpFromFile(L"res/images/EVil Wizard 2/Sprites/idle/6.png");
	GraphicsMap::idMap[GraphicsEnum::EvilWizardIdle6] = graphicsId;
	graphicsId = _graphics->createBmpFromFile(L"res/images/EVil Wizard 2/Sprites/idle/7.png");
	GraphicsMap::idMap[GraphicsEnum::EvilWizardIdle7] = graphicsId;
	graphicsId = _graphics->createBmpFromFile(L"res/images/EVil Wizard 2/Sprites/idle/8.png");
	GraphicsMap::idMap[GraphicsEnum::EvilWizardIdle8] = graphicsId;
	// Evil Wizard run
	graphicsId = _graphics->createBmpFromFile(L"res/images/EVil Wizard 2/Sprites/run/1.png");
	GraphicsMap::idMap[GraphicsEnum::EvilWizardRun1] = graphicsId;
	graphicsId = _graphics->createBmpFromFile(L"res/images/EVil Wizard 2/Sprites/run/2.png");
	GraphicsMap::idMap[GraphicsEnum::EvilWizardRun2] = graphicsId;
	graphicsId = _graphics->createBmpFromFile(L"res/images/EVil Wizard 2/Sprites/run/3.png");
	GraphicsMap::idMap[GraphicsEnum::EvilWizardRun3] = graphicsId;
	graphicsId = _graphics->createBmpFromFile(L"res/images/EVil Wizard 2/Sprites/run/4.png");
	GraphicsMap::idMap[GraphicsEnum::EvilWizardRun4] = graphicsId;
	graphicsId = _graphics->createBmpFromFile(L"res/images/EVil Wizard 2/Sprites/run/5.png");
	GraphicsMap::idMap[GraphicsEnum::EvilWizardRun5] = graphicsId;
	graphicsId = _graphics->createBmpFromFile(L"res/images/EVil Wizard 2/Sprites/run/6.png");
	GraphicsMap::idMap[GraphicsEnum::EvilWizardRun6] = graphicsId;
	graphicsId = _graphics->createBmpFromFile(L"res/images/EVil Wizard 2/Sprites/run/7.png");
	GraphicsMap::idMap[GraphicsEnum::EvilWizardRun7] = graphicsId;
	graphicsId = _graphics->createBmpFromFile(L"res/images/EVil Wizard 2/Sprites/run/8.png");
	GraphicsMap::idMap[GraphicsEnum::EvilWizardRun8] = graphicsId;
	// Evil Wizard jump
	graphicsId = _graphics->createBmpFromFile(L"res/images/EVil Wizard 2/Sprites/jump/1.png");
	GraphicsMap::idMap[GraphicsEnum::EvilWizardJump1] = graphicsId;
	graphicsId = _graphics->createBmpFromFile(L"res/images/EVil Wizard 2/Sprites/jump/2.png");
	GraphicsMap::idMap[GraphicsEnum::EvilWizardJump2] = graphicsId;
	// Evil Wizard fall
	graphicsId = _graphics->createBmpFromFile(L"res/images/EVil Wizard 2/Sprites/fall/1.png");
	GraphicsMap::idMap[GraphicsEnum::EvilWizardFall1] = graphicsId;
	graphicsId = _graphics->createBmpFromFile(L"res/images/EVil Wizard 2/Sprites/fall/2.png");
	GraphicsMap::idMap[GraphicsEnum::EvilWizardFall2] = graphicsId;
	// Evil Wizard attack one
	graphicsId = _graphics->createBmpFromFile(L"res/images/EVil Wizard 2/Sprites/attack1/1.png");
	GraphicsMap::idMap[GraphicsEnum::EvilWizardAttackOne1] = graphicsId;
	graphicsId = _graphics->createBmpFromFile(L"res/images/EVil Wizard 2/Sprites/attack1/2.png");
	GraphicsMap::idMap[GraphicsEnum::EvilWizardAttackOne2] = graphicsId;
	graphicsId = _graphics->createBmpFromFile(L"res/images/EVil Wizard 2/Sprites/attack1/3.png");
	GraphicsMap::idMap[GraphicsEnum::EvilWizardAttackOne3] = graphicsId;
	graphicsId = _graphics->createBmpFromFile(L"res/images/EVil Wizard 2/Sprites/attack1/4.png");
	GraphicsMap::idMap[GraphicsEnum::EvilWizardAttackOne4] = graphicsId;
	graphicsId = _graphics->createBmpFromFile(L"res/images/EVil Wizard 2/Sprites/attack1/5.png");
	GraphicsMap::idMap[GraphicsEnum::EvilWizardAttackOne5] = graphicsId;
	graphicsId = _graphics->createBmpFromFile(L"res/images/EVil Wizard 2/Sprites/attack1/6.png");
	GraphicsMap::idMap[GraphicsEnum::EvilWizardAttackOne6] = graphicsId;
	graphicsId = _graphics->createBmpFromFile(L"res/images/EVil Wizard 2/Sprites/attack1/7.png");
	GraphicsMap::idMap[GraphicsEnum::EvilWizardAttackOne7] = graphicsId;
	graphicsId = _graphics->createBmpFromFile(L"res/images/EVil Wizard 2/Sprites/attack1/8.png");
	GraphicsMap::idMap[GraphicsEnum::EvilWizardAttackOne8] = graphicsId;
	// Bubble effect
	loadImageGroup(L"C:/Users/jowar/My Folder/programming/C++/VS2019 projects/Game1/Game1/res/images/bubble effect/", GraphicsEnum::BubbleEffect0, 60);
	// Nice ground
	std::wstring niceGroundPath = L"res/images/pixel adventure ground/";
	loadImage(niceGroundPath + L"tl.png", GraphicsEnum::NiceGroundTopLeft);
	loadImage(niceGroundPath + L"tm.png", GraphicsEnum::NiceGroundTopMid);
	loadImage(niceGroundPath + L"tr.png", GraphicsEnum::NiceGroundTopRight);
	loadImage(niceGroundPath + L"ml.png", GraphicsEnum::NiceGroundMidLeft);
	loadImage(niceGroundPath + L"mm.png", GraphicsEnum::NiceGroundMidMid);
	loadImage(niceGroundPath + L"mr.png", GraphicsEnum::NiceGroundMidRight);
	loadImage(niceGroundPath + L"bl.png", GraphicsEnum::NiceGroundBottomLeft);
	loadImage(niceGroundPath + L"bm.png", GraphicsEnum::NiceGroundBottomMid);
	loadImage(niceGroundPath + L"br.png", GraphicsEnum::NiceGroundBottomRight);
	// Tree
	loadImage(std::wstring(L"res/images/tree.png"), GraphicsEnum::Tree);
	// 20x20 tile
	loadImage(std::wstring(L"res/images/20x20-tile.png"), GraphicsEnum::TwentyMiscTile);
	// Circle rune 1
	loadImage(std::wstring(L"res/images/circle-rune-1.png"), GraphicsEnum::CircleRune1);
	// Circle rune 2
	loadImage(std::wstring(L"res/images/circle-rune-2-2.png"), GraphicsEnum::CircleRune2);
	// Square rune 1
	loadImage(std::wstring(L"res/images/square-rune-1.png"), GraphicsEnum::SquareRune1);
	// Bubble
	loadImage(std::wstring(L"res/images/bubble.png"), GraphicsEnum::Bubble);
	// Grim cathedral
	loadImage(std::wstring(L"res/images/grim cathedral.png"), GraphicsEnum::GrimCathedral);
}

void World::BeginContact(b2Contact* contact)
{
	int fixtureIdA = (int)contact->GetFixtureA()->GetUserData();
	int fixtureIdB = (int)contact->GetFixtureB()->GetUserData();
	UnitBodyIdHolder* idHolderA = (UnitBodyIdHolder*)contact->GetFixtureA()->GetBody()->GetUserData();
	UnitBodyIdHolder* idHolderB = (UnitBodyIdHolder*)contact->GetFixtureB()->GetBody()->GetUserData();
	Unit* unitA = getUnit(idHolderA->unitId);
	Unit* unitB = getUnit(idHolderB->unitId);

	b2WorldManifold worldManifold;
	contact->GetWorldManifold(&worldManifold);
	b2Vec2 worldNormal = worldManifold.normal;
	b2Vec2 inverseWorldNormal = b2Vec2(-worldNormal.x, -worldNormal.y);
	
	unitA->onContactBegin(unitB, idHolderB->bodyId, fixtureIdB, idHolderA->bodyId, fixtureIdA, inverseWorldNormal);
	unitB->onContactBegin(unitA, idHolderA->bodyId, fixtureIdA, idHolderB->bodyId, fixtureIdB, worldNormal);
}

void World::EndContact(b2Contact* contact)
{
	int fixtureIdA = (int)contact->GetFixtureA()->GetUserData();
	int fixtureIdB = (int)contact->GetFixtureB()->GetUserData();
	UnitBodyIdHolder* idHolderA = (UnitBodyIdHolder*)contact->GetFixtureA()->GetBody()->GetUserData();
	UnitBodyIdHolder* idHolderB = (UnitBodyIdHolder*)contact->GetFixtureB()->GetBody()->GetUserData();
	Unit* unitA = _unitList.at(idHolderA->unitId);
	Unit* unitB = _unitList.at(idHolderB->unitId);

	unitA->onContactEnd(unitB, idHolderB->bodyId, fixtureIdB, idHolderA->bodyId, fixtureIdA);
	unitB->onContactEnd(unitA, idHolderA->bodyId, fixtureIdA, idHolderB->bodyId, fixtureIdB);
}

void World::CreateNewWorld()
{
	disposeAll();
	_world = new b2World(b2Vec2(0, 0));
	_world->SetContactListener(this);
}

void World::SetGravity(float x, float y)
{
	b2Vec2 gravity(x, y);
	_world->SetGravity(gravity);
}

int World::addUnitWall(float x, float y, float w, float h, float angle, int color, bool bouncy, bool visible)
{
	int id = getNewUnitId();
	auto unit = new Wall(&*_world, x, y, w, h, angle, color, id, bouncy, visible);
	unit->setDebugDraw(_globalDebugDraw);
	_unitList.insert(std::pair<int, Wall*>(id, unit));
	return id;
}

int World::addUnitKillZone(float x, float y, float w, float h, float angle, int color)
{
	int id = getNewUnitId();
	auto unit = new KillZone(_world, x, y, w, h, angle, color, id);
	unit->setDebugDraw(_globalDebugDraw);
	_unitList.insert(std::pair<int, KillZone*>(id, unit));
	return id;
}

int World::addUnitNiceGround(float x, float y, float w, float h, float angle)
{
	int id = getNewUnitId();
	int d = 4;
	auto unit = new NiceGroundUnit(_world, id, x, y, w, h, angle, 45.0f - d, 48.0f - d, 45.0f - d, 48.0f - d, 48.0f - d, 42.0f - d, 0.03);
	unit->setDebugDraw(_globalDebugDraw);
	_unitList.insert(std::pair<int, NiceGroundUnit*>(id, unit));
	return id;
}

/*void World::addUnitUnit1(float x, float y, SoundEnum sound)
{
	int id = getNewUnitId();
	auto unit = new Unit1(&*_world, x, y, id, sound);
	unit->setDebugDraw(_globalDebugDraw);
	_unitList.insert(std::pair<int, Unit1*>(id, unit));
}*/

void World::addUnitBouncyCircle(float x, float y, int color, SoundEnum sound)
{
	int id = getNewUnitId();
	auto unit = new BouncyCircle(&*_world, x, y, 1.0f, color, id, sound);
	unit->setDebugDraw(_globalDebugDraw);
	_unitList.insert(std::pair<int, BouncyCircle*>(id, unit));
}

void World::addUnitBouncyCirclePlayer(float x, float y, int color)
{
	int id = getNewUnitId();
	auto unit = new BouncyCircle(&*_world, x, y, 0.8, color, id, SoundEnum::PianoC6);
	unit->setDebugDraw(_globalDebugDraw);
	unit->FlagPlayerControlled = true;
	_unitList.insert(std::pair<int, BouncyCircle*>(id, unit));
}

int World::addUnitPlayer(float x, float y)
{
	int id = getNewUnitId();
	auto unit = new Player(&*_world, x, y, id);
	unit->setDebugDraw(_globalDebugDraw);
	unit->FlagPlayerControlled = true;
	_unitList.insert(std::pair<int, Player*>(id, unit));
	return id;
}

int World::addUnitColorFlyer(float x, float y, int color)
{
	int id = getNewUnitId();
	auto unit = new ColorFlyer(&*_world, x, y, color, id);
	unit->setDebugDraw(_globalDebugDraw);
	_unitList.insert(std::pair<int, ColorFlyer*>(id, unit));
	return id;
}

int World::addUnitBubble(float x, float y)
{
	int id = getNewUnitId();
	auto unit = new BubbleUnit(&*_world, x, y, id);
	unit->setDebugDraw(_globalDebugDraw);
	_unitList.insert(std::pair<int, BubbleUnit*>(id, unit));
	return id;
}

int World::addUnitBlackHoleShot(float x, float y)
{
	int id = getNewUnitId();
	auto unit = new BlackHoleShot(&*_world, x, y, id);
	unit->setDebugDraw(_globalDebugDraw);
	_unitList.insert(std::pair<int, BlackHoleShot*>(id, unit));
	return id;
}

/*void World::addUnitCircleUnit(float x, float y, int color)
{
	int id = getNewUnitId();
	auto unit = new CircleUnit(&*_world, x, y, color, id);
	unit->setDebugDraw(_globalDebugDraw);
	_unitList.insert(std::pair<int, CircleUnit*>(id, unit));
}*/

void World::addUnitTestJoint(float x, float y, float motorSpeedInRadians)
{
	int id = getNewUnitId();
	auto unit = new TestJoint(&*_world, x, y, motorSpeedInRadians, id);
	unit->setDebugDraw(_globalDebugDraw);
	_unitList.insert(std::pair<int, TestJoint*>(id, unit));
}

int World::addUnitWallBuilder()
{
	int id = getNewUnitId();
	auto unit = new WallBuilderUnit(&*_world, id);
	unit->setDebugDraw(_globalDebugDraw);
	_unitList.insert(std::pair<int, WallBuilderUnit*>(id, unit));
	return id;
}

/*int World::addUnitSquare(float x, float y)
{
	int id = getNewUnitId();
	auto unit = new Square(&*_world, x, y, 1, 1, 0x0080FF, id);
	unit->setDebugDraw(_globalDebugDraw);
	_unitList.insert(std::pair<int, Square*>(id, unit));
	return id;
}

void World::addUnitFlipperUnit(float x, float y, bool right)
{
	int id = getNewUnitId();
	auto unit = new FlipperUnit(&*_world, x, y, right, id);
	unit->setDebugDraw(_globalDebugDraw);
	_unitList.insert(std::pair<int, FlipperUnit*>(id, unit));
}

void World::addUnitBlackHoleUnit(float x, float y, SoundEnum sound)
{
	int id = getNewUnitId();
	auto unit = new BlackHoleUnit(&*_world, x, y, id, sound);
	unit->setDebugDraw(_globalDebugDraw);
	_unitList.insert(std::pair<int, BlackHoleUnit*>(id, unit));
}

int World::addUnitShotForLevel6(float x, float y)
{
	int unitId = getNewUnitId();
	auto unit = new ShotForLevel6(&*_world, x, y, unitId);
	unit->setDebugDraw(_globalDebugDraw);
	_unitList.insert(std::pair<int, ShotForLevel6*>(unitId, unit));
	return unitId;
}

int World::addUnitLevel6BasketUnit(float x, float y)
{
	int id = getNewUnitId();
	auto unit = new Level6BasketUnit(&*_world, x, y, id);
	unit->setDebugDraw(_globalDebugDraw);
	_unitList.insert(std::pair<int, Level6BasketUnit*>(id, unit));
	return id;
}

int World::addUnitLevel6SliderUnit(float x, float y, std::wstring title)
{
	int id = getNewUnitId();
	auto unit = new Level6SliderUnit(&*_world, x, y, id, title);
	unit->setDebugDraw(_globalDebugDraw);
	_unitList.insert(std::pair<int, Level6SliderUnit*>(id, unit));
	return id;
}*/

int World::addUnitStaticImageVisualUnit(float x, float y, float scale, float alpha, std::vector<std::vector<GraphicsEnum>> animationSet)
{
	int id = getNewUnitId();
	_visualUnitList[id] = new StaticImageVisualUnit(x, y, scale, alpha, animationSet);
	return id;
}

void World::loadImage(std::wstring file, GraphicsEnum graphic)
{
	GraphicsMap::idMap[graphic] = _graphics->createBmpFromFile(file.c_str());
}

/* This imports a whole folder of images for one animation. The images must be PNG files
 * of the format "<num>.png", where the image files names start as "0.png" and increase.
 * The *folder* parameter must include the slash at the end.
 */
void World::loadImageGroup(std::wstring folder, GraphicsEnum startingImageEnum, int finalImageNum)
{
	int i = 0;
	while (i <= finalImageNum) {
		loadImage(folder + std::to_wstring(i) + L".png", (GraphicsEnum)(startingImageEnum + i));
		i++;
	}
}

void World::createForceExplosion(float x, float y, float radius, float force)
{
	/*for (auto pair : _unitList) {
		Unit* unit = pair.second;
		auto pos = unit->getPos();
		float m = sqrt(powf(x - pos.x, 2) + powf(y - pos.y, 2));
		if (m <= radius) {
			float newForce = sqrt(sqrt(1 - m / radius)) * force;
			unit->megaPush(pos.x - x, pos.y - y, newForce);
		}
	}*/
}

int World::getNewUnitId()
{
	int id = _unitIdIncrementer;
	_unitIdIncrementer++;
	return id;
}

void World::disposeAll()
{
	if (_world != 0) {
		delete _world;
		_world = 0;
	}
	for (auto it = _unitList.begin(); it != _unitList.end(); ) {
		delete (*it).second;
		it = _unitList.erase(it);
	}
	for (auto it = _visualUnitList.begin(); it != _visualUnitList.end(); ) {
		delete (*it).second;
		it = _visualUnitList.erase(it);
	}
}

void World::setDebugStr(std::wstring newDebugStr)
{
	_debugStr = newDebugStr;
}

void World::moveUnit(int unitId, bool left, bool right, bool up, bool down)
{
	_unitList[unitId]->push(b2Vec2((int)right - (int)left, (int)up - (int)down), 3.0f);
}

void World::setDebugDrawOn(bool debugDrawOn)
{
	_globalDebugDraw = debugDrawOn;
	for (auto& pair : _unitList) {
		pair.second->setDebugDraw(_globalDebugDraw);
	}
}

void World::rotateColorFlyers()
{
	float bottom = -17 + 13;
	float maxForce = 32;
	float top = 16 + 13;
	float minForce = 24;

	for (auto& pair : _unitList) {
		if (pair.second->getUnitType() == UnitType::ColorFlyer) {
			ColorFlyer* cf = static_cast<ColorFlyer*>(pair.second);
			b2Vec2 pos = cf->getBody(0)->getPos();
			float force = ((minForce - maxForce) / (top - bottom)) * (pos.y - bottom) + maxForce;
			cf->getBody(0)->spin(0.0f, 1.0f, force);
		}
	}
}

// To implement
bool World::isUnitInArea(float left, float top, float right, float bottom)
{
	return true;
}

void World::setMousePos(int mouseScreenX, int mouseScreenY)
{
	_mouseScreenX = mouseScreenX;
	_mouseScreenY = mouseScreenY;
	auto worldPt = _graphics->convertScreenToWorldPoint(DrawInstructionsPoint(_mouseScreenX, _mouseScreenY));
	_mouseWorldPos = b2Vec2(worldPt.x, worldPt.y);
}

Unit* World::getUnit(int unitId)
{
	return _unitList[unitId];
}

void World::toggleDrawImages()
{
	_globalDrawImages = !_globalDrawImages;
}
