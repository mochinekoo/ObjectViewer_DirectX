#pragma once
#include <d3dcompiler.h>
#include <d3d11.h>
#include <DirectXMath.h>
#include "Framework.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

struct Color {
	float r_ = 0.0f;
	float g_ = 0.0f;
	float b_ = 0.0f;
	float a_ = 1.0f;

	Color(const float r = 0.0f, const float g = 0.0f, const float b = 0.0f, const float a = 1.0f) {
		r_ = r;
		g_ = g;
		b_ = b;
		a_ = a;
	}

	static bool Equal(const Color& colorA, const Color& colorB) {
		return colorA.r_ == colorB.r_ && colorA.g_ == colorB.g_ && colorA.b_ == colorB.b_ && colorA.a_ == colorB.a_;
	}

	bool operator==(const Color& color) const {
		return Equal(*this, color);
	}

	static Color Red() { return Color(1.0f, 0.0f, 0.0f, 1.0f); }
	static Color Blue() { return Color(0.0f, 0.0f, 1.0f, 1.0f); }
	static Color Green() { return Color(0.0f, 1.0f, 0.0f, 1.0f); }
	static Color White() { return Color(1.0f, 1.0f, 1.0f, 1.0f); }
	static Color Black() { return Color(0.0f, 0.0f, 0.0f, 1.0f); }
};

struct Vertex {
	DirectX::XMFLOAT3 location_ = {};
	DirectX::XMFLOAT3 normal_ = {};
	DirectX::XMFLOAT4 color_ = {};
	DirectX::XMFLOAT2 uv_ = {};
};

struct ConstantBuffer {
	DirectX::XMMATRIX wvpMatrix_ = {};
	DirectX::XMFLOAT4 diffuse_ = {};
	DirectX::XMFLOAT4 ambient_ = {};
	DirectX::XMFLOAT4 specular_ = {};
	DirectX::XMFLOAT3 emission_ = {};
	float shininess_ = 0.0;
	int hasTexture_ = FALSE;
	DirectX::XMFLOAT3 lightDirection_ = {};
};

namespace DX3DManager {

	void InitDX3D();
	void InitShader();
	void InitDevice();
	void InitDepthBuffer();
	void InitViewPort();
	void InitRasterizer();
	void InitBlend();

	 ID3D11Device* GetDevice();
	 ID3D11DeviceContext* GetDeviceContext();
	 IDXGISwapChain* GetSwapChain();
	 ID3D11RenderTargetView* GetRTV();
	 ID3D11Texture2D* GetTexture2D();
	 ID3D11Texture2D* GetDepthTexture2D();
	 ID3D11DepthStencilState* GetDepthState();
	 ID3D11DepthStencilView* GetDepthView();
	 ID3D11RasterizerState* GetRasterizerState();
	 ID3D11BlendState* GetBlendState();

	 void EnableZDepthWrite();
	 void DisableZDepthWrite();
	 void EnableWireframe();
	 void DisableWireframe();

}