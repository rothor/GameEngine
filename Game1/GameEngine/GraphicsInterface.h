#pragma once
#include <d2d1.h>
#include <d2d1helper.h>
#include <dwrite.h>
#include <wincodec.h>
#include <memory>
#include <map>
#include <string>
#pragma comment(lib, "d2d1")
#pragma comment(lib, "Windowscodecs")
#pragma comment(lib, "Dwrite")
#include "DrawInstructions.h"
#include "GraphicsInterface/Camera.h"
#include "DrawInstructionsPolygon.h"
#include "DrawInstructionsImage.h"
#include "DrawInstructionsEllipse.h"
#include "DrawInstructionsText.h"

class GraphicsInterface
{
public:
	GraphicsInterface(HWND, int drawWidth, int drawHeight);
	~GraphicsInterface();

	void initialize(int newDrawWidth, int newDrawHeight);
	void sizeChange(int newDrawWidth, int newDrawHeight);
	void drawRect(double x, double y, double width, double height, int color, double alpha = 1);
	void drawRectScreen(double x, double y, double width, double height, int color, double alpha = 1);
	void drawEllipse(double x, double y, double width, double height, int color, double alpha = 1);
	void draw(DrawInstructions* di);
	void drawPolygon(DrawInstructionsPolygon* di);
	void drawImage(DrawInstructionsImage* di);
	void drawEllipse(DrawInstructionsEllipse* di);
	void drawText(DrawInstructionsText* di);
	//void drawLine(DrawInstructions& di);
public:
	void fillScreen(int color, double alpha = 1);
	void copyBuffer();
	void getDimensions(int& widthIn, int& heightIn);

	int getBmpFrameCount(std::wstring file);
	int createBmpFromFile(LPCWSTR fileName, int frame = 0);
	void releaseBmp(int bmpIndex);
	void drawBmp(int index, int x, int y, int width, int height, float alpha = 1);

	void drawText(std::wstring str, std::wstring font, float size, float x, float y, float width, float height, int color, float alpha = 1);

	// Camera
	void cameraMove(bool moveLeft, bool moveRight, bool moveUp, bool moveDown);
	void cameraMoveTo(float x, float y);
	void cameraZoomOut();
	void cameraZoomIn();
	float cameraGetZoomFactor();
	void cameraSetZoomNotch(float notch);
	void setWorldDraw();
	void setScreenDraw();
	DrawInstructionsPoint convertScreenToWorldPoint(DrawInstructionsPoint dip);
	bool windowSizeChanged();
	void changeWindowSize();

private:
	// Camera
	double _cameraX;
	double _cameraY;
	double _cameraZoomNotch;
	double _cameraMinZoom;
	double _cameraMaxZoom;

private:
	HWND hwnd;
	int drawWidth;
	int drawHeight;
	std::map<int, ID2D1Bitmap*> _bmpMap{};
	int _bmpIdIncrementer = 1;
	bool _windowSizeChanged{ false };
	int _newWindowWidth{ 0 };
	int _newWindowHeight{ 0 };

	ID2D1Factory* m_pDirect2dFactory;
	ID2D1HwndRenderTarget* m_pRenderTarget;
	ID2D1SolidColorBrush* m_pBrush;
	IWICImagingFactory* m_pWicFactory;
	//ID2D1BitmapBrush* m_pBmpBrush;
};