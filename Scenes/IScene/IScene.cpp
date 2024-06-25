#include "IScene.h"

// sceneNo_ getter
SceneNo IScene::GetSceneNo() const {

	return sceneNo_;
}

// ゲームシーンで初期化
SceneNo IScene::sceneNo_ = SceneNo::GAME;