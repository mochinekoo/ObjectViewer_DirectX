#include "ShaderManager.h"
#include <vector>
#include "DX3DManager.h"
#include <map>
#include <filesystem>

using namespace DX3DManager;

namespace ShaderManager {
	std::map<std::string, PixelGameShader> pixelShaderMap_;
	std::map<std::string, VertexGameShader> vertexShaderMap_;
}

void ShaderManager::Init() {
	pixelShaderMap_.clear();
	vertexShaderMap_.clear();
}

void ShaderManager::Release() {
}

void ShaderManager::AddShader(const ShaderType& type, const std::string& fileName) {
	if (!std::filesystem::exists(fileName)) {
		MessageBox(NULL, L"シェーダーが見つかりませんでした。", NULL, MB_OK);
		ExitProcess(-1);
		return;
	}

	ID3DBlob* shaderBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;

	D3D11_INPUT_ELEMENT_DESC inputDesc[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(Vertex, location_), D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT,    0, 36, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, offsetof(Vertex, color_), D3D11_INPUT_PER_VERTEX_DATA, 0},
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, offsetof(Vertex, uv_), D3D11_INPUT_PER_VERTEX_DATA, 0},
	};
	
	std::wstring wFileName = std::wstring(fileName.begin(), fileName.end());
	if (type == ShaderType::PIXEL_SHADER) {
		PixelGameShader pixelShader = {};

		D3DCompileFromFile(wFileName.c_str(), nullptr, nullptr, "main", "ps_5_0", 0, 0, &shaderBlob, &errorBlob);
		GetDevice()->CreatePixelShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), nullptr, &pixelShader.pixelShader_);

		pixelShaderMap_[fileName] = pixelShader;
	}
	else if (type == ShaderType::VERTEX_SHADER) {
		VertexGameShader vertexShader = {};

		D3DCompileFromFile(wFileName.c_str(), nullptr, nullptr, "main", "vs_5_0", 0, 0, &shaderBlob, &errorBlob);
		GetDevice()->CreateVertexShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), nullptr, &vertexShader.vertexShader_);

		GetDevice()->CreateInputLayout(inputDesc, std::size(inputDesc), shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), &vertexShader.inputLayout_);

		vertexShaderMap_[fileName] = vertexShader;
	}
	
}

void ShaderManager::SetPixelShader(const std::string& fileName) {
	GetDeviceContext()->PSSetShader(pixelShaderMap_[fileName].pixelShader_, nullptr, 0);
}

void ShaderManager::SetVertexShader(const std::string& fileName) {
	GetDeviceContext()->VSSetShader(vertexShaderMap_[fileName].vertexShader_, nullptr, 0);
	GetDeviceContext()->IASetInputLayout(vertexShaderMap_[fileName].inputLayout_);
}