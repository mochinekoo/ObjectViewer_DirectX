#include "SphereCollider.h"
#include <DirectXMath.h>

using namespace DirectX;

void SphereCollider::Init() {
	fbx_->Init();
	fbx_->SetWireframe(true);
	Transform transform = fbx_->GetTransform();
	transform.scale_ = {radius_, radius_, radius_};
	fbx_->SetTransform(transform);
}

void SphereCollider::Update() {
	Transform parentTransform = parent_->GetTransform();
    transform_.location_ = parentTransform.location_;
	transform_.scale_ = { radius_, radius_, radius_ };

	fbx_->SetTransform(transform_);
	fbx_->Update();
}

void SphereCollider::Draw() {
	fbx_->Draw();
}

bool SphereCollider::IsHitSphereSphere(SphereCollider* col1, SphereCollider* col2) {
    if (col1 != nullptr && col2 != nullptr) {
		XMFLOAT3 aPos = col1->transform_.location_;
		XMFLOAT3 bPos = col2->transform_.location_;
		XMVECTOR vectorA = XMLoadFloat3(&aPos);
		XMVECTOR vectorB = XMLoadFloat3(&bPos);

		XMVECTOR diff = XMVectorSubtract(vectorA, vectorB);
		float distance = XMVectorGetX(XMVector3Length(diff));
		return distance < (col1->GetRadius() + col2->GetRadius());
    }

    return false;
}
