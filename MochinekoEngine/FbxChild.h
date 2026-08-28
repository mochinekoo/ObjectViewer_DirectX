#pragma once
#include "BaseObject.h"
#include <d3d11.h>
#include <vector>
#include "DX3DManager.h"
#include "fbxsdk.h"
#include "Texture.h"
#include <map>

class FBX;
struct Material;
struct Weight;
struct Bone;

class FbxChild : public BaseObject {
private:
	bool zDepthWrite_;
	bool wireframe_;

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

	FbxChild(FBX* fbx, FbxNode* node, const bool zDepthWrite = true, const bool wireFrame = false)
		: BaseObject("FBX") {
		parent_ = (BaseObject*) fbx;
		node_ = node;
		vertexBuffer_ = nullptr;
		indexBuffer_.clear();
		materialIndexList_.clear();
		constantBufferList_.clear();
		vertexList_.clear();
		materialList_.clear();
		mesh_ = nullptr;
		node_ = node;
		vertexCount_ = 0;
		polygonCount_ = 0;
		materialCount_ = 0;
		animation_ = true;
		maxFrame_ = 60;
		hasAnimation_ = false;
		zDepthWrite_ = zDepthWrite;
		wireframe_ = wireFrame;
	}

	~FbxChild() {

	}

	void Init() override;
	void Update() override;
	void Draw() override;
	void DrawImGUI() override;
	void Release() override;

	bool IsZDepthWrite() const { return zDepthWrite_; }
	void SetZDepthWrite(const bool flag) { zDepthWrite_ = flag; }
	bool IsWireframe() const { return wireframe_; }
	void SetWireframe(const bool flag) { wireframe_ = flag; }
	bool HasAnimation() const { return hasAnimation_; }
};

