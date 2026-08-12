#include "DX3DManager.h"
#include <Windows.h>
#include <d3dcompiler.h>
#include <d3d11.h>
#include <DirectXMath.h>
#include "Framework.h"
#include "ShaderManager.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

using namespace GameLib;

namespace DX3DManager {
	inline ID3D11Device* device_ = nullptr;
	inline ID3D11DeviceContext* deviceContext_ = nullptr;
	inline IDXGISwapChain* swapChain_ = nullptr;
	inline ID3D11RenderTargetView* renderTargetView_ = nullptr;
	inline ID3D11Texture2D* texture2D_ = nullptr;
	inline ID3D11Texture2D* depthStencilTexture_ = nullptr;
	inline ID3D11DepthStencilState* depthState_ = nullptr;
	inline ID3D11DepthStencilView* depthStencilView_ = nullptr;
	inline ID3D11RasterizerState* rasterizerState_ = nullptr;
	inline ID3D11BlendState* blendState_ = nullptr;
}

void DX3DManager::InitDX3D() {
	InitDevice();
	InitDepthBuffer();
	InitViewPort();
	InitRasterizer();
	InitBlend();
	InitShader();
}

void DX3DManager::InitShader() {
	ShaderManager::Init();
	ShaderManager::AddShader(ShaderType::PIXEL_SHADER, "PixelShader.hlsl");
	ShaderManager::AddShader(ShaderType::VERTEX_SHADER, "VertexShader.hlsl");
}

void DX3DManager::InitDevice() {

	DXGI_SWAP_CHAIN_DESC swapchainDesc = {};
	swapchainDesc.BufferDesc.Width = 1280;
	swapchainDesc.BufferDesc.Height = 720;
	swapchainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapchainDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapchainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapchainDesc.Windowed = TRUE;
	swapchainDesc.OutputWindow = GetGameWindowHandle();
	swapchainDesc.BufferCount = 1;
	swapchainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapchainDesc.SampleDesc.Count = 1;	
	swapchainDesc.SampleDesc.Quality = 0;	

	D3D_FEATURE_LEVEL level = {};
	D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		D3D11_CREATE_DEVICE_BGRA_SUPPORT,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&swapchainDesc,
		&swapChain_,
		&device_,
		&level,
		&deviceContext_
	);

	swapChain_->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&texture2D_);
	device_->CreateRenderTargetView(texture2D_, NULL, &renderTargetView_);
}

void DX3DManager::InitDepthBuffer() {
	D3D11_TEXTURE2D_DESC depthDesc = {};
	depthDesc.Width = 1280;
	depthDesc.Height = 720;
	depthDesc.MipLevels = 1;
	depthDesc.ArraySize = 1;
	depthDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthDesc.SampleDesc.Count = 1;
	depthDesc.SampleDesc.Quality = 0;
	depthDesc.Usage = D3D11_USAGE_DEFAULT;
	depthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthDesc.CPUAccessFlags = 0;

	device_->CreateTexture2D(&depthDesc, nullptr, &depthStencilTexture_);

	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc = {};
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	device_->CreateDepthStencilView(depthStencilTexture_, &depthStencilViewDesc, &depthStencilView_);
}

void DX3DManager::InitViewPort() {
	D3D11_VIEWPORT viewport = {};
	viewport.Width = 1280;
	viewport.Height = 720;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	deviceContext_->RSSetViewports(1, &viewport);
}

void DX3DManager::InitRasterizer() {
	D3D11_RASTERIZER_DESC rasterizerDesc = {};
	rasterizerDesc.FillMode = D3D11_FILL_SOLID;
	rasterizerDesc.CullMode = D3D11_CULL_NONE;
	rasterizerDesc.FrontCounterClockwise = FALSE;

	device_->CreateRasterizerState(&rasterizerDesc, &rasterizerState_);
	deviceContext_->RSSetState(rasterizerState_);
}

void DX3DManager::InitBlend() {
	D3D11_BLEND_DESC blendDesc = {};
	blendDesc.RenderTarget[0].BlendEnable = TRUE; 
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	device_->CreateBlendState(&blendDesc, &blendState_);

	float blendFactor[4] = { 0, 0, 0, 0 };
	deviceContext_->OMSetBlendState(
		blendState_,
		blendFactor,
		0xFFFFFFFF
	);
}

ID3D11Device* DX3DManager::GetDevice() { return device_; }
ID3D11DeviceContext* DX3DManager::GetDeviceContext() { return deviceContext_; }
IDXGISwapChain* DX3DManager::GetSwapChain() { return swapChain_; }
ID3D11RenderTargetView* DX3DManager::GetRTV() { return renderTargetView_; }
ID3D11Texture2D* DX3DManager::GetTexture2D() { return texture2D_; }
ID3D11Texture2D* DX3DManager::GetDepthTexture2D() { return depthStencilTexture_; }
ID3D11DepthStencilState* DX3DManager::GetDepthState() { return depthState_; }
ID3D11DepthStencilView* DX3DManager::GetDepthView() { return depthStencilView_; }
ID3D11RasterizerState* DX3DManager::GetRasterizerState() { return rasterizerState_; }
ID3D11BlendState* DX3DManager::GetBlendState() { return blendState_; }