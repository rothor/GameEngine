#include "AnimatableObject.h"

AnimatableObject::AnimatableObject()
{
	// Do not use this.
}

AnimatableObject::AnimatableObject(float pAnimationX, float pAnimationY, float pAnimationScale, float pAnimationAngle, float pAnimationAlpha, std::vector<std::vector<GraphicsEnum>> pAnimation) :
	_animationX(pAnimationX),
	_animationY(pAnimationY),
	_animationScale(pAnimationScale),
	_animationAngle(pAnimationAngle),
	_animationAlpha(pAnimationAlpha),
	_animationMap(pAnimation)
{

}

GraphicsEnum AnimatableObject::getAnimationImage()
{
	return _animationMap[_chosenAnimation][_currentFrame];
}

void AnimatableObject::setAnimations(std::vector<std::vector<GraphicsEnum>> animationVector)
{
	_animationMap = animationVector;
}

void AnimatableObject::timeStepForGraphics(int microseconds)
{
	if (!hasAnimation())
		return;
	_elapsedMicroseconds += microseconds;
	int frameAdvance = _elapsedMicroseconds / ((long long)_millisecondsPerFrame * 1000);
	_currentFrame += frameAdvance;
	_currentFrame = _currentFrame % _animationMap[_chosenAnimation].size();
	_elapsedMicroseconds = _elapsedMicroseconds % ((long long)_millisecondsPerFrame * 1000);;
}

void AnimatableObject::chooseAnimation(int animationIndex)
{
	if (animationIndex == _chosenAnimation) {
		return;
	}
	_chosenAnimation = animationIndex;
	_currentFrame = 0;
}

void AnimatableObject::setMillisecondsPerFrame(int millisecondsPerFrame)
{
	_millisecondsPerFrame = millisecondsPerFrame;
}

bool AnimatableObject::hasAnimation()
{
	return _animationMap.size() > 0;
}

bool AnimatableObject::shouldDraw()
{
	return getAnimationImage() != GraphicsEnum::None;
}

void AnimatableObject::setAlpha(float alpha)
{
	_animationAlpha = alpha;
}

void AnimatableObject::setScale(float scale)
{
	_animationScale = scale;
}

float AnimatableObject::getX()
{
	return _animationX;
}

float AnimatableObject::getY()
{
	return _animationY;
}

float AnimatableObject::getScale()
{
	return _animationScale;
}

float AnimatableObject::getAngle()
{
	return _animationAlpha;
}

float AnimatableObject::getAlpha()
{
	return _animationAlpha;
}

void AnimatableObject::setAnimationAngleFixed(bool animationAngleIsFixed)
{
	_animationAngleIsFixed = animationAngleIsFixed;
}

bool AnimatableObject::getAnimationAngleIsFixed()
{
	return _animationAngleIsFixed;
}
