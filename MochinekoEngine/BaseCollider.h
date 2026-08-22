#pragma once
#include <string>
#include "Transform.h"

class BaseObject;
class Transform;

enum class ColliderType {
	SPHERE,
	BOX
};

class BaseCollider {
protected:
	ColliderType type_;
	BaseObject* parent_;
	Transform transform_;
public:

	BaseCollider(const ColliderType& type, BaseObject* parent) {
		type_ = type;
		parent_ = parent;
		transform_ = {};
	};

	~BaseCollider() {};

	/// <summary>
	/// コライダーを初期化する関数
	/// </summary>
	virtual void Init() = 0;

	/// <summary>
	/// コライダーを更新する関数
	/// </summary>
	virtual void Update() = 0;

	/// <summary>
	/// コライダーをテスト用で描画する関数
	/// </summary>
	virtual void Draw() {};

	ColliderType GetColliderType() const { return type_; }

	BaseObject* GetParent() const { return parent_; }
};