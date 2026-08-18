#pragma once
#include "BaseObject.h"
#include <d3d11.h>
#include <vector>
#include "DX3DManager.h"
#include "fbxsdk.h"
#include "Texture.h"
#include <map>
#include "FbxChild.h"

#pragma comment(lib, "LibFbxSDK-MD.lib")
#pragma comment(lib, "LibXml2-MD.lib")
#pragma comment(lib, "zlib-MD.lib")

struct Material {
	DirectX::XMFLOAT4 ambient_ = {};
	DirectX::XMFLOAT4 diffuse_ = {};
	DirectX::XMFLOAT4 specular_ = {};
	DirectX::XMFLOAT3 emission_ = {};
	float shininess_ = 0.0;
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
	bool zDepthWrite_;
	bool wireframe_;

	std::string fileName_;
	fbxsdk::FbxNode* rootNode_;

	std::vector<FbxChild*> childFbx_;
private:
	void InitFBX();
public:

	FBX(const std::string fileName, const bool zDepthWrite = true, const bool wireFrame = false)
		: BaseObject("FBX") {
		fileName_ = fileName;
		rootNode_ = nullptr;
		zDepthWrite_ = zDepthWrite;
		wireframe_ = wireFrame;
	}

	~FBX() {

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
};