#pragma once
#include <string>
#include "BaseScene.h"

/// <summary>
/// シーンを管理する名前空間
/// </summary>
namespace SceneManager {

	void Init();
	void Update();
	void Release();

	/// <summary>
	/// シーンを追加する関数。
	/// </summary>
	/// <param name="scene">追加したいシーン</param>
	/// <returns>変更に成功した場合はtrue、失敗した場合はfalseを返します。</returns>
	bool AddScene(BaseScene* scene);

	/// <summary>
	/// 現在のシーンを取得する関数
	/// </summary>
	/// <returns>現在のシーンをポインタで返します。</returns>
	BaseScene* GetCurrentScene();

	/// <summary>
	/// シーンを変更する関数
	/// </summary>
	/// <param name="sceneName">シーンの名前</param>
	/// <returns>変更に成功した場合はtrue、失敗した場合はfalseを返します。</returns>
	bool ChangeScene(const std::string& sceneName);
}