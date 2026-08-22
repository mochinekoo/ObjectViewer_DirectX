#include "BoxCollider.h"
#include "Transform.h"

using namespace DirectX;

void BoxCollider::Init() {
	fbx_->Init();
	fbx_->SetWireframe(true);
	Transform transform = fbx_->GetTransform();
	transform.scale_ = colliderSize_;
	fbx_->SetTransform(transform);
}

void BoxCollider::Update() {
	Transform parentTransform = parent_->GetTransform();
	transform_.location_ = parentTransform.location_;
	transform_.scale_ = colliderSize_;

	fbx_->SetTransform(transform_);
	fbx_->Update();
}

void BoxCollider::Draw() {
	fbx_->Draw();
}

bool BoxCollider::IsHitBoxBox(BoxCollider* colA, BoxCollider* colB) {
	if (colA != nullptr && colB != nullptr) {
		XMFLOAT3 locA = colA->transform_.location_;
		XMFLOAT3 sizeA = colA->GetColliderSize();
		XMFLOAT3 locB = colB->transform_.location_;
		XMFLOAT3 sizeB = colB->GetColliderSize();
		bool hitX = locA.x < locB.x + sizeB.x && locA.x + sizeA.x > locB.x;
		bool hitY = locA.y < locB.y + sizeB.y && locA.y + sizeA.y > locB.y;
		bool hitZ = locA.z < locB.z + sizeB.z && locA.z + sizeA.z > locB.z;
		return hitX && hitY && hitZ;
	}
	return false;
}
