#include "DebugScene.h"
#include "ObjectManager.h"
#include "FBX.h"
#include "SoundManager.h"
#include "Image.h"
#include "ImageManager.h"
#include "DX3DManager.h"

using namespace DX3DManager;

void DebugScene::Init() {
	FBX* grid = new FBX("grid.fbx", false, true);
	grid->SetTag("Grid");
	ObjectManager::AddObject(grid);

	FBX* gizmo = new FBX("gizmo.fbx", false, false);
	gizmo->SetTag("Gizmo");
	ObjectManager::AddObject(gizmo);
}

void DebugScene::Update() {
}

void DebugScene::Draw() {
}
