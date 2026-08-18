#pragma once
#include <DirectXMath.h>

class Transform {
private:
public:
	DirectX::XMFLOAT3 location_;
	DirectX::XMFLOAT3 velocity_;
	DirectX::XMFLOAT3 rotation_;
	DirectX::XMFLOAT3 scale_;

	Transform() {
		location_ = { 0,0,0 };
		velocity_ = { 0,0,0 };
		rotation_ = { 0,0,0 };
		scale_ = { 1,1,1 };
	};

	~Transform() {};

	DirectX::XMMATRIX GetWorldMatrix() const {
		DirectX::XMMATRIX scale = DirectX::XMMatrixScaling(scale_.x, scale_.y, scale_.z);
		DirectX::XMMATRIX rotation = DirectX::XMMatrixRotationRollPitchYaw(rotation_.x, rotation_.y, rotation_.z);
		DirectX::XMMATRIX translation = DirectX::XMMatrixTranslation(location_.x, location_.y, location_.z);
		DirectX::XMMATRIX world = scale * rotation * translation;
		return world;
	}
};