#include "Player.h"
#include "Unit1.h"
#include "GraphicsMap.h"
#include "DrawInstructionsInclude.h"

Player::Player(b2World* world, float x, float y, int id) :
	Unit(UnitType::Player, world, id),
	CombatObject(100)
{
	_bodyId = Unit::createBody(b2BodyType::b2_dynamicBody, x, y, id);
	Body* body = Unit::getBody(_bodyId);
	body->setFixedRotation(true);

	float width = 0.9f;
	float height = 2.6f;
	float density = 1.0f;
	float friction = 0.3f;
	float angle = 0.0f;
	float bounciness = 0.0f;
	std::vector<std::vector<GraphicsEnum>> animation = {
		{
			GraphicsEnum::EvilWizardRun1, GraphicsEnum::EvilWizardRun2, GraphicsEnum::EvilWizardRun3, GraphicsEnum::EvilWizardRun4,
			GraphicsEnum::EvilWizardRun5, GraphicsEnum::EvilWizardRun6, GraphicsEnum::EvilWizardRun7, GraphicsEnum::EvilWizardRun8
		},
		{
			GraphicsEnum::EvilWizardIdle1, GraphicsEnum::EvilWizardIdle2, GraphicsEnum::EvilWizardIdle3, GraphicsEnum::EvilWizardIdle4,
			GraphicsEnum::EvilWizardIdle5, GraphicsEnum::EvilWizardIdle6, GraphicsEnum::EvilWizardIdle7, GraphicsEnum::EvilWizardIdle8
		},
		{
			GraphicsEnum::EvilWizardFall1, GraphicsEnum::EvilWizardFall2
		},
		{
			GraphicsEnum::EvilWizardJump1, GraphicsEnum::EvilWizardJump2
		},
		{
			GraphicsEnum::EvilWizardFall1, GraphicsEnum::EvilWizardFall2
		},
		{
			GraphicsEnum::EvilWizardJump1, GraphicsEnum::EvilWizardJump2
		}
	};
	_runningAnimationId = 0;
	_idleAnimationId = 1;
	_fallingAnimationId = 2;
	_risingAnimationId = 3;
	_castingAnimationId = 4;
	_doneCastingAnimationId = 5;
	_fixtureId = body->addBox(0, 0, width, height, angle, density, friction, bounciness, 0x00DEEF, 0.05f, 0, 1.0f, animation, false, 0.0f, 0.75f);
	_slowVXTimerId = TimableObject::addTimer(0.05f, L"slow vx");

	// Create casting bubbles
	std::vector<std::vector<GraphicsEnum>> animationCasting = {
		{
			BubbleEffect0, BubbleEffect1, BubbleEffect2, BubbleEffect3, BubbleEffect4, BubbleEffect5, BubbleEffect6, BubbleEffect7,
			BubbleEffect8, BubbleEffect9, BubbleEffect10, BubbleEffect11, BubbleEffect12, BubbleEffect13, BubbleEffect14, BubbleEffect15,
			BubbleEffect16, BubbleEffect17, BubbleEffect18, BubbleEffect19, BubbleEffect20, BubbleEffect21, BubbleEffect22, BubbleEffect23,
			BubbleEffect24, BubbleEffect25, BubbleEffect26, BubbleEffect27, BubbleEffect28, BubbleEffect29, BubbleEffect30, BubbleEffect31,
			BubbleEffect32, BubbleEffect33, BubbleEffect34, BubbleEffect35, BubbleEffect36, BubbleEffect37, BubbleEffect38, BubbleEffect39,
			BubbleEffect40, BubbleEffect41, BubbleEffect42, BubbleEffect43, BubbleEffect44, BubbleEffect45, BubbleEffect46, BubbleEffect47,
			BubbleEffect48, BubbleEffect49, BubbleEffect50, BubbleEffect51, BubbleEffect52, BubbleEffect53, BubbleEffect54, BubbleEffect55,
			BubbleEffect56, BubbleEffect57, BubbleEffect58, BubbleEffect59, BubbleEffect60
		}
	};
	_castingEffect = AnimatableObject(0.0f, 2.0f, 0.05f, 0.0f, 1.0f, animationCasting);
	_castingEffect.setMillisecondsPerFrame(70);
}

void Player::setMoveInput(bool left, bool right, bool jump)
{
	_inputLeft = left;
	_inputRight = right;
	_inputLJump = jump;

	Body* body = Unit::getBody(_bodyId);
	b2Vec2 v = body->getVelocity();
	
}

void Player::onContactBegin(Unit* otherUnit, int otherBodyId, int otherFixtureId, int thisBodyId, int thisFixtureId, b2Vec2 contactNormal)
{
	if (otherUnit->FlagIsStatic) {
		CollisionManager::addCollision(otherUnit->getId(), otherBodyId, otherFixtureId, contactNormal);
	}
	else {
		CombatObject::takeDamage(5);
	}
}

void Player::onContactEnd(Unit* otherUnit, int otherBodyId, int otherFixtureId, int thisBodyId, int thisFixtureId)
{
	if (otherUnit->FlagIsStatic) {
		CollisionManager::removeCollision(otherUnit->getId(), otherBodyId, otherFixtureId);
	}
}

std::list<DrawInstructions*>* Player::getDrawInstructions()
{
	auto v = Unit::getDrawInstructions();

	if (_isCasting) {
		Body* body = Unit::getBody(_bodyId);
		Fixture* fixture = body->getFixture(_fixtureId);
		b2Vec2 worldPos = body->getBodyPtr()->GetWorldPoint(b2Vec2(fixture->getX(), fixture->getY()));
		DrawInstructionsImage* diBubble = new DrawInstructionsImage(
			DrawInstructionsPoint(_castingEffect.getX() + worldPos.x, _castingEffect.getY() + worldPos.y),
			_castingEffect.getScale(),
			_castingEffect.getAngle(),
			GraphicsMap::idMap[_castingEffect.getAnimationImage()],
			_castingEffect.getAlpha(),
			false
		);
		v->push_front(diBubble);
	}

	for (auto& di : *v) {
		if (di->type == DrawInstructions::DrawType::image) {
			if (_facingLeft) {
				auto dii = static_cast<DrawInstructionsImage*>(di);
				dii->flipHorizontally = true;
			}
		}
	}

	b2Vec2 pos = Unit::getBody(0)->getPos();
	auto diList = CombatObject::getDrawInstructionsForHealthBar(pos.x, pos.y + 3.5);
	v->insert(v->end(), diList->begin(), diList->end());
	delete diList;

	//v->push_back(new DrawInstructionsText(DrawInstructionsPoint(getPos().x - 1.0f, getPos().y + 1.8f), L"Abraham", L"Georgia", 0.5f, 0.0f, 0xFFFFFF, 1.0f));
	return v;
}

void Player::timeStepForGraphics(int microseconds)
{
	Unit::timeStepForGraphics(microseconds);

	Body* body = Unit::getBody(_bodyId);
	Fixture* fixture = body->getFixture(_fixtureId);
	if (_isAttacking) {
		if (_isCasting)
			fixture->chooseAnimation(_castingAnimationId);
		else
			fixture->chooseAnimation(_doneCastingAnimationId);
	} else {
		if (_isFalling) {
			fixture->chooseAnimation(_fallingAnimationId);
		} else if (_isRising) {
			fixture->chooseAnimation(_risingAnimationId);
		} else {
			if (_isIdle)
				fixture->chooseAnimation(_idleAnimationId);
			else
				fixture->chooseAnimation(_runningAnimationId);
		}
	}

	_castingEffect.timeStepForGraphics(microseconds);
}

void Player::doEvent(std::wstring event)
{
	if (event == L"set black hole shot pos") {
		if (_projectileId != -1) {
			b2Vec2 pos = Unit::getBody(_bodyId)->getPos();
			float deltaX = 2;
			if (_facingLeft) {
				deltaX *= -1;
			}
			MoveUnitEffect* muEffect = new MoveUnitEffect(_projectileId, pos.x + deltaX, pos.y);
			Unit::addEffectToQueue(muEffect);
		}
	} else if (event == L"release projectile") {
		_isCasting = false;
		_doneReleasingProjectileTimerId = TimableObject::addTimer(_currentCombatAttack->getRecoveryTime(), L"done releasing projectile");
		TimableObject::destroyTimerAfterNumTimes(_doneReleasingProjectileTimerId, 1);

		// Create summon projectile effect
		b2Vec2 pos = Unit::getBody(_bodyId)->getPos();
		float deltaX = 2.2f;
		if (_facingLeft) {
			deltaX *= -1;
		}
		b2Vec2 projectilePos = b2Vec2(pos.x + deltaX, pos.y);
		b2Vec2 projectileGravity = _currentCombatAttack->getProjectileGravity();
		b2Vec2 projectInitialVelocity = _currentCombatAttack->getProjectileInitialVelocity();
		if (_facingLeft) {
			projectileGravity.x *= -1;
			projectInitialVelocity.x *= -1;
		}
		SummonProjectileEffect* effect = new SummonProjectileEffect(
			projectilePos, _currentCombatAttack->getUnitType(), projectileGravity, projectInitialVelocity
		);
		Unit::addEffectToQueue(effect);
	} else if (event == L"done releasing projectile") {
		_isAttacking = false;
	} else if (event == L"slow vx") {
		if (!_modifiedVelocity) {
			Body* body = Unit::getBody(_bodyId);
			b2Vec2 v = body->getVelocity();
			if (_onGround) {
				body->setVelocity(v.x * 0.7f, v.y);
			} else {
				body->setVelocity(v.x * 0.7f, v.y);
			}
		}
	} else if (event == L"done attacking") {
		_isAttacking = false;
	}
}

void Player::onDestroy(std::wstring event)
{
	if (event == L"set projectile pos") {
		_projectileId = -1;
		_setProjectilePosTimerId = -1;
	} else if (event == L"release projectile") {
		_releaseProjectileTimerId = -1;
	} else if (event == L"done releasing projectile") {
		_doneReleasingProjectileTimerId = -1;
	}
}

void Player::timeElapsed(long long microseconds)
{
	TimableObject::timeElapsed(microseconds);

	Body* body = Unit::getBody(_bodyId);
	Fixture* fixture = body->getFixture(_fixtureId);
	auto v = body->getVelocity();

	bool prevOnGround = _onGround;
	_onGround = CollisionManager::isOnFloor();
	bool isOnLeftWall = CollisionManager::isOnLeftWall();
	bool isOnRightWall = CollisionManager::isOnRightWall();
	bool isOnCeiling = CollisionManager::isOnCeiling();

	// Reset jump after we hit the ground.
	if (!prevOnGround && _onGround) {
		_jumpCount = 0;
	}

	_modifiedVelocity = false;
	if (isOnRightWall) {
		_inputRight = false;
	}
	if (isOnLeftWall) {
		_inputLeft = false;
	}
	if (_inputLJump) {
		if (_jumpCount < _maxJumpCount) {
			_jumpCount++;
			body->setVerticalVelocity(10.0f);
			v = body->getVelocity();
		}
	}
	int xDirection = (int)_inputRight - (int)_inputLeft;
	if (_onGround) {
		if (!_isAttacking) {
			if (xDirection != 0) {
				body->setHorizontalVelocity(xDirection * 10);
				v = body->getVelocity();
			}
		}
	} else {
		if (!_isAttacking && xDirection != 0) {
			if (!(v.x > 12.0f && xDirection > 0) && !(v.x < -12.0f && xDirection < 0)) {
				v.x += xDirection * 1.0f;
				if (v.x > 12.0f) {
					v.x = 12.0f;
				} else if (v.x < -12.0f) {
					v.x = -12.0f;
				}
			}
			_modifiedVelocity = true;
		} else {
			if (v.x > 0.1f) {
				v.x -= 0.1f;
			} else if (v.x < -0.1f) {
				v.x += 0.1f;
			} else {
				v.x *= 0.5f;
			}
			_modifiedVelocity = true;
		}
	}

	if (_modifiedVelocity) {
		body->setVelocity(v.x, v.y);
	}

	if (v.x < -0.0001)
		_facingLeft = true;
	else if (v.x > 0.0001)
		_facingLeft = false;
	_isIdle = abs(v.x) < 0.0001 && abs(v.y) < 0.0001;
	_isFalling = v.y < -0.0001;
	_isRising = v.y > 0.0001;
}

void Player::attack(std::shared_ptr<CombatAttack> combatAttack)
{
	if (_isAttacking) {
		return;
	}
	
	_isAttacking = true;
	_isCasting = true;
	Unit::getBody(_bodyId)->getFixture(_fixtureId)->chooseAnimation(_castingAnimationId);

	_currentCombatAttack = combatAttack;

	// Create set-black-hole-shot-pos timer
	/*if (TimableObject::timerExists(_setBlackHoleShotPosTimerId)) {
		TimableObject::destroyTimer(_setBlackHoleShotPosTimerId);
	}
	_setBlackHoleShotPosTimerId = TimableObject::addTimer(0.01f, L"set black hole shot pos");
	TimableObject::destroyTimerAfterAmountTime(_setBlackHoleShotPosTimerId, 1.0f);*/

	// Create release-black-hole-shot timer
	if (TimableObject::timerExists(_releaseProjectileTimerId)) {
		TimableObject::destroyTimer(_releaseProjectileTimerId);
	}
	_releaseProjectileTimerId = TimableObject::addTimer(_currentCombatAttack->getCastTime(), L"release projectile");
	TimableObject::destroyTimerAfterNumTimes(_releaseProjectileTimerId, 1);
}

void Player::butterflyAttack()
{
	if (_isAttacking) {
		return;
	}

	_isAttacking = false;

	// Create summon butterfly effect
	b2Vec2 pos = Unit::getBody(_bodyId)->getPos();
	float deltaX = 2;
	if (_facingLeft) {
		deltaX *= -1;
	}
	float direction = (_facingLeft * 2 - 1) * -1;
	SummonButterflyEffect* effect = new SummonButterflyEffect(pos.x + deltaX, pos.y, direction);
	Unit::addEffectToQueue(effect);

	//int timerId = TimableObject::addTimer(1.0f, L"done attacking");
	//TimableObject::destroyTimerAfterNumTimes(timerId, 1);
}

void Player::setBlackHoleShotId(int id)
{
	_projectileId = id;
}

void Player::setMoonJumpSwitch(bool moonJump)
{
	_moonJumpSwitch = moonJump;
}

void Player::onCurrentHpChanged()
{

}