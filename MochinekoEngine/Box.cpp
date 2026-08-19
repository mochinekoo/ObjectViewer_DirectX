#include "Box.h"
#include <d3d11.h>
#include "DX3DManager.h"
#include "Camera.h"
#include "ShaderManager.h"
#include "CameraManager.h"
#include "ImGUI/imgui.h"

using namespace DirectX;
using namespace DX3DManager;

void Box::Init() {
	InitVertexBuffer();
	InitConstantBuffer();
}

void Box::Update() {
	Camera* currentCamera = CameraManager::GetCurrentCamera();
	XMMATRIX world = transform_.GetWorldMatrix();
	XMMATRIX view = XMMatrixIdentity();
	XMMATRIX projection = XMMatrixOrthographicOffCenterLH(
		0.0f, 1280.0f,
		720.0f, 0.0f,
		0.0f, 100.0f
	);

	ConstantBuffer constantBuffer = {};
	constantBuffer.wvpMatrix_ = XMMatrixTranspose(world * view * projection);
	constantBuffer.diffuse_ = color_;
	constantBuffer.ambient_ = {};
	constantBuffer.hasTexture_ = FALSE;
	GetDeviceContext()->UpdateSubresource(constantBuffer_, 0, nullptr, &constantBuffer, 0, 0);
}

void Box::Draw() {
	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	ShaderManager::SetPixelShader("MochinekoEngine/PixelShader.hlsl");
	ShaderManager::SetVertexShader("MochinekoEngine/VertexShader.hlsl");

	ID3D11ShaderResourceView* nullSrv = { nullptr };
	GetDeviceContext()->PSSetShaderResources(0, 1, &nullSrv);
	GetDeviceContext()->IASetVertexBuffers(0, 1, &vertexBuffer_, &stride, &offset);
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	GetDeviceContext()->RSSetState(GetRasterizerState());
	GetDeviceContext()->VSSetConstantBuffers(0, 1, &constantBuffer_);
	GetDeviceContext()->PSSetConstantBuffers(0, 1, &constantBuffer_);

	GetDeviceContext()->Draw(6, 0);

	GetDeviceContext()->RSSetState(nullptr);
}

void Box::DrawImGUI() {
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
}

void Box::Release()
{
}


void Box::InitVertexBuffer() {
	vertexList_.resize(6);
	vertexList_[0] = { {0.0f, 0.0f, 0.0f}, {}, color_, {0.0f, 0.0f} };
	vertexList_[1] = { {0.0f, (float) height_, 0.0f}, {}, color_, {0.0f, 0.0f} };
	vertexList_[2] = { { (float)width_, 0.0f, 0.0f}, {}, color_, {0.0f, 0.0f} };
	vertexList_[3] = { { (float)width_, 0.0f, 0.0f}, {}, color_, {0.0f, 0.0f} };
	vertexList_[4] = { { (float) width_,  (float)height_, 0.0f}, {}, color_, {0.0f, 0.0f} };
	vertexList_[5] = { {0.0f,  (float)height_, 0.0f}, {}, color_, {0.0f, 0.0f} };

	D3D11_BUFFER_DESC vertexDesc = {};
	vertexDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexDesc.ByteWidth = sizeof(Vertex) * vertexList_.size();
	vertexDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA vertexData = {};
	vertexData.pSysMem = vertexList_.data();

	GetDevice()->CreateBuffer(&vertexDesc, &vertexData, &vertexBuffer_);
}

void Box::InitConstantBuffer() {
	D3D11_BUFFER_DESC constantDesc = {};
	constantDesc.Usage = D3D11_USAGE_DEFAULT;
	constantDesc.ByteWidth = sizeof(ConstantBuffer);
	constantDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	GetDevice()->CreateBuffer(&constantDesc, nullptr, &constantBuffer_);
}