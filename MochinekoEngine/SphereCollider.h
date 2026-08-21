#pragma once
#include "BaseObject.h"
#include "BaseCollider.h"
#include "FBX.h"

class SphereCollider : public BaseCollider {
private:
	FBX* fbx_;
	float radius_;
public:

	SphereCollider(BaseObject* parent, const float radius)
		: BaseCollider(ColliderType::SPHERE, parent) {
		radius_ = radius;
		fbx_ = new FBX("MochinekoEngine/Asset/SphereCollider.fbx");
	};

	~SphereCollider() {

	}

	void Init() override;
	void Update() override;
	void Draw() override;

	float GetRadius() const { return radius_; }
	void SetRadius(const float radius) { radius_ = radius; }

	static bool IsHitSphereSphere(SphereCollider* col1, SphereCollider* col2);
};