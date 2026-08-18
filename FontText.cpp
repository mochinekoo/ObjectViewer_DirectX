#define _CRT_SECURE_NO_WARNINGS

#include "FontText.h"
#include "DX2DManager.h"
#include "ImGUI/imgui.h"
#include "ImGUI/imgui_stdlib.h"

void FontText::Init() {

	DX2DManager::GetWriteFactory()->CreateTextFormat(
		fontName_.c_str(),
		nullptr,
		DWRITE_FONT_WEIGHT_NORMAL,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		fontSize_,
		L"",
		&fontData_.textFormat_
	);

	fontData_.textFormat_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
	fontData_.textFormat_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);

	D2D1_COLOR_F d2dColor = D2D1::ColorF(0.0, 0.0, 0.0, 1.0);
	d2dColor.r = color_.r_;
	d2dColor.g = color_.g_;
	d2dColor.b = color_.b_;
	d2dColor.a = color_.a_;

	DX2DManager::GetRenderTarget()->CreateSolidColorBrush(d2dColor, &fontData_.brush_);
}

void FontText::Update()
{
}

void FontText::Draw() {
	D2D1_SIZE_F size = DX2DManager::GetRenderTarget()->GetSize();
	DX2DManager::GetRenderTarget()->BeginDraw();
	D2D1_MATRIX_3X2_F transform = D2D1::Matrix3x2F::Scale(transform_.scale_.x, transform_.scale_.y) * D2D1::Matrix3x2F::Rotation(transform_.rotation_.y);
	DX2DManager::GetRenderTarget()->SetTransform(transform);

	D2D1_COLOR_F d2dColor = D2D1::ColorF(0.0f, 0.0f, 0.0f, 1.0f);
	d2dColor.r = color_.r_;
	d2dColor.g = color_.g_;
	d2dColor.b = color_.b_;
	d2dColor.a = color_.a_;
	fontData_.brush_->SetColor(d2dColor);

	DX2DManager::GetRenderTarget()->DrawText(
		text_.c_str(),
		text_.size(),
		fontData_.textFormat_,
		D2D1::RectF(transform_.location_.x, transform_.location_.y, size.width, size.height),
		fontData_.brush_
	);

	DX2DManager::GetRenderTarget()->EndDraw();
}

void FontText::DrawImGUI() {
	ImGui::Begin("FontText", nullptr, ImGuiWindowFlags_NoDocking);

	if (ImGui::BeginTabBar("Tab")) {
		if (ImGui::BeginTabItem("ObjectInfo")) {
			ImGui::SliderFloat("Location X", &transform_.location_.x, -10.0f, 1280.0f);
			ImGui::SliderFloat("Location Y", &transform_.location_.y, -10.0f, 720.0f);
			ImGui::SliderFloat("Location Z", &transform_.location_.z, -10.0f, 10.0f);
			ImGui::SliderFloat("Velocity X", &transform_.velocity_.x, -10.0f, 10.0f);
			ImGui::SliderFloat("Velocity Y", &transform_.velocity_.y, -10.0f, 10.0f);
			ImGui::SliderFloat("Velocity Z", &transform_.velocity_.z, -10.0f, 10.0f);
			ImGui::SliderFloat("Rotation X", &transform_.rotation_.x, -DirectX::XM_PI, DirectX::XM_PI);
			ImGui::SliderFloat("Rotation Y", &transform_.rotation_.y, -DirectX::XM_PI, DirectX::XM_PI);
			ImGui::SliderFloat("Rotation Z", &transform_.rotation_.z, -DirectX::XM_PI, DirectX::XM_PI);
			ImGui::SliderFloat("Scale X", &transform_.scale_.x, 0.1f, 10.0f);
			ImGui::SliderFloat("Scale Y", &transform_.scale_.y, 0.1f, 10.0f);
			ImGui::SliderFloat("Scale Z", &transform_.scale_.z, 0.1f, 10.0f);

			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("FontData")) {

			static std::string utf8Buffer;
			if (utf8Buffer.empty()) {
				utf8Buffer = WStringToUtf8(text_);
			}

			if (ImGui::InputText("Text", &utf8Buffer)) {
				SetText(Utf8ToWString(utf8Buffer));
			}

			static float color[4] = {
				color_.r_,
				color_.g_,
				color_.b_,
				color_.a_
			};

			ImGui::ColorPicker4("Color", color);

			SetColor({
				color[0],
				color[1],
				color[2],
				color[3]
				});

			ImGui::EndTabItem();
		}
	}
	ImGui::EndTabBar();

	ImGui::End();
}

void FontText::Release()
{
}


