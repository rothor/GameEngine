#pragma once
#include "GraphicsMap.h"
#include <vector>

class AnimatableObject
{
public:
	AnimatableObject();
	AnimatableObject(float pAnimationX, float pAnimationY, float pAnimationScale,
		float pAnimationAngle, float pAnimationAlpha, std::vector<std::vector<GraphicsEnum>> pAnimation);
	GraphicsEnum getAnimationImage();
	void setAnimations(std::vector<std::vector<GraphicsEnum>> animationVector);
	void timeStepForGraphics(int microseconds);
	void chooseAnimation(int animationIndex);
	// Call without passing an argument for *millisecondsPerFrame* to set it to the default of 167.
	void setMillisecondsPerFrame(int millisecondsPerFrame = 167);
	bool hasAnimation();
	bool shouldDraw();
	void setAlpha(float alpha);
	void setScale(float scale);
	float getX();
	float getY();
	float getScale();
	float getAngle();
	float getAlpha();
	void setAnimationAngleFixed(bool animationAngleIsFixed);
	bool getAnimationAngleIsFixed();

protected:
	long long _elapsedMicroseconds{ 0 };
	std::vector<std::vector<GraphicsEnum>> _animationMap{};
	int _chosenAnimation{ 0 };
	int _currentFrame{ 0 };
	int _millisecondsPerFrame{ 167 };

	float _animationX;
	float _animationY;
	float _animationScale;
	float _animationAngle;
	float _animationAlpha;
	bool _animationAngleIsFixed;
};

