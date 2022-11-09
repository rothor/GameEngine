#pragma once
#include "Unit.h"
#include "CombatObject.h"
#include "Effects/SummonBlackHoleShotEffect.h"
#include "Effects/SummonButterflyEffect.h"
#include "Effects/SetUnitAlphaEffect.h"
#include "Effects/LaunchPendingProjectileEffect.h"
#include "Effects/MoveUnitEffect.h"
#include "Effects/SummonProjectileEffect.h"
#include "Attack/CombatAttack.h"
#include <memory>

class Player : public Unit, public CombatObject
{
public:
	Player(b2World* world, float x, float y, int id);
	void setMoveInput(bool left, bool right, bool jump);
	virtual void onContactBegin(Unit* otherUnit, int otherBodyId, int otherFixtureId, int thisBodyId, int thisFixtureId, b2Vec2 contactNormal) override;
	virtual void onContactEnd(Unit* otherUnit, int otherBodyId, int otherFixtureId, int thisBodyId, int thisFixtureId) override;
	virtual std::list<DrawInstructions*>* getDrawInstructions() override;
	virtual void timeStepForGraphics(int microseconds) override;
	virtual void doEvent(std::wstring event) override;
	virtual void onDestroy(std::wstring event) override;
	virtual void timeElapsed(long long microseconds) override;
	void attack(std::shared_ptr<CombatAttack> combatAttack);
	void butterflyAttack();
	void setBlackHoleShotId(int id);
	void setMoonJumpSwitch(bool moonJump);
	virtual void onCurrentHpChanged() override;

protected:
	int _bodyId{ -1 };
	int _fixtureId{ -1 };
	int _releaseProjectileTimerId{ -1 };
	int _doneReleasingProjectileTimerId{ -1 };
	int _setProjectilePosTimerId{ -1 };
	int _slowVXTimerId{ -1 };
	int _projectileId{ -1 };
	bool _moonJumpSwitch{ false };
	bool _inputLeft{ false };
	bool _inputRight{ false };
	bool _inputLJump{ false };
	AnimatableObject _castingEffect;
	int _jumpCount{ 0 };
	int _maxJumpCount{ 1000 };
	std::shared_ptr<CombatAttack> _currentCombatAttack;


	// Unit state
	bool _facingLeft{ false };
	bool _isIdle{ true };
	bool _isRising{ false };
	bool _isFalling{ false };
	bool _isAttacking{ false };
	bool _onGround{ true };
	bool _modifiedVelocity{ false };
	bool _isCasting{ false };

	// Animation IDs
	int _idleAnimationId{ -1 };
	int _runningAnimationId{ -1 };
	int _risingAnimationId{ -1 };
	int _fallingAnimationId{ -1 };
	int _castingAnimationId{ -1 };
	int _doneCastingAnimationId{ -1 };
};