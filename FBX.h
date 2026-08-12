#pragma once
#include "BaseObject.h"
#include <d3d11.h>
#include <vector>
#include "DX3DManager.h"
#include "fbxsdk.h"
#include "Texture.h"
#include <map>

#pragma comment(lib, "LibFbxSDK-MD.lib")
#pragma comment(lib, "LibXml2-MD.lib")
#pragma comment(lib, "zlib-MD.lib")

struct Material {
	DirectX::XMFLOAT4 ambient_ = {};
	DirectX::XMFLOAT4 diffuse_ = {};
	DirectX::XMFLOAT4 specular_ = {};
	Texture* texture_ = nullptr;
};

struct Bone {
	DirectX::XMMATRIX offsetMatrix_ = {};		// ボーンの初期位置
	DirectX::XMMATRIX newBoneMatrix_ = {};		// ボーンの現在位置
	DirectX::XMMATRIX differenceMatrix_ = {};	// ボーンの差分
};

struct Weight {
	DirectX::XMFLOAT3 location_ = {};	// 頂点の位置
	std::vector<int> boneIndex_;		// ボーンのインデックス
	std::vector<float> boneWeight_;		// ボーンの重み
};

class FBX : public BaseObject {
private:
	std::string fileName_;
	ID3D11Buffer* vertexBuffer_;
	std::vector<ID3D11Buffer*> indexBuffer_;

	std::vector<std::vector<int>> materialIndexList_;
	std::vector<ID3D11Buffer*> constantBufferList_;
	std::vector<int> indexVertexList_;
	std::vector<Vertex> vertexList_;
	std::vector<Material> materialList_;
	std::vector<fbxsdk::FbxCluster*> clusterList_;
	std::vector<Weight> weightList_;
	std::vector<Bone> boneList_;
	std::map<std::string, Bone*> boneMap_;

	fbxsdk::FbxMesh* mesh_;
	fbxsdk::FbxNode* node_;
	fbxsdk::FbxSkin* skin_;

	unsigned int vertexCount_;
	unsigned int polygonCount_;
	unsigned int materialCount_;
	unsigned int clusterCount_;

	bool hasAnimation_;
	bool animation_;
	unsigned int maxFrame_;
	FbxTime fbxTime_;

private:

	void InitFBX();
	void InitVertexBuffer();
	void InitIndexBuffer();
	void InitConstantBuffer();
	void InitMaterial();
	void InitBone();

public:

	FBX(const std::string fileName)
		: BaseObject("FBX") {
		fileName_ = fileName;
		vertexBuffer_ = nullptr;
		indexBuffer_.clear();
		materialIndexList_.clear();
		constantBufferList_.clear();
		vertexList_.clear();
		materialList_.clear();
		mesh_ = nullptr;
		node_ = nullptr;
		vertexCount_ = 0;
		polygonCount_ = 0;
		materialCount_ = 0;
		animation_ = true;
		maxFrame_ = 60;
	}

	~FBX() {

	}

	void Init() override;
	void Update() override;
	void Draw() override;
	void Release() override;
};