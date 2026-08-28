#include "DebugScene.h"
#include "ObjectManager.h"
#include "FBX.h"
#include "SoundManager.h"
#include "Image.h"
#include "ImageManager.h"
#include "DX3DManager.h"
#include "VideoObject.h"

using namespace DX3DManager;

void DebugScene::Init() {
	FBX* grid = new FBX("MochinekoEngine/Asset/grid.fbx", false, true);
	grid->SetTag("Grid");
	ObjectManager::AddObject(grid);

	FBX* gizmo = new FBX("MochinekoEngine/Asset/gizmo.fbx", false, false);
	gizmo->SetTag("Gizmo");
	gizmo->AddChild(grid);
	ObjectManager::AddObject(gizmo);

	//VideoObject* video = new VideoObject("a.mp4");
	//ObjectManager::AddObject(video);
}

void DebugScene::Update() {
}

void DebugScene::Draw() {
}
