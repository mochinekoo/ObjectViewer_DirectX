#include "DebugScene.h"
#include "ObjectManager.h"
#include "FBX.h"
#include "SoundManager.h"
#include "Image.h"
#include "ImageManager.h"
#include "DX3DManager.h"

using namespace DX3DManager;

void DebugScene::Init() {
	FBX* fbx = new FBX("grid.fbx", false, true);
	fbx->SetTag("Grid");
	ObjectManager::AddObject(fbx);
}

void DebugScene::Update() {
}

void DebugScene::Draw() {
}
