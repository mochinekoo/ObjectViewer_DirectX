#pragma once
#include <string>
#include <DirectXMath.h>
#include "Transform.h"

class BaseObject {
private:
	int drawHighOrder_;	// 高いほど手前に
	std::string name_;
protected:
	std::string tag_;
	bool isDead_;

	Transform transform_;
public:

	BaseObject(const std::string& name) {
		name_ = name;
		tag_ = "";
		isDead_ = false;
		transform_ = Transform();
		drawHighOrder_ = 0;
	};
	virtual ~BaseObject() {};

	virtual void Init() {};
	virtual void Update() {};
	virtual void Draw() {};
	virtual void Release() {};

	std::string GetName() const { return name_; }
	std::string GetTag() const { return tag_; }
	void SetTag(const std::string& tag) { tag_ = tag; }
	void KillMe() { isDead_ = true; }
	bool IsDead() const { return isDead_; }
	Transform GetTransform() const { return transform_; }
	void SetTransform(const Transform& transform) { transform_ = transform; }
	int GetDrawOrder() const { return drawHighOrder_; }
	void SetDrawOrder(const int order) { drawHighOrder_ = order;  }

};