#pragma once
#include "BaseObject.h"
#include <Windows.h>
#include <d3d11.h>
#include <wincodec.h>
#include <DirectXMath.h>
#include <string>
#include "DX3DManager.h"
#include <vector>

class Texture {
private:
	std::string fileName_;
	UINT width_, height_;

	ID3D11SamplerState* samplerState_;
	ID3D11ShaderResourceView* shaderResourceView_;
	ID3D11Buffer* constantBuffer_;
	ID3D11Buffer* vertexBuffer_;
	ID3D11Texture2D* texture2D_;
	IWICImagingFactory* imagingFactory_;
	IWICBitmapDecoder* bitmapDecoder_;
	IWICBitmapFrameDecode* bitmapFrame_;
	IWICFormatConverter* formatConverter_;
	std::vector<Vertex> vertexList_;
public:

	Texture(const std::string& fileName) {
		fileName_ = fileName;
		width_ = 0;
		height_ = 0;
		samplerState_ = nullptr;
		shaderResourceView_ = nullptr;
		constantBuffer_ = nullptr;
		vertexBuffer_ = nullptr;
		texture2D_ = nullptr;
		imagingFactory_ = nullptr;
		bitmapDecoder_ = nullptr;
		bitmapFrame_ = nullptr;
		formatConverter_ = nullptr;
		vertexList_.clear();
	}

	~Texture() {}

	void Init();

	void InitWIC();
	void InitTexture();
	void InitSamplerState();
	void InitVertexBuffer();
	void InitConstantBuffer();

	ID3D11SamplerState* GetSamplerState() const { return samplerState_; }
	ID3D11ShaderResourceView* GetShaderResourceView() const { return shaderResourceView_; }

	ID3D11Buffer* GetConstanctBuffer() const { return constantBuffer_; }
	ID3D11Buffer* GetVertexBuffer() const { return vertexBuffer_; }

	std::string GetFileName() const { return fileName_; }


};