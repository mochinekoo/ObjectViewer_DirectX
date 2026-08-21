#pragma once
#include <string>
#include <DirectXMath.h>
#include "Transform.h"
#include "ImGUI/imgui.h"
#include <vector>
#include "BaseCollider.h"

class BaseObject {
private:
	int drawHighOrder_;	// 高いほど手前に
	std::string name_;
protected:
	std::string tag_;
	bool isDead_;
	bool showImGUI_;
	std::vector<BaseCollider*> colliderList_;

	Transform transform_;
public:

	BaseObject(const std::string& name) {
		name_ = name;
		tag_ = "";
		isDead_ = false;
		transform_ = Transform();
		drawHighOrder_ = 0;
		showImGUI_ = true;
	};
	virtual ~BaseObject() {};

	virtual void Init() {};
	virtual void Update() {};
	virtual void Draw() {};
	virtual void DrawImGUI() {
		if (!showImGUI_) return;
		ImGui::Begin(name_.c_str(), nullptr, ImGuiWindowFlags_NoDocking);
		if (ImGui::BeginTabBar("Tab")) {
			if (ImGui::BeginTabItem("ObjectInfo")) {
				ImGui::SliderFloat("Location X", &transform_.location_.x, -10.0f, 10.0f);
				ImGui::SliderFloat("Location Y", &transform_.location_.y, -10.0f, 10.0f);
				ImGui::SliderFloat("Location Z", &transform_.location_.z, -10.0f, 10.0f);
				ImGui::SliderFloat("Velocity X", &transform_.velocity_.x, -10.0f, 10.0f);
				ImGui::SliderFloat("Velocity Y", &transform_.velocity_.y, -10.0f, 10.0f);
				ImGui::SliderFloat("Velocity Z", &transform_.velocity_.z, -10.0f, 10.0f);
				ImGui::SliderFloat("Rotation X", &transform_.rotation_.x, -DirectX::XM_PI, DirectX::XM_PI);
				ImGui::SliderFloat("Rotation Y", &transform_.rotation_.y, -DirectX::XM_PI, DirectX::XM_PI);
				ImGui::SliderFloat("Rotation Z", &transform_.rotation_.z, -DirectX::XM_PI, DirectX::XM_PI);
				ImGui::SliderFloat("Scale X", &transform_.scale_.x, 0.1f, 10.0f);
				ImGui::SliderFloat("Scale Y", &transform_.scale_.y, 0.1f, 10.0f);
				ImGui::SliderFloat("Scale Z", &transform_.scale_.z, 0.1f, 10.0f);
			}
		}
		ImGui::End();
	};
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
	bool IsShowImGUI() const { return showImGUI_; }
	void SetShowImGUI(const bool flag) { showImGUI_ = flag; }
	std::vector<BaseCollider*>& GetColliderList() { return colliderList_; }
};