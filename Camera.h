#pragma once
#include <DirectXMath.h>
#include <string>

class Camera {
private:
	std::string name_;
	DirectX::XMMATRIX projection_;
public:
	DirectX::XMFLOAT3 postion_;
	DirectX::XMFLOAT3 target_;
	DirectX::XMFLOAT3 upDirection_;

	Camera(const std::string& name) {
		name_ = name;
		postion_ = { 0.0f, 0.0f, -5.0f};
		target_ = { 0.0f, 2.0f, 0.0f };
		upDirection_ = { 0.0f, 1.0f, 0.0f };
		projection_ = DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(60.0f), 1280.0f / 720.0f, 0.1f, 100.0f);
	}

	Camera(const std::string& name, const DirectX::XMFLOAT3& eye, const DirectX::XMFLOAT3& at, const DirectX::XMFLOAT3& up, const DirectX::XMMATRIX& projection) {
		postion_ = eye;
		target_ = at;
		upDirection_ = up;
		projection_ = projection;
		name_ = name;
	}

	~Camera() {};

	DirectX::XMMATRIX GetViewMatrix() const {
		DirectX::XMVECTOR postion = DirectX::XMVectorSet(postion_.x, postion_.y, postion_.z, 1.0f);
		DirectX::XMVECTOR target = DirectX::XMVectorSet(target_.x, target_.y, target_.z, 1.0f);
		DirectX::XMVECTOR upDirection = DirectX::XMVectorSet(upDirection_.x, upDirection_.y, upDirection_.z, 1.0f);
		return DirectX::XMMatrixLookAtLH(postion, target, upDirection);
	}

	DirectX::XMMATRIX GetProjection() const {
		return projection_;
	}

	void SetProjection(const DirectX::XMMATRIX& matrix) {
		projection_ = matrix;
	}

	std::string GetName() const {
		return name_;
	}
};