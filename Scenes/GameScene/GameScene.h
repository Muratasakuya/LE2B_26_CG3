#pragma once

// Base
#include "IScene.h"

// Lib
#include "Camera2D.h"
#include "Camera3D.h"

// 2D
#include "Object2D.h"

// 3D
#include "Object3D.h"

// c++
#include <memory>
#include <array>
#include <vector>
#include <string>
#include <numbers>

/*////////////////////////////////////////////////////////////////////////////////
*
*								GameScene Class
*
////////////////////////////////////////////////////////////////////////////////*/
class GameScene :
	public IScene {
public:
	/*-----------------------------*/
	///			メンバ関数
	/*-----------------------------*/

	GameScene();
	~GameScene();

	void Initialize()override;
	void Update()override;
	void Draw()override;

private:
	/*-----------------------------*/
	///			メンバ変数
	/*-----------------------------*/

	/*----------------------------------------------------------------------*/
	// カメラ

	std::unique_ptr<Camera2D> camera2D_;
	std::unique_ptr<Camera3D> camera3D_;

	/*----------------------------------------------------------------------*/
	// 2Dオブジェクト



	/*----------------------------------------------------------------------*/
	// 3Dオブジェクト

	/*----------------------------------------------------------------------*/
	/* 三角形 */

	// インスタンス
	std::unique_ptr<Object3D> triangle_;

	/*----------------------------------------------------------------------*/
	// 3Dモデル

	/*----------------------------------------------------------------------*/
	/* plane */

	// インスタンス
	std::unique_ptr<Object3D> plane_;

};