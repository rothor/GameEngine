#include "GraphicsInterface.h"
#include "D2d1_1.h"
#include "D2d1_3.h"
#include "Wincodec.h"
#include <dwrite_1.h>

template<class Interface>
inline void SafeRelease(Interface **ppInterfaceToRelease)
{
	if (*ppInterfaceToRelease != NULL) {
		(*ppInterfaceToRelease)->Release();
		(*ppInterfaceToRelease) = NULL;
	}
}

int intFromRGB(int red, int green, int blue)
{
	return red * 0x10000 + green * 0x100 + blue;
}

class RGBColor
{
public:
	RGBColor(int pColor)
	{
		color = pColor;
		red = (pColor >> 16) & 0xFF;
		green = (pColor >> 8) & 0xFF;
		blue = pColor & 0xFF;
	}

public:
	int color;
	int red;
	int green;
	int blue;
};

GraphicsInterface::GraphicsInterface(HWND p_hwnd, int drawWidthIn, int drawHeightIn) :
	m_pDirect2dFactory(0),
	m_pRenderTarget(0),
	m_pBrush(0),
	_cameraZoomNotch(0),
	_cameraMinZoom(-10),
	_cameraMaxZoom(10),
	_cameraX(0),
	_cameraY(0)
{
	hwnd = p_hwnd;

	// Create Direct2D factory
	D2D1CreateFactory(D2D1_FACTORY_TYPE_MULTI_THREADED, &m_pDirect2dFactory);

	// Create imaging factory
	m_pWicFactory = NULL;
	HRESULT hr = CoCreateInstance(
		CLSID_WICImagingFactory,
		NULL,
		CLSCTX_INPROC_SERVER,
		IID_IWICImagingFactory,
		(LPVOID*)&m_pWicFactory
	);

	ID2D1Bitmap* pBitmap = nullptr;
	_bmpMap[0] = pBitmap;

	initialize(drawWidthIn, drawHeightIn);
}

GraphicsInterface::~GraphicsInterface()
{
	SafeRelease(&m_pWicFactory);
	SafeRelease(&m_pBrush);
	SafeRelease(&m_pRenderTarget);
	SafeRelease(&m_pDirect2dFactory);

	for (auto bmpPtr : _bmpMap) {
		delete bmpPtr.second;
		_bmpMap.erase(bmpPtr.first);
	}
}

void GraphicsInterface::initialize(int newDrawWidth, int newDrawHeight)
{
	SafeRelease(&m_pBrush);
	SafeRelease(&m_pRenderTarget);
	
	drawWidth = newDrawWidth;
	drawHeight = newDrawHeight;

	// Create a Direct2D render target.
	D2D1_SIZE_U size = D2D1::SizeU(drawWidth, drawHeight);
	m_pDirect2dFactory->CreateHwndRenderTarget(
		D2D1::RenderTargetProperties(),
		D2D1::HwndRenderTargetProperties(hwnd, size),
		&m_pRenderTarget
	);
	m_pRenderTarget->SetAntialiasMode(D2D1_ANTIALIAS_MODE::D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);

	// Create solid color brush
	m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(0x0, 0x0), &m_pBrush);

	m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
	m_pRenderTarget->EndDraw();
	m_pRenderTarget->BeginDraw();
}

void GraphicsInterface::sizeChange(int newDrawWidth, int newDrawHeight)
{
	_newWindowWidth = newDrawWidth;
	_newWindowHeight = newDrawHeight;
	_windowSizeChanged = true;
}

void GraphicsInterface::getDimensions(int& widthIn, int& heightIn)
{
	widthIn = drawWidth;
	heightIn = drawHeight;
}

void GraphicsInterface::drawRect(double x, double y, double width, double height, int color, double alpha)
{
	y = -y;
	height = -height;
	D2D1_RECT_F rectangle1 = D2D1::RectF(x - width / 2, y + height / 2, x + width / 2, y - height / 2);
	m_pBrush->SetColor(D2D1::ColorF(D2D1::ColorF(color, alpha)));
	m_pRenderTarget->FillRectangle(&rectangle1, m_pBrush);
}

void GraphicsInterface::drawRectScreen(double x, double y, double width, double height, int color, double alpha)
{
	y = drawHeight - y;
	height = -height;
	D2D1_MATRIX_3X2_F transform;
	m_pRenderTarget->GetTransform(&transform);
	m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
	D2D1_RECT_F rectangle1 = D2D1::RectF(x - width / 2, y + height / 2, x + width / 2, y - height / 2);
	m_pBrush->SetColor(D2D1::ColorF(D2D1::ColorF(color, alpha)));
	m_pRenderTarget->FillRectangle(&rectangle1, m_pBrush);
	m_pRenderTarget->SetTransform(transform);
}

/* The pointers you pass to this function will be deleted automatically.
 * Do not delete them once the function returns.
 */
void GraphicsInterface::draw(DrawInstructions* di)
{
	if (di->type == DrawInstructions::DrawType::polygon) {
		auto dip = static_cast<DrawInstructionsPolygon*>(di);
		drawPolygon(dip);
		delete dip;
	} else if (di->type == DrawInstructions::DrawType::image) {
		drawImage(static_cast<DrawInstructionsImage*>(di));
		delete di;
	} else if (di->type == DrawInstructions::DrawType::ellipse) {
		drawEllipse(static_cast<DrawInstructionsEllipse*>(di));
		delete di;
	} else if (di->type == DrawInstructions::DrawType::text) {
		drawText(static_cast<DrawInstructionsText*>(di));
		delete di;
	}
}

void GraphicsInterface::drawPolygon(DrawInstructionsPolygon* di)
{
	int vertexCount = di->vertexes->size();
	if (vertexCount < 3)
		return;

	ID2D1PathGeometry* geo;
	ID2D1GeometrySink* sink;
	D2D1_POINT_2F* points = new D2D1_POINT_2F[di->vertexes->size()];
	HRESULT hr;

	// create a direct2d pathGeometry
	hr = m_pDirect2dFactory->CreatePathGeometry(&geo);
	hr = geo->Open(&sink);
	sink->SetFillMode(D2D1_FILL_MODE::D2D1_FILL_MODE_ALTERNATE);

	// first point
	(*di->vertexes)[vertexCount - 1].y = -(*di->vertexes)[vertexCount - 1].y;
	sink->BeginFigure(D2D1::Point2F((*di->vertexes)[vertexCount - 1].x, (*di->vertexes)[vertexCount - 1].y), D2D1_FIGURE_BEGIN_FILLED);
	// middle points
	for (int i = 0; i < di->vertexes->size(); i++) {
		points[i].x = (*di->vertexes)[i].x;
		if (i != di->vertexes->size() - 1)
			(*di->vertexes)[i].y = -(*di->vertexes)[i].y;
		points[i].y = (*di->vertexes)[i].y;
	}
	sink->AddLines(points, vertexCount);
	// close it
	sink->EndFigure(D2D1_FIGURE_END_CLOSED);
	sink->Close();
	SafeRelease(&sink);

	D2D1::ColorF dColor(di->color, di->alpha);
	m_pBrush->SetColor(dColor);
	m_pRenderTarget->FillGeometry(geo, m_pBrush);
	SafeRelease(&geo);

	delete points;
}

void GraphicsInterface::drawImage(DrawInstructionsImage* di)
{
	if (di->resourceId == 0)
		return;

	int flipX = -di->flipHorizontally * 2 + 1;
	D2D1_SIZE_U size = _bmpMap[di->resourceId]->GetPixelSize();
	float width = size.width * di->scale;
	float height = size.height * di->scale;
	di->center.y = -di->center.y;
	D2D1_RECT_F rectDest = D2D1::RectF(di->center.x - width / 2, di->center.y - height / 2, di->center.x + width / 2, di->center.y + height / 2);
	D2D1_RECT_F rectSource = D2D1::RectF(0, 0, _bmpMap[di->resourceId]->GetSize().width, _bmpMap[di->resourceId]->GetSize().height);
	D2D1::Matrix3x2F originalTransform;
	m_pRenderTarget->GetTransform(&originalTransform);
	D2D1::Matrix3x2F newTransform = D2D1::Matrix3x2F::Scale(flipX, 1, D2D1::Point2F(di->center.x, di->center.y)) * D2D1::Matrix3x2F::Rotation(di->angle, D2D1::Point2F(di->center.x, di->center.y)) * originalTransform;
	m_pRenderTarget->SetTransform(newTransform);

	m_pRenderTarget->DrawBitmap(
		_bmpMap[di->resourceId],
		rectDest,
		di->alpha,
		D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
		rectSource
	);
	m_pRenderTarget->SetTransform(originalTransform);
}

void GraphicsInterface::drawEllipse(DrawInstructionsEllipse* di)
{
	di->center.y = -di->center.y;
	di->height = -di->height;
	D2D1::Matrix3x2F originalTransform;
	m_pRenderTarget->GetTransform(&originalTransform);
	D2D1::Matrix3x2F newTransform = D2D1::Matrix3x2F::Rotation(di->angle, D2D1::Point2F(di->center.x, di->center.y)) * originalTransform;
	m_pRenderTarget->SetTransform(newTransform);

	D2D1_ELLIPSE ellipse = D2D1_ELLIPSE{ D2D1_POINT_2F{ di->center.x, di->center.y }, di->width, di->height };
	D2D1::ColorF dColor(di->color, di->alpha);
	m_pBrush->SetColor(dColor);
	m_pRenderTarget->FillEllipse(&ellipse, m_pBrush);

	m_pRenderTarget->SetTransform(originalTransform);
}

void GraphicsInterface::drawText(DrawInstructionsText* di)
{
	di->center.y = -di->center.y;
	//di->height = -di->height;
	D2D1::Matrix3x2F originalTransform;
	m_pRenderTarget->GetTransform(&originalTransform);
	D2D1::Matrix3x2F newTransform = D2D1::Matrix3x2F::Rotation(di->angle, D2D1::Point2F(di->center.x, di->center.y)) * originalTransform;
	m_pRenderTarget->SetTransform(newTransform);

	drawText(di->text, di->font, di->size, di->center.x, di->center.y, 200, 200, di->color, di->alpha);

	m_pRenderTarget->SetTransform(originalTransform);
}

/*void GraphicsInterface::drawLine(DrawInstructions& di)
{

}*/

void GraphicsInterface::fillScreen(int color, double alpha)
{
	D2D1_MATRIX_3X2_F transform;
	m_pRenderTarget->GetTransform(&transform);
	m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
	drawRect(drawWidth / 2, -drawHeight / 2, drawWidth, -drawHeight, color, alpha);
	m_pRenderTarget->SetTransform(transform);
}

void GraphicsInterface::copyBuffer()
{
	m_pRenderTarget->EndDraw(); // EndDraw() coppies the offscreen buffer to the window
	m_pRenderTarget->BeginDraw();
}

int GraphicsInterface::getBmpFrameCount(std::wstring file)
{
	HRESULT hr;
	IWICBitmapDecoder *pWicBmpDecoder = NULL;
	hr = m_pWicFactory->CreateDecoderFromFilename(
		file.c_str(),
		NULL,
		GENERIC_READ,
		WICDecodeOptions::WICDecodeMetadataCacheOnDemand,
		&pWicBmpDecoder
	);
	
	UINT frameCount;
	hr = pWicBmpDecoder->GetFrameCount(&frameCount);

	return frameCount;
}

int GraphicsInterface::createBmpFromFile(LPCWSTR fileName, int frame)
{
	HRESULT hr;
	IWICBitmapDecoder *pWicBmpDecoder = NULL;
	hr = m_pWicFactory->CreateDecoderFromFilename(
		fileName,
		NULL,
		GENERIC_READ,
		WICDecodeOptions::WICDecodeMetadataCacheOnDemand,
		&pWicBmpDecoder
	);

	IWICBitmapFrameDecode* pWicBmpFrameDecode = NULL;
	hr = pWicBmpDecoder->GetFrame(frame, &pWicBmpFrameDecode);

	IWICFormatConverter *pWicFmtConverter = NULL;
	hr = m_pWicFactory->CreateFormatConverter(&pWicFmtConverter);
	
	hr = pWicFmtConverter->Initialize(
		pWicBmpFrameDecode,
		GUID_WICPixelFormat32bppPBGRA,
		WICBitmapDitherTypeNone,
		NULL,
		0.0,
		WICBitmapPaletteTypeMedianCut
	);
	
	ID2D1Bitmap* pBitmap;
	hr = m_pRenderTarget->CreateBitmapFromWicBitmap(
		pWicFmtConverter,
		NULL,
		&pBitmap
	);
	
	SafeRelease(&pWicBmpDecoder);
	SafeRelease(&pWicBmpFrameDecode);
	SafeRelease(&pWicFmtConverter);

	int id = _bmpIdIncrementer;
	_bmpMap[id] = pBitmap;
	_bmpIdIncrementer++;
	return id;
}

void GraphicsInterface::releaseBmp(int bmpIndex)
{
	SafeRelease(&_bmpMap[bmpIndex]);
	_bmpMap.erase(bmpIndex);
}

void GraphicsInterface::drawBmp(int index, int x, int y, int width, int height, float alpha)
{
	if (index == 0)
		return;

	D2D1_RECT_F rectDest = D2D1::RectF(x, y, x + width, y + height);
	D2D1_RECT_F rectSource = D2D1::RectF(0, 0, _bmpMap[index]->GetSize().width, _bmpMap[index]->GetSize().height);
	m_pRenderTarget->DrawBitmap(
		_bmpMap[index],
		rectDest,
		alpha,
		D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
		rectSource
	);
}



void GraphicsInterface::drawText(std::wstring str, std::wstring font, float size, float x, float y, float width, float height, int color, float alpha)
{
	IDWriteFactory* pDWriteFactory = NULL;
	HRESULT hr = DWriteCreateFactory(
		DWRITE_FACTORY_TYPE_SHARED,
		__uuidof(pDWriteFactory),
		reinterpret_cast<IUnknown **>(&pDWriteFactory)
	);

	IDWriteTextFormat* pDwriteTextFormat = NULL;
	hr = pDWriteFactory->CreateTextFormat(
		font.c_str(),
		NULL,
		DWRITE_FONT_WEIGHT_NORMAL,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		size,
		L"",
		&pDwriteTextFormat
	);

	D2D1_RECT_F rectDest = D2D1::RectF(x, y, x + width, y + height);
	SafeRelease(&m_pBrush);
	m_pRenderTarget->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF(color, alpha)),
		&m_pBrush
	);

	m_pRenderTarget->DrawTextW(
		str.c_str(),
		str.length(),
		pDwriteTextFormat,
		&rectDest,
		m_pBrush
	);

	SafeRelease(&pDWriteFactory);
	SafeRelease(&pDwriteTextFormat);
}

void GraphicsInterface::cameraMove(bool moveLeft, bool moveRight, bool moveUp, bool moveDown)
{
	const float CAMERA_SPEED = 2.5f;
	float zoom = cameraGetZoomFactor();
	_cameraX -= (moveRight - moveLeft) * CAMERA_SPEED / zoom;
	_cameraY += (moveUp - moveDown) * CAMERA_SPEED / zoom;
}

void GraphicsInterface::cameraMoveTo(float pX, float pY)
{
	_cameraX = -pX;
	_cameraY = pY;
}

void GraphicsInterface::cameraZoomOut()
{
	_cameraZoomNotch--;
	if (_cameraZoomNotch < _cameraMinZoom)
		_cameraZoomNotch = _cameraMinZoom;
}

void GraphicsInterface::cameraZoomIn()
{
	_cameraZoomNotch++;
	if (_cameraZoomNotch > _cameraMaxZoom)
		_cameraZoomNotch = _cameraMaxZoom;
}

float GraphicsInterface::cameraGetZoomFactor()
{
	const float C_ZOOM_BASE = 2.0f;
	return powf(C_ZOOM_BASE, _cameraZoomNotch) * 32;
}

void GraphicsInterface::cameraSetZoomNotch(float notch)
{
	_cameraZoomNotch = notch;
}

void GraphicsInterface::setWorldDraw()
{
	float zoom = cameraGetZoomFactor();
	auto scale = D2D1::Matrix3x2F::Scale(zoom, zoom, D2D1::Point2F(0, 0));
	auto translation = D2D1::Matrix3x2F::Translation(
		_cameraX * zoom + drawWidth / 2.0f,
		_cameraY * zoom + drawHeight / 2.0f
	);
	m_pRenderTarget->SetTransform(scale * translation);
}

void GraphicsInterface::setScreenDraw()
{
	m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
}

DrawInstructionsPoint GraphicsInterface::convertScreenToWorldPoint(DrawInstructionsPoint dip)
{
	dip.y = drawHeight - dip.y;
	DrawInstructionsPoint dip2(
		-_cameraX - (drawWidth / 2 - dip.x) / cameraGetZoomFactor(),
		_cameraY - (drawHeight / 2 - dip.y) / cameraGetZoomFactor()
	);
	return dip2;
}

bool GraphicsInterface::windowSizeChanged()
{
	return _windowSizeChanged;
}

void GraphicsInterface::changeWindowSize()
{
	_windowSizeChanged = false;
	m_pRenderTarget->EndDraw();
	initialize(_newWindowWidth, _newWindowHeight);
}
