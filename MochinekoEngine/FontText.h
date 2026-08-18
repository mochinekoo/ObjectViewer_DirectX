#pragma once
#include "BaseObject.h"
#include <string>
#include "DX2DManager.h"

/// <summary>
/// オブジェクトとして文字を描画する用（※基本的には、DX2DManager::DrawTextを使うことを推奨）。
/// </summary>
class FontText : public BaseObject{
private:
	std::wstring text_;
	Color color_;
	std::wstring fontName_;
	int fontSize_;
	FontData fontData_;
public:

	FontText(const std::wstring fontName, const int fontSize, const std::wstring& text, const Color& color) 
		: BaseObject("FontText") {
		fontName_ = fontName;
		fontSize_ = fontSize;
		text_ = text;
		//fontData_ = fontData;
		color_ = color;
	};

	~FontText() {
	};

	void Init() override;
	void Update() override;
	void Draw() override;
	void DrawImGUI() override;
	void Release() override;

	FontData GetFontData() const { return fontData_; }
	void SetFontData(const FontData& fontData) { fontData_ = fontData; }
	std::wstring GetText() const { return text_; }
	void SetText(const std::wstring& text) { text_ = text; }
	Color GetColor() const { return color_; }
	void SetColor(const Color& color) { color_ = color; }

	static std::string WStringToUtf8(const std::wstring& wText) {
		if (wText.empty()) return {};
		int size = WideCharToMultiByte(CP_UTF8, 0, wText.c_str(), (int)wText.size(), NULL, 0, NULL, NULL);
		std::string s(size, 0);
		WideCharToMultiByte(CP_UTF8, 0, wText.c_str(), (int)wText.size(), s.data(), size, NULL, NULL);
		return s;
	}

	static std::wstring Utf8ToWString(const std::string& cText) {
		if (cText.empty()) return {};
		int size = MultiByteToWideChar(CP_UTF8, 0, cText.c_str(), (int)cText.size(), NULL, 0);
		std::wstring w(size, 0);
		MultiByteToWideChar(CP_UTF8, 0, cText.c_str(), (int)cText.size(), w.data(), size);
		return w;
	}

	//(const int x, const int y, const std::string& text, const Color& color)
};

