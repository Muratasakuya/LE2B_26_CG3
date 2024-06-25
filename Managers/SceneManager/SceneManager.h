#pragma once
#include "Engine.h"

// Base
#include "IScene.h"

// from IScene
#include "GameScene.h"

// c++
#include <memory>

/*////////////////////////////////////////////////////////////////////////////////
*
*								SceneManager Class
*
////////////////////////////////////////////////////////////////////////////////*/
class SceneManager {
public:
	/*-----------------------------*/
	///			メンバ関数
	/*-----------------------------*/

	SceneManager();
	~SceneManager();

	void Run();

private:
	/*-----------------------------*/
	///			メンバ変数
	/*-----------------------------*/

	std::unique_ptr<IScene> sceneArr_[1];

	SceneNo currentSceneNo_{};
	SceneNo prevSceneNo_{};

};