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
	// モデル plane
	std::unique_ptr<Object3D> plane_;

	// 描画タイプ
	PipelineType planeDrawType_;
	// ブレンドモード
	BlendMode planeBlendMode_;

	// モデルの名前
	std::string planeModelName_;

	// テクスチャの名前
	std::string planeTextureName_;

	/*----------------------------------------------------------------------*/
	// モデル fence

	std::unique_ptr<Object3D> fence_;

	// 描画タイプ
	PipelineType fenceDrawType_;
	// ブレンドモード
	BlendMode fenceBlendMode_;

	// モデルの名前
	std::string fenceModelName_;

	// テクスチャの名前
	std::string fenceTextureName_;

};