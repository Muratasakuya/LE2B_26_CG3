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

// 三角形のインスタンスの数
const uint32_t triangleNum = 2;

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

	// スプライト
	std::unique_ptr<Object2D> sprite_;

	// テクスチャの名前
	std::string spriteTextureName_;

	/*----------------------------------------------------------------------*/
	// 3Dオブジェクト

	/*----------------------------------------------------------------------*/
	// 三角形
	std::array<std::unique_ptr<Object3D>, triangleNum> triangles_;

	// 描画タイプ
	PipelineType triangleDrawType_;

	// テクスチャの名前
	std::string triangleTextureName_;

	/*----------------------------------------------------------------------*/
	// 球
	std::unique_ptr<Object3D> sphere_;

	// 描画タイプ
	PipelineType sphereDrawType_;

	// テクスチャの名前
	std::string sphereTextureName_;

	/*----------------------------------------------------------------------*/
	// モデル teapot
	std::unique_ptr<Object3D> teapot_;

	// 描画タイプ
	PipelineType teapotDrawType_;

	// モデルの名前
	std::string teapotModelName_;

	// テクスチャの名前
	std::string teapotTextureName_;

};