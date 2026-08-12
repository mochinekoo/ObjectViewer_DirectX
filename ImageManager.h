#pragma once
#include <string>
#include <DirectXMath.h>
#include "Transform.h"

/// <summary>
/// 画像を管理する名前空間。
/// </summary>
namespace ImageManager {

	void Init();

	int Load(const std::string& fileName);
	void Draw(const int id);
	Transform GetTransform(const int id);
	void SetTransform(const int id, const Transform& transform);
}