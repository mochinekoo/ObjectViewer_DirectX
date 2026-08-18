#pragma once
#include <string>
#include <d2d1.h>
#include <dwrite.h>
#include <DirectXMath.h>
#include "DX3DManager.h"

#pragma comment(lib, "d2d1.lib") 
#pragma comment(lib, "dwrite.lib")

struct FontData {
	std::string fontName_;
	int size_;
	IDWriteTextFormat* textFormat_ = nullptr;
	ID2D1SolidColorBrush* brush_ = nullptr;

	static bool Equal(const FontData& fontA, const FontData& fontB) {
		bool equalFontName = (fontA.fontName_ == fontB.fontName_);
		bool equalFontSize = (fontA.size_ == fontB.size_);

		return equalFontName && equalFontSize;
	}

	bool operator==(const FontData& font) const {
		return Equal(*this, font);
	}
};

namespace DX2DManager {
	inline FontData defaultFont_ = FontData();

	void Init();
	void DrawFontText(const int x, const int y, const std::string& text, const Color& color);
	ID2D1RenderTarget* GetRenderTarget();
	IDWriteFactory* GetWriteFactory();
}
