#pragma once
#include "Camera.h"
#include <string>

namespace CameraManager {

	void Init();

	Camera* CreateCamera(const std::string& name, const DirectX::XMFLOAT3& eye, const DirectX::XMFLOAT3& at, const DirectX::XMFLOAT3& up, const DirectX::XMMATRIX projection);
	Camera* CreateCamera(const std::string& name);

	Camera* GetCurrentCamera();
	void SetCurrentCamera(const std::string& name);

	Camera* GetCamera(const std::string& name);

}