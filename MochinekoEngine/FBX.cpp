#include "FBX.h"
#include <vector>
#include "fbxsdk.h"
#include "DX3DManager.h"
#include "ImGUI/imgui.h"
#include <unordered_map>
#include "ShaderManager.h"
#include "Camera.h"
#include "CameraManager.h"
#include "SphereCollider.h"
#include "BoxCollider.h"

using namespace fbxsdk;
using namespace DX3DManager;
using namespace DirectX;

void FBX::Init() {
	InitFBX();
} 

void FBX::Update() {
	for (int i = 0; i < childList_.size(); i++) {
		FbxChild* fbx = dynamic_cast<FbxChild*>(childList_[i]);
		if (fbx != nullptr) {
			fbx->Update();
		}
	}
}

void FBX::Draw() {
	ShaderManager::SetPixelShader("MochinekoEngine/PixelShader.hlsl");
	ShaderManager::SetVertexShader("MochinekoEngine/VertexShader.hlsl");

	if (!zDepthWrite_) {
		DisableZDepthWrite();
	}
	if (wireframe_) {
		EnableWireframe();
	}
	else {
		DisableWireframe();
	}

	for (int i = 0; i < childList_.size(); i++) {
		FbxChild* fbx = dynamic_cast<FbxChild*>(childList_[i]);
		if (fbx != nullptr) {
			fbx->Draw();
		}
	}

	EnableZDepthWrite();
	DisableWireframe();
}

void FBX::DrawImGUI() {
	ImGui::Begin(fileName_.c_str(), nullptr, ImGuiWindowFlags_NoDocking);

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
			ImGui::Checkbox("Wireframe", &wireframe_);
			if (ImGui::Button("Create Sphere Collider")) {
				auto collider = new SphereCollider(this, 1.0f);
				collider->Init();
				colliderList_.push_back(collider);
			}
			if (ImGui::Button("Create Box Collider")) {
				auto collider = new BoxCollider(this, {1.0f, 1.0f, 1.0f});
				collider->Init();
				colliderList_.push_back(collider);
			}

			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Collider")) {
			for (int a = 0; a < colliderList_.size(); a++) {
				BaseCollider* collider = colliderList_[a];
				SphereCollider* sphere = dynamic_cast<SphereCollider*>(collider);
				BoxCollider* box = dynamic_cast<BoxCollider*>(collider);
				if (ImGui::TreeNode(std::to_string(a).c_str())) {
					if (sphere != nullptr) {
						float radius = sphere->GetRadius();
						ImGui::SliderFloat("Radius", &radius, 0.0f, 10.0f);
						sphere->SetRadius(radius);
					}
					if (box != nullptr) {
						XMFLOAT3 sizeCol = box->GetColliderSize();
						ImGui::SliderFloat("X Size", &sizeCol.x, 0.0f, 10.0f);
						ImGui::SliderFloat("Y Size", &sizeCol.y, 0.0f, 10.0f);
						ImGui::SliderFloat("Z Size", &sizeCol.z, 0.0f, 10.0f);
						box->SetColliderSize(sizeCol);
					}
					ImGui::TreePop();
				}
			}
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Light")) {

			for (int i = 0; i < rootNode_->GetChildCount(); i++) {
				FbxNode* node = rootNode_->GetChild(i);
				FbxLight* light = node->GetLight();
				if (light == nullptr) continue;

				if (ImGui::TreeNode(light->GetName())) {
					ImGui::Text("Light Name: %s", light == nullptr ? "(nullptr)" : light->GetName());
					float r = light->Color.Get()[0];
					float g = light->Color.Get()[1];
					float b = light->Color.Get()[2];
					ImGui::Text("Color: R: %.1f (%.1f), G: %.1f (%.1f), B: %.1f (%.1f)", r, r * 255, g, g * 255, b, b * 255);

					ImGui::TreePop();
				}

				ImGui::EndTabItem();
			}
		}
	}
	ImGui::EndTabBar();

	ImGui::End();
}

void FBX::Release() {
}

void FBX::InitFBX() {
	 FbxManager* fbxManager = FbxManager::Create();
	 FbxIOSettings* ios = FbxIOSettings::Create(fbxManager, IOSROOT);
	 fbxManager->SetIOSettings(ios);
	 FbxImporter* fbxImporter = FbxImporter::Create(fbxManager, "fbximporter");
	 fbxImporter->Initialize(fileName_.c_str(), -1, fbxManager->GetIOSettings());
	 FbxScene* fbxScene = FbxScene::Create(fbxManager, "fbxscene");
	 fbxImporter->Import(fbxScene);

	 FbxGeometryConverter converter(fbxManager);
	 converter.Triangulate(fbxScene, true);

	 rootNode_ = fbxScene->GetRootNode();
	 for (int i = 0; i < rootNode_->GetChildCount(); i++) {
		 FbxNode* node = rootNode_->GetChild(i);
		 if (node->GetMesh() == nullptr) continue;
		 FbxChild* fbx = new FbxChild(this, node);
		 fbx->Init();
		 childList_.push_back(fbx);
	 }
}
