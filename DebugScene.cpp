#include "DebugScene.h"
#include "ObjectManager.h"
#include "FBX.h"
#include "SoundManager.h"
#include "Image.h"
#include "ImageManager.h"

void DebugScene::Init() {
}

void DebugScene::Update() {
}

void DebugScene::Draw() {
	ImageManager::Draw(imageHandle_);
}
