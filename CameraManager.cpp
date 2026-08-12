#include "CameraManager.h"
#include <map>
#include <string>
#include "Camera.h"

namespace CameraManager {
	std::map<std::string, Camera*> cameraMap_;
	Camera* currentCamera_;
}

void CameraManager::Init() {
	cameraMap_.clear();
	CreateCamera("RootCamera");
	SetCurrentCamera("RootCamera");
}

Camera* CameraManager::CreateCamera(const std::string& name, const DirectX::XMFLOAT3& eye, const DirectX::XMFLOAT3& at, const DirectX::XMFLOAT3& up, const DirectX::XMMATRIX projection) {
	Camera* camera = new Camera(name, eye, at, up, projection);
	cameraMap_[name] = camera;
	return camera;
}

Camera* CameraManager::CreateCamera(const std::string& name) {
	Camera* camera = new Camera(name);
	cameraMap_[name] = camera;
	return camera;
}

Camera* CameraManager::GetCurrentCamera() {
	return currentCamera_;
}

void CameraManager::SetCurrentCamera(const std::string& name) {
	Camera* camera = GetCamera(name);
	if (camera != nullptr) {
		currentCamera_ = camera;
	}
}

Camera* CameraManager::GetCamera(const std::string& name) {
	Camera* camera = cameraMap_[name];
	if (camera->GetName() == name) {
		return camera;
	}

	return nullptr;
}
