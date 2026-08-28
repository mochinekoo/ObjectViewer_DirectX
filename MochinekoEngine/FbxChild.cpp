#include "FbxChild.h"
#include "FBX.h"
#include <vector>
#include "fbxsdk.h"
#include "DX3DManager.h"
#include "ImGUI/imgui.h"
#include <unordered_map>
#include "ShaderManager.h"
#include "Camera.h"
#include "CameraManager.h"

using namespace fbxsdk;
using namespace DX3DManager;
using namespace DirectX;

void FbxChild::Init() {
	InitFBX();
	InitVertexBuffer();
	InitIndexBuffer();
	InitConstantBuffer();
	InitMaterial();
	InitBone();
}

void FbxChild::Update() {
	Camera* currentCamera = CameraManager::GetCurrentCamera();

	XMMATRIX world = GetWorldMatrix();
	XMMATRIX view = currentCamera->GetViewMatrix();
	XMMATRIX projection = currentCamera->GetProjection();

	for (int i = 0; i < materialCount_; i++) {
		ConstantBuffer constantBuffer = {};
		constantBuffer.wvpMatrix_ = XMMatrixTranspose(world * view * projection);
		constantBuffer.diffuse_ = materialList_[i].diffuse_;
		constantBuffer.ambient_ = materialList_[i].ambient_;
		constantBuffer.specular_ = materialList_[i].specular_;
		constantBuffer.emission_ = materialList_[i].emission_;
		constantBuffer.hasTexture_ = (materialList_[i].texture_ != nullptr);
		constantBuffer.lightDirection_ = { 0.0f, -1.0f, 0.0f };
		GetDeviceContext()->UpdateSubresource(constantBufferList_[i], 0, nullptr, &constantBuffer, 0, 0);
	}
}

void FbxChild::Draw() {
	if (!zDepthWrite_) {
		DisableZDepthWrite();
	}
	if (wireframe_) {
		EnableWireframe();
	}
	else {
		DisableWireframe();
	}

	if (animation_ && hasAnimation_) {

		if (fbxTime_.GetFrameCount() > maxFrame_) {
			fbxTime_.SetFrame(0);
		}
		fbxTime_.SetFrame(fbxTime_.GetFrameCount() + 1);

		for (int i = 0; i < clusterCount_; i++) {
			FbxAnimEvaluator* evaluator = clusterList_[i]->GetLink()->GetScene()->GetAnimationEvaluator();
			FbxMatrix currentAnimLocation = evaluator->GetNodeGlobalTransform(clusterList_[i]->GetLink(), fbxTime_);

			XMFLOAT4X4 postion = {};
			for (int x = 0; x < 4; x++) {
				for (int y = 0; y < 4; y++) {
					postion.m[x][y] = (float)currentAnimLocation.Get(x, y);
				}
			}

			XMFLOAT4X4 offset = {};
			XMMATRIX mirror = XMMatrixIdentity();
			XMStoreFloat4x4(&offset, mirror);
			offset.m[2][2] = -1.0f;
			mirror = XMLoadFloat4x4(&offset);

			boneList_[i].newBoneMatrix_ = XMLoadFloat4x4(&postion) * mirror;
			boneList_[i].differenceMatrix_ = boneList_[i].newBoneMatrix_ * XMMatrixInverse(nullptr, boneList_[i].offsetMatrix_);
		}

		for (int i = 0; i < vertexCount_; i++) {
			XMMATRIX matrix = {};
			for (int m = 0; m < clusterCount_; m++) {
				if (weightList_[i].boneIndex_[m] < 0) {
					break;
				}
				matrix += boneList_[weightList_[i].boneIndex_[m]].differenceMatrix_ * weightList_[i].boneWeight_[m];
			}

			XMVECTOR location = XMLoadFloat3(&weightList_[i].location_);
			XMStoreFloat3(&vertexList_[i].location_, XMVector3TransformCoord(location, matrix));
		}

		D3D11_MAPPED_SUBRESOURCE mappedResource = {};
		DX3DManager::GetDeviceContext()->Map(vertexBuffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		if (mappedResource.pData) {
			memcpy_s(mappedResource.pData, sizeof(Vertex) * vertexCount_, vertexList_.data(), sizeof(Vertex) * vertexCount_);
			DX3DManager::GetDeviceContext()->Unmap(vertexBuffer_, 0);
		}
	}

	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	ShaderManager::SetPixelShader("MochinekoEngine/PixelShader.hlsl");
	ShaderManager::SetVertexShader("MochinekoEngine/VertexShader.hlsl");

	GetDeviceContext()->IASetVertexBuffers(0, 1, &vertexBuffer_, &stride, &offset);
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	for (int i = 0; i < materialCount_; i++) {
		GetDeviceContext()->IASetIndexBuffer(indexBuffer_[i], DXGI_FORMAT_R32_UINT, 0);
		GetDeviceContext()->VSSetConstantBuffers(0, 1, &constantBufferList_[i]);
		GetDeviceContext()->PSSetConstantBuffers(0, 1, &constantBufferList_[i]);

		Texture* texture = materialList_[i].texture_;
		if (texture != nullptr) {
			ID3D11ShaderResourceView* textureSrv = texture->GetShaderResourceView();
			ID3D11SamplerState* sampler = texture->GetSamplerState();
			GetDeviceContext()->PSSetShaderResources(0, 1, &textureSrv);
			GetDeviceContext()->PSSetSamplers(0, 1, &sampler);
		}
		else {
			ID3D11ShaderResourceView* nullSrv = { nullptr };
			GetDeviceContext()->PSSetShaderResources(0, 1, &nullSrv);
		}
		GetDeviceContext()->DrawIndexed(materialIndexList_[i].size(), 0, 0);
	}

	GetDeviceContext()->RSSetState(nullptr);
	EnableZDepthWrite();
}

void FbxChild::DrawImGUI() {
	std::string title = mesh_->GetName();
	ImGui::Begin(title.c_str(), nullptr, ImGuiWindowFlags_NoDocking);

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

			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("FBXInfo")) {
			FbxMesh* mesh = node_->GetMesh();
			FbxLight* light = node_->GetLight();

			ImGui::Text("Light Name: %s", light == nullptr ? "(nullptr)" : light->GetName());

			if (ImGui::CollapsingHeader("Mesh", ImGuiTreeNodeFlags_DefaultOpen)) {
				ImGui::Text("Mesh Name: %s", mesh == nullptr ? "(nullptr)" : mesh->GetName());
				if (mesh != nullptr) {
					ImGui::Text("Control Point Count: %d", mesh->GetControlPointsCount());
					ImGui::Text("Polygon Count: %d", mesh_->GetPolygonCount());
				}
			}

			if (ImGui::CollapsingHeader("Material", ImGuiTreeNodeFlags_DefaultOpen)) {
				ImGui::Text("Material Count %d", node_->GetMaterialCount());
				for (int i = 0; i < node_->GetMaterialCount(); i++) {
					FbxSurfaceMaterial* material = node_->GetMaterial(i);
					FbxProperty property = material->FindProperty(FbxSurfaceMaterial::sDiffuse);
					int textureCount = property.GetSrcObjectCount<FbxFileTexture>();
					FbxSurfaceLambert* lambartMaterial = (FbxSurfaceLambert*)node_->GetMaterial(i);
					FbxDouble3 diffuse = lambartMaterial->Diffuse;
					FbxDouble3 ambient = lambartMaterial->Ambient;

					std::string title = "Material" + std::to_string(i);
					if (ImGui::TreeNode(title.c_str())) {
						ImGui::Text("Material Name: %s", material->GetName());
						ImGui::Text("Material Diffuse Color: (%2.2f, %2.2f, %2.2f)", diffuse[0], diffuse[1], diffuse[2]);
						ImGui::Text("Material Ambient Color: (%2.2f, %2.2f, %2.2f)", ambient[0], ambient[1], ambient[2]);

						ImGui::TreePop();
					}
				}
			}

			if (ImGui::CollapsingHeader("Skin", ImGuiTreeNodeFlags_DefaultOpen)) {
				float frame = fbxTime_.GetFrameCount();
				ImGui::SliderFloat("FbxTime Frame", &frame, 0, 60);
				ImGui::Text("Skin Name: %s", skin_ == nullptr ? "(nullptr)" : skin_->GetName());
				if (skin_ != nullptr) {
					ImGui::Text("Cluster Count: %d", skin_->GetClusterCount());
					for (int i = 0; i < skin_->GetClusterCount(); i++) {
						std::string title = "Skin" + std::to_string(i);
						if (ImGui::TreeNode(title.c_str())) {
							FbxCluster* cluster = skin_->GetCluster(i);
							int controlPointCount = clusterList_[i]->GetControlPointIndicesCount();
							int* controlPointIndices = clusterList_[i]->GetControlPointIndices();
							double* weights = clusterList_[i]->GetControlPointWeights();

							for (int a = 0; a < controlPointCount; a++) {
								int indices = controlPointIndices[a];
								double weight = weights[a];
								ImGui::Text("Control Point Index: %d, Weight: %2.2f", indices, weight);
							}

							ImGui::TreePop();
						}
					}
				}

				fbxTime_.SetFrame(frame);
			}


			ImGui::EndTabItem();
		}
	}
	ImGui::EndTabBar();

	ImGui::End();
}

void FbxChild::Release() {
}

void FbxChild::InitFBX() {
	mesh_ = node_->GetMesh();
	polygonCount_ = mesh_->GetPolygonCount();
	materialCount_ = node_->GetMaterialCount();
}

void FbxChild::InitVertexBuffer() {
	assert(mesh_ != nullptr);

	FbxLayer* layer = mesh_->GetLayer(0);
	FbxLayerElementMaterial* materialList = layer->GetMaterials();
	FbxLayerElement::EMappingMode mapping = materialList->GetMappingMode();

	for (DWORD polyCount = 0; polyCount < polygonCount_; polyCount++) {
		for (int vertexCount = 0; vertexCount < 3; vertexCount++) {
			FbxVector4 normal = {};
			mesh_->GetPolygonVertexNormal(polyCount, vertexCount, normal);

			int vertexIndex = mesh_->GetPolygonVertex(polyCount, vertexCount);
			FbxVector4 vertexLoc = mesh_->GetControlPointAt(vertexIndex);
			int uvIndex = mesh_->GetTextureUVIndex(polyCount, vertexCount);
			FbxLayerElementUV* uvLayer = mesh_->GetLayer(0)->GetUVs();
			FbxVector2 uvLoc = uvLayer->GetDirectArray().GetAt(uvIndex);
			Vertex vertex = {};
			vertex.location_ = { (float)vertexLoc[0], (float)vertexLoc[2], (float)vertexLoc[1] };
			vertex.color_ = { 1.0f, 1.0f, 1.0f, 1.0f };
			vertex.uv_ = { (float)uvLoc.mData[0], (float)(1.0f - uvLoc.mData[1]) };
			vertex.normal_ = { (float)normal[0], (float)normal[1], (float)normal[2] };
			vertexList_.push_back(vertex);

			indexVertexList_.push_back(vertexIndex);
		}
	}
	vertexCount_ = vertexList_.size();

	D3D11_BUFFER_DESC vertexDesc = {};
	vertexDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexDesc.ByteWidth = sizeof(Vertex) * vertexCount_;
	vertexDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA vertexData = {};
	vertexData.pSysMem = vertexList_.data();

	GetDevice()->CreateBuffer(&vertexDesc, &vertexData, &vertexBuffer_);
}

void FbxChild::InitIndexBuffer() {
	indexBuffer_.resize(materialCount_);
	materialIndexList_.resize(materialCount_);

	for (int i = 0; i < materialCount_; i++) {
		for (int polyCount = 0; polyCount < polygonCount_; polyCount++) {
			FbxLayerElementMaterial* materialList = mesh_->GetLayer(0)->GetMaterials();
			int materialIndex = materialList->GetIndexArray().GetAt(polyCount);
			if (materialIndex == i) {
				for (int vertexCount = 0; vertexCount < 3; vertexCount++) {
					materialIndexList_[i].push_back(polyCount * 3 + vertexCount);
				}
			}
		}

		D3D11_BUFFER_DESC indexDesc = {};
		indexDesc.Usage = D3D11_USAGE_DEFAULT;
		indexDesc.ByteWidth = sizeof(int) * materialIndexList_[i].size();
		indexDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

		D3D11_SUBRESOURCE_DATA indexData = {};
		indexData.pSysMem = materialIndexList_[i].data();

		GetDevice()->CreateBuffer(&indexDesc, &indexData, &indexBuffer_[i]);
	}
}

void FbxChild::InitConstantBuffer() {
	constantBufferList_.resize(materialCount_);
	for (int i = 0; i < materialCount_; i++) {
		D3D11_BUFFER_DESC constantDesc = {};
		constantDesc.Usage = D3D11_USAGE_DEFAULT;
		constantDesc.ByteWidth = sizeof(ConstantBuffer);
		constantDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		GetDevice()->CreateBuffer(&constantDesc, nullptr, &constantBufferList_[i]);
	}
}

void FbxChild::InitMaterial() {
	materialList_.resize(materialCount_);

	for (int i = 0; i < materialCount_; i++) {
		FbxSurfaceMaterial* material = node_->GetMaterial(i);
		FbxProperty property = material->FindProperty(FbxSurfaceMaterial::sDiffuse);
		int textureCount = property.GetSrcObjectCount<FbxFileTexture>();
		FbxSurfaceLambert* lambartMaterial = (FbxSurfaceLambert*)node_->GetMaterial(i);
		FbxSurfacePhong* phongMaterial = (FbxSurfacePhong*)node_->GetMaterial(i);
		FbxDouble3 diffuse = lambartMaterial->Diffuse;
		FbxDouble3 ambient = lambartMaterial->Ambient;
		FbxDouble shininess = phongMaterial->Shininess;
		FbxDouble3 emission = lambartMaterial->Emissive;
		FbxDouble3 specular = FbxDouble3(0.0, 0.0, 0.0);

		if (phongMaterial != nullptr) {
			specular = phongMaterial->Specular;
		}

		Material materialData = {};
		materialData.diffuse_ = { (float)diffuse[0], (float)diffuse[1], (float)diffuse[2], 1.0f };
		materialData.ambient_ = { (float)ambient[0], (float)ambient[1], (float)ambient[2], 1.0f };
		materialData.specular_ = { (float)specular[0], (float)specular[1], (float)specular[2], 1.0f };
		materialData.emission_ = { (float)emission[0], (float)emission[1], (float)emission[2] };
		materialData.shininess_ = shininess;

		if (textureCount > 0) {
			FbxFileTexture* fileTexture = property.GetSrcObject<FbxFileTexture>();
			Texture* texture = new Texture(fileTexture->GetFileName());
			texture->Init();
			materialData.texture_ = texture;
		}

		materialList_[i] = materialData;
	}
}

void FbxChild::InitBone() {
	FbxDeformer* deformer = mesh_->GetDeformer(0, FbxDeformer::eSkin);
	if (deformer == nullptr) {
		hasAnimation_ = false;
		return;
	}

	hasAnimation_ = true;
	skin_ = (FbxSkin*)deformer;
	clusterCount_ = skin_->GetClusterCount();

	for (int i = 0; i < vertexCount_; i++) {
		Weight weight = {};
		weight.location_ = vertexList_[i].location_;
		weight.boneIndex_.resize(clusterCount_, -1);
		weight.boneWeight_.resize(clusterCount_, 0.0f);

		weightList_.push_back(weight);
	}

	std::unordered_map<int, std::vector<int>> vertexToClusterMap;
	vertexToClusterMap.reserve(vertexCount_);
	for (int i = 0; i < (int)indexVertexList_.size(); i++) {
		int vertexIndex = indexVertexList_[i];
		vertexToClusterMap[vertexIndex].push_back(i);
	}

	for (int i = 0; i < clusterCount_; i++) {
		clusterList_.push_back(skin_->GetCluster(i));

		int controlPointCount = clusterList_[i]->GetControlPointIndicesCount();
		int* controlPointIndices = clusterList_[i]->GetControlPointIndices();
		double* weights = clusterList_[i]->GetControlPointWeights();

		for (int a = 0; a < controlPointCount; a++) {
			int indices = controlPointIndices[a];
			double weight = weights[a];

			auto vertexIndicesIterator = vertexToClusterMap.find(indices);
			if (vertexIndicesIterator == vertexToClusterMap.end()) {
				continue;
			}

			for (int vertexIndex : vertexIndicesIterator->second) {
				for (int j = 0; j < 4; j++) {
					if (j >= clusterCount_) {
						break;
					}

					if (weight > weightList_[vertexIndex].boneWeight_[j]) {
						weightList_[vertexIndex].boneWeight_[j] = (float)weight;
						weightList_[vertexIndex].boneIndex_[j] = i;
						break;
					}
				}
			}

		}

		boneList_.resize(clusterCount_);
		FbxAMatrix transformMatrix = {};
		clusterList_[i]->GetTransformLinkMatrix(transformMatrix);

		XMFLOAT4X4 offsetMatrix = {};
		for (int x = 0; x < 4; x++) {
			for (int y = 0; y < 4; y++) {
				offsetMatrix.m[x][y] = (float)transformMatrix.Get(x, y);
			}
		}

		boneList_[i].offsetMatrix_ = XMLoadFloat4x4(&offsetMatrix);
		boneMap_[clusterList_[i]->GetLink()->GetName()] = &boneList_[i];
	}
}
