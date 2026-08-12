#include "Image.h"
#include "ShaderManager.h"
#include "DX3DManager.h"
#include <DirectXMath.h>
#include "ImGUI/imgui.h"

using namespace DX3DManager;
using namespace DirectX;

void Image::Init() {
	texture_->Init();
}

void Image::Update() {
	XMMATRIX world = transform_.GetWorldMatrix();
	XMMATRIX view = XMMatrixIdentity();
	XMMATRIX projection = XMMatrixOrthographicOffCenterLH(
		0.0f, 1280.0f,
		720.0f, 0.0f,
		0.0f, 100.0f
	);

	ConstantBuffer constantBuffer = {};
	constantBuffer.wvpMatrix_ = XMMatrixTranspose(world * view * projection);
	constantBuffer.diffuse_ = {};
	constantBuffer.ambient_ = {};
	constantBuffer.hasTexture_ = TRUE;
	GetDeviceContext()->UpdateSubresource(texture_->GetConstanctBuffer(), 0, nullptr, &constantBuffer, 0, 0);
}

void Image::Draw() {
	auto vertexBuffer = texture_->GetVertexBuffer();
	auto constantBuffer = texture_->GetConstanctBuffer();
	auto srv = texture_->GetShaderResourceView();
	auto samplerState = texture_->GetSamplerState();

	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	ShaderManager::SetPixelShader("PixelShader.hlsl");
	ShaderManager::SetVertexShader("VertexShader.hlsl");
	GetDeviceContext()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	GetDeviceContext()->RSSetState(GetRasterizerState());
	GetDeviceContext()->PSSetShaderResources(0, 1, &srv);
	GetDeviceContext()->PSSetSamplers(0, 1, &samplerState);

	GetDeviceContext()->VSSetConstantBuffers(0, 1, &constantBuffer);
	GetDeviceContext()->PSSetConstantBuffers(0, 1, &constantBuffer);

	GetDeviceContext()->Draw(6, 0);

	std::string title = GetName() + "(" + GetTag() + ")";
	ImGui::Begin(title.c_str(), nullptr, ImGuiWindowFlags_NoDocking);

	if (ImGui::BeginTabBar("Tab")) {
		if (ImGui::BeginTabItem("ObjectInfo")) {

			ImGui::SliderFloat("Location X", &transform_.location_.x, -10.0f, 1280.0f);
			ImGui::SliderFloat("Location Y", &transform_.location_.y, -10.0f, 720.0f);
			ImGui::SliderFloat("Location Z", &transform_.location_.z, -10.0f, 100.0f);
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
	}
	ImGui::EndTabBar();

	ImGui::End();



	GetDeviceContext()->RSSetState(nullptr);
}

void Image::Release() {
}
