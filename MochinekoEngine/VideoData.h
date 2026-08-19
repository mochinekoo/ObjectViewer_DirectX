#pragma once
#include "BaseObject.h"
#include <Windows.h>
#include <d3d11.h>
#include <wincodec.h>
#include <DirectXMath.h>
#include <string>
#include "DX3DManager.h"
#include <vector>
#include <mfapi.h>
#include <mfidl.h>
#include <mfreadwrite.h>

class VideoData {
private:
	std::string fileName_;
	UINT32 width_, height_;

	IMFSourceReader* sourceReader_ = nullptr;
	IMFMediaType* mediaType_ = nullptr;
	std::vector<BYTE> mediaBuffer_;

	ID3D11SamplerState* samplerState_;
	ID3D11ShaderResourceView* shaderResourceView_;
	ID3D11Buffer* constantBuffer_;
	ID3D11Buffer* vertexBuffer_;
	ID3D11Texture2D* texture2D_;
	std::vector<Vertex> vertexList_;

	IMFSample* sample = nullptr;
	DWORD streamFlag = 0;
	IMFMediaBuffer* buffer = nullptr;
	BYTE* data = nullptr;
	DWORD currentLength = 0;
	PROPVARIANT var;
	LONGLONG sampleTime = 0;
public:

	VideoData(const std::string& fileName) {
		fileName_ = fileName;
		width_ = 0;
		height_ = 0;
		samplerState_ = nullptr;
		shaderResourceView_ = nullptr;
		constantBuffer_ = nullptr;
		vertexBuffer_ = nullptr;
		texture2D_ = nullptr;
		vertexList_.clear();
		PropVariantInit(&var);
	}

	~VideoData() {}

	void Init();
	void Update();

	void InitWIC();
	void InitTexture();
	void InitSamplerState();
	void InitVertexBuffer();
	void InitConstantBuffer();

	ID3D11SamplerState* GetSamplerState() const { return samplerState_; }
	ID3D11ShaderResourceView* GetShaderResourceView() const { return shaderResourceView_; }

	ID3D11Buffer* GetConstanctBuffer() const { return constantBuffer_; }
	ID3D11Buffer* GetVertexBuffer() const { return vertexBuffer_; }

	IMFSourceReader* GetSourceReader() const { return sourceReader_; }

	std::string GetFileName() const { return fileName_; }

	double GetTime() const {
		return sampleTime / 10'000'000.0;
	}

	void SetTime(const double time) {
		sampleTime = (LONGLONG)(time * 10'000'000.0);
		var.vt = VT_I8;
		var.hVal.QuadPart = sampleTime;
		sourceReader_->SetCurrentPosition(GUID_NULL, var);
		PropVariantClear(&var);
	}
};