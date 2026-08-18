#include "SceneManager.h"
#include <map>
#include <string>
#include "BaseScene.h"
#include "DebugScene.h"
#include "ObjectManager.h"

namespace SceneManager {
	std::map<std::string, BaseScene*> sceneMap_;	// シーンの名前とポインタ
	BaseScene* currentScene_ = nullptr;				// 現在のシーン
}

void SceneManager::Init() {
	sceneMap_.clear();
	AddScene(new DebugScene());

	ChangeScene("DebugScene");
}

void SceneManager::Update() {
	if (currentScene_ != nullptr) {
		currentScene_->Update();
		currentScene_->Draw();
	}
}

void SceneManager::Release() {
}

bool SceneManager::AddScene(BaseScene* scene) {
	if (sceneMap_.find(scene->GetName()) == sceneMap_.end()) {	// シーンが存在しない場合
		sceneMap_[scene->GetName()] = scene;	// シーンをマップに追加する
		return true;
	}
	return false;	// シーンが既に存在する場合はfalseを返す
}

BaseScene* SceneManager::GetCurrentScene() {
	return currentScene_;
}

bool SceneManager::ChangeScene(const std::string& sceneName) {
	if (sceneMap_.find(sceneName) != sceneMap_.end()) {	// シーンが見つかった場合
		ObjectManager::ClearAllObject();			// シーンを切り替える前に、全てのオブジェクトを削除する
		currentScene_ = sceneMap_[sceneName];		// 現在のシーンを切り替える
		currentScene_->Init();						// 現在のシーンを初期化する
		return true;
	}

	return false;	// シーンが見つからなかった場合はfalseを返す
}
