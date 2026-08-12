#pragma once
#include <d3dcompiler.h>
#include <d3d11.h>
#include <DirectXMath.h>
#include "Framework.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

struct Vertex {
	DirectX::XMFLOAT3 location_ = {};
	DirectX::XMFLOAT4 color_ = {};
	DirectX::XMFLOAT2 uv_ = {};
};

struct ConstantBuffer {
	DirectX::XMMATRIX wvpMatrix_ = {};
	DirectX::XMFLOAT4 diffuse_ = {};
	DirectX::XMFLOAT4 ambient_ = {};
	int hasTexture_ = FALSE;
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

}