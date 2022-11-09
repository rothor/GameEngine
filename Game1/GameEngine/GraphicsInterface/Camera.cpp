#include "Camera.h"
#include <math.h>

Camera::Camera() :
	x(0),
	y(0),
	_zoomNotch(0),
	_minZoom(-10),
	_maxZoom(10)
{

}

void Camera::move(bool moveLeft, bool moveRight, bool moveUp, bool moveDown)
{
	const float CAMERA_SPEED = 2.5f;
	x += (moveRight - moveLeft) * CAMERA_SPEED / getZoomFactor();
	y += (moveUp - moveDown) * CAMERA_SPEED / getZoomFactor();
}

void Camera::moveTo(float pX, float pY)
{
	x = pX;
	y = pY;
}

void Camera::zoomOut()
{
	_zoomNotch--;
	if (_zoomNotch < _minZoom)
		_zoomNotch = _minZoom;
}

void Camera::zoomIn()
{
	_zoomNotch++;
	if (_zoomNotch > _maxZoom)
		_zoomNotch = _maxZoom;
}

float Camera::getZoomFactor()
{
	const float C_ZOOM_BASE = 2.0f;
	return powf(C_ZOOM_BASE, _zoomNotch);
}