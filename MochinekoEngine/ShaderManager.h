#pragma once
#include <string>
#include <d3dcompiler.h>
#include <d3d11.h>
#include <vector>

enum class ShaderType {
	PIXEL_SHADER,
	VERTEX_SHADER
};

struct VertexGameShader {
	ID3D11VertexShader* vertexShader_ = nullptr;
	ID3D11InputLayout* inputLayout_ = nullptr;
};

struct PixelGameShader {
	ID3D11PixelShader* pixelShader_ = nullptr;
};

namespace ShaderManager {

	void Init();
	void Release();

	void AddShader(const ShaderType& type, const std::string& fileName);

	void SetPixelShader(const std::string& fileName);
	void SetVertexShader(const std::string& fileName);

}