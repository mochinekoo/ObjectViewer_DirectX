#include "VideoData.h"
#include "DX3DManager.h"
#include <wincodec.h>
#include <DirectXMath.h>
#include <string>
#include <vector>
#include <d3d11.h>
#include <Windows.h>
#include "ShaderManager.h"
#include <filesystem>
#include "SoundManager.h"

using namespace DirectX;
using namespace DX3DManager;

void VideoData::Init() {
	InitWIC();
	InitTexture();
	InitSamplerState();
	InitConstantBuffer();
	InitVertexBuffer();

	SoundManager::Load(fileName_);
	SoundManager::Play(fileName_);
}

void VideoData::Update() {
	if (streamFlag & MF_SOURCE_READERF_ENDOFSTREAM) {
		PropVariantInit(&var);
		var.vt = VT_I8;
		var.hVal.QuadPart = 0;
		sourceReader_->SetCurrentPosition(GUID_NULL, var);
		PropVariantClear(&var);
		return;
	}

	sourceReader_->ReadSample(
		MF_SOURCE_READER_FIRST_VIDEO_STREAM,
		0,
		nullptr,
		&streamFlag,
		nullptr,
		&sample
	);

	sample->ConvertToContiguousBuffer(&buffer);
	sample->GetSampleTime(&sampleTime);
	buffer->Lock(&data, nullptr, &currentLength);

	GetDeviceContext()->UpdateSubresource(
		texture2D_,
		0,
		nullptr,
		data,
		width_ * 4,
		0
	);

	buffer->Unlock();
	buffer->Release();
	sample->Release();
}

void VideoData::InitWIC() {
	std::wstring wFileName(fileName_.begin(), fileName_.end());

	if (!std::filesystem::exists(fileName_)) {
		MessageBox(NULL, L"ファイルが見つかりません。", NULL, MB_OK);
		return;
	}

	IMFAttributes* attricutes = nullptr;
	MFCreateAttributes(&attricutes, 1);
	attricutes->SetUINT32(
		MF_SOURCE_READER_ENABLE_ADVANCED_VIDEO_PROCESSING,
		TRUE
	);

	MFCreateSourceReaderFromURL(
		wFileName.c_str(),
		attricutes,
		&sourceReader_
	);

	MFCreateMediaType(&mediaType_);
	mediaType_->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Video);
	mediaType_->SetGUID(MF_MT_SUBTYPE, MFVideoFormat_RGB32);

	sourceReader_->SetCurrentMediaType(
		MF_SOURCE_READER_FIRST_VIDEO_STREAM,
		nullptr,
		mediaType_
	);

	IMFMediaType* mediaType = nullptr;
	sourceReader_->GetCurrentMediaType(
		MF_SOURCE_READER_FIRST_VIDEO_STREAM,
		&mediaType
	);

	MFGetAttributeSize(
		mediaType,
		MF_MT_FRAME_SIZE,
		&width_,
		&height_
	);
}

void VideoData::InitTexture() {
	HRESULT result = {};

	IMFSample* sample = nullptr;
	DWORD streamFlag = 0;

	sourceReader_->ReadSample(
		MF_SOURCE_READER_FIRST_VIDEO_STREAM,
		0,
		nullptr,
		&streamFlag,
		nullptr,
		&sample
	);

	IMFMediaBuffer* buffer = nullptr;
	sample->ConvertToContiguousBuffer(&buffer);

	BYTE* data = nullptr;
	DWORD currentLength = 0;
	result = buffer->Lock(
		&data,
		nullptr,
		&currentLength
	);

	D3D11_TEXTURE2D_DESC textureDesc = {};
	textureDesc.Width = width_;
	textureDesc.Height = height_;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MipLevels = 1;

	D3D11_SUBRESOURCE_DATA VideoTextureData = {};
	VideoTextureData.pSysMem = data;
	VideoTextureData.SysMemPitch = width_ * 4;

	result = GetDevice()->CreateTexture2D(&textureDesc, &VideoTextureData, &texture2D_);


	D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc = {};
	viewDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	viewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	viewDesc.Texture2D.MipLevels = 1;
	result = GetDevice()->CreateShaderResourceView(texture2D_, &viewDesc, &shaderResourceView_);

	buffer->Unlock();
	sample->Release();
}

void VideoData::InitSamplerState() {
	D3D11_SAMPLER_DESC samplerDesc = {};
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	GetDevice()->CreateSamplerState(&samplerDesc, &samplerState_);
}

void VideoData::InitVertexBuffer() {
	vertexList_.clear();
	vertexList_.push_back({ {0.0f, 0.0f, 0.0f}, {}, { 1.0f, 1.0f, 1.0f, 1.0f }, {0.0f, 0.0f} });
	vertexList_.push_back({ {(float)width_, 0.0f, 0.0f}, {}, {1.0f, 1.0f, 1.0f, 1.0f}, {1.0f, 0.0f} });
	vertexList_.push_back({ {(float)width_, (float)height_, 0.0f}, {}, {1.0f, 1.0f, 1.0f, 1.0f}, {1.0f, 1.0f} });
	vertexList_.push_back({ {(float)width_, (float)height_, 0.0f }, {}, { 1.0f, 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f } });
	vertexList_.push_back({ {0.0f, (float)height_, 0.0f}, {}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 1.0f} });
	vertexList_.push_back({ {0.0f, 0.0f, 0.0f}, {}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 0.0f} });

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

void VideoData::InitConstantBuffer() {
	D3D11_BUFFER_DESC constantBufferDesc = {};
	constantBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	constantBufferDesc.ByteWidth = sizeof(ConstantBuffer);
	constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constantBufferDesc.CPUAccessFlags = 0;
	constantBufferDesc.MiscFlags = 0;
	constantBufferDesc.StructureByteStride = 0;
	GetDevice()->CreateBuffer(&constantBufferDesc, nullptr, &constantBuffer_);
}
