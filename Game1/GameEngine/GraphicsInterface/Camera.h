#pragma once


class Camera
{
public:
	Camera();
	void move(bool moveLeft, bool moveRight, bool moveUp, bool moveDown);
	void moveTo(float x, float y);
	void zoomOut();
	void zoomIn();
	float getZoomFactor();

public:
	float x;
	float y;

protected:
	float _vH;
	float _vV;
	int _zoomNotch;
	int _minZoom;
	int _maxZoom;
};