#include "DX2DManager.h"
#include "DX3DManager.h"
#include <string>
#include <vector>

using namespace DX3DManager;

namespace DX2DManager {
	std::vector<FontData> fontList_;
	ID2D1Factory* d2dFactory_ = nullptr;
	IDWriteFactory* writeFactory_ = nullptr;
	ID2D1RenderTarget* renderTarget_ = nullptr;
}

void DX2DManager::Init() {
	D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &d2dFactory_);
	DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(writeFactory_), reinterpret_cast<IUnknown**>(&writeFactory_));
	IDXGISurface* surface = nullptr;
	GetSwapChain()->GetBuffer(0, IID_PPV_ARGS(&surface));
	D2D1_RENDER_TARGET_PROPERTIES props = D2D1::RenderTargetProperties(
		D2D1_RENDER_TARGET_TYPE_DEFAULT,
		D2D1::PixelFormat(
			DXGI_FORMAT_UNKNOWN,
			D2D1_ALPHA_MODE_IGNORE
		)
	);
	d2dFactory_->CreateDxgiSurfaceRenderTarget(surface, &props, &renderTarget_);

	/// デフォルトのフォント
	const std::wstring DEFAULT_FONTNAME = L"メイリオ";
	const float DEFAULT_FONTSIZE = 30;
	D2D1_COLOR_F DEFAULT_COLOR = D2D1::ColorF(0.0, 0.0, 0.0, 1.0);

	writeFactory_->CreateTextFormat(
		DEFAULT_FONTNAME.c_str(),
		nullptr,
		DWRITE_FONT_WEIGHT_NORMAL,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		DEFAULT_FONTSIZE,
		L"",
		&defaultFont_.textFormat_
	);

	defaultFont_.textFormat_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
	defaultFont_.textFormat_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);

	renderTarget_->CreateSolidColorBrush(DEFAULT_COLOR, &defaultFont_.brush_);
}

void DX2DManager::DrawFontText(const int x, const int y, const std::string& text, const Color& color) {
	D2D1_SIZE_F size = renderTarget_->GetSize();
	renderTarget_->BeginDraw();
	D2D1_MATRIX_3X2_F transform_ = D2D1::Matrix3x2F::Scale(1.0, 1.0) * D2D1::Matrix3x2F::Translation(x, y);
	renderTarget_->SetTransform(transform_);

	D2D1_COLOR_F d2dColor = D2D1::ColorF(0.0, 0.0, 0.0, 1.0);
	d2dColor.r = color.r_;
	d2dColor.g = color.g_;
	d2dColor.b = color.b_;
	d2dColor.a = color.a_;
	defaultFont_.brush_->SetColor(d2dColor);

	std::wstring wText = std::wstring(text.begin(), text.end());
	renderTarget_->DrawTextW(
		wText.c_str(),
		text.size(),
		defaultFont_.textFormat_,
		D2D1::RectF(x, y, size.width, size.height),
		defaultFont_.brush_
	);

	renderTarget_->EndDraw();
}

ID2D1RenderTarget* DX2DManager::GetRenderTarget() {
	return renderTarget_;
}

IDWriteFactory* DX2DManager::GetWriteFactory() {
	return writeFactory_;
}
