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
	// plane

	// インスタンス
	std::unique_ptr<Object3D> plane_;

	// 描画タイプ
	PipelineType planeDrawType_;

	// ブレンドモード
	BlendMode planeBlendMode_;

	// モデルの名前
	std::string planeModelName_;
	// テクスチャの名前
	std::string planeTextureName_;

};