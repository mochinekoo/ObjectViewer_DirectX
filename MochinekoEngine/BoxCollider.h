#pragma once
#include <DirectXMath.h>
#include "BaseCollider.h"
#include "FBX.h"

class BoxCollider : public BaseCollider {
private:
	FBX* fbx_;
	DirectX::XMFLOAT3 colliderSize_;
public:

	BoxCollider(BaseObject* parent, const DirectX::XMFLOAT3& colliderSize)
		: BaseCollider(ColliderType::BOX, parent) {
		colliderSize_ = colliderSize;
		fbx_ = new FBX("MochinekoEngine/Asset/BoxCollider.fbx");
	}

	~BoxCollider() {

	}

	void Init() override;
	void Update() override;
	void Draw() override;

	DirectX::XMFLOAT3 GetColliderSize() const { return colliderSize_; }
	void SetColliderSize(const DirectX::XMFLOAT3& colliderSize) { colliderSize_ = colliderSize; }

	static bool IsHitBoxBox(BoxCollider* colA, BoxCollider* colB);
};