#include "Texture.h"
#include "DX3DManager.h"
#include <wincodec.h>
#include <DirectXMath.h>
#include <string>
#include <vector>
#include <d3d11.h>
#include <Windows.h>
#include "ShaderManager.h"

using namespace DirectX;
using namespace DX3DManager;

void Texture::Init() {
	InitWIC();
	InitTexture();
	InitSamplerState();
	InitConstantBuffer();
	InitVertexBuffer();
}

void Texture::InitWIC() {
	std::wstring wFileName(fileName_.begin(), fileName_.end());
	CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&imagingFactory_));
	imagingFactory_->CreateDecoderFromFilename(wFileName.c_str(), nullptr, GENERIC_READ, WICDecodeMetadataCacheOnLoad, &bitmapDecoder_);
	bitmapDecoder_->GetFrame(0, &bitmapFrame_);
	imagingFactory_->CreateFormatConverter(&formatConverter_);
	formatConverter_->Initialize(bitmapFrame_, GUID_WICPixelFormat32bppRGBA, WICBitmapDitherTypeNone, nullptr, 0.0, WICBitmapPaletteTypeMedianCut);
	formatConverter_->GetSize(&width_, &height_);
}

void Texture::InitTexture() {
	D3D11_TEXTURE2D_DESC textureDesc = {};
	textureDesc.Width = width_;
	textureDesc.Height = height_;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MipLevels = 1;

	std::vector<BYTE> pixelData(width_ * height_ * 4);
	formatConverter_->CopyPixels(nullptr, width_ * 4, pixelData.size(), pixelData.data());

	D3D11_SUBRESOURCE_DATA textureData = {};
	textureData.pSysMem = pixelData.data();
	textureData.SysMemPitch = width_ * 4;

	GetDevice()->CreateTexture2D(&textureDesc, &textureData, &texture2D_);

	D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc = {};
	viewDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	viewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	viewDesc.Texture2D.MipLevels = 1;
	GetDevice()->CreateShaderResourceView(texture2D_, &viewDesc, &shaderResourceView_);
}

void Texture::InitSamplerState() {
	D3D11_SAMPLER_DESC samplerDesc = {};
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	GetDevice()->CreateSamplerState(&samplerDesc, &samplerState_);
}

void Texture::InitVertexBuffer() {
	vertexList_.clear();
	vertexList_.push_back({ {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 0.0f} });
	vertexList_.push_back({ {(float) width_, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {1.0f, 0.0f} });
	vertexList_.push_back({ {(float) width_, (float) height_, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {1.0f, 1.0f} });
	vertexList_.push_back({ {(float) width_, (float) height_, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f } });
	vertexList_.push_back({ {0.0f, (float) height_, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 1.0f} });
	vertexList_.push_back({ {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 0.0f} });

	D3D11_BUFFER_DESC vertexDesc = {};
	vertexDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexDesc.ByteWidth = sizeof(Vertex) * vertexList_.size();
	vertexDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexDesc.CPUAccessFlags = 0;
	vertexDesc.MiscFlags = 0;
	vertexDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA vertexData = {};
	vertexData.pSysMem = vertexList_.data();

	GetDevice()->CreateBuffer(&vertexDesc, &vertexData, &vertexBuffer_);
}

void Texture::InitConstantBuffer() {
	D3D11_BUFFER_DESC constantBufferDesc = {};
	constantBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	constantBufferDesc.ByteWidth = sizeof(ConstantBuffer);
	constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constantBufferDesc.CPUAccessFlags = 0;
	constantBufferDesc.MiscFlags = 0;
	constantBufferDesc.StructureByteStride = 0;
	GetDevice()->CreateBuffer(&constantBufferDesc, nullptr, &constantBuffer_);
}
