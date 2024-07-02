#include "GameScene.h"

#include "ImGuiManager.h"
#include "TextureManager.h"
#include "ModelManager.h"


/*////////////////////////////////////////////////////////////////////////////////
*								コンストラクタ
////////////////////////////////////////////////////////////////////////////////*/
GameScene::GameScene() {

	/*======================================================*/
	// 2Dカメラ

	// 生成
	camera2D_ = std::make_unique<Camera2D>();

	// 初期化
	camera2D_->Initialize();

	/*======================================================*/
	// 3Dカメラ

	// 生成
	camera3D_ = std::make_unique<Camera3D>();

	// 初期化
	camera3D_->Initialize();

	/*======================================================*/
	// 2Dオブジェクト



	/*======================================================*/
	// 3Dオブジェクト

	/*--------------------------------------------*/
	/* 三角形 */

	// 生成
	triangle_ = std::make_unique<Object3D>();

	// 初期化
	triangle_->Initialize(
		camera3D_.get(), Object3DType::Triangle, GS, kBlendModeNormal,
		"uvChecker.png", "");

	/*--------------------------------------------*/
	/* plane */

	// 生成
	plane_ = std::make_unique<Object3D>();

	// 初期化
	plane_->Initialize(
		camera3D_.get(), Object3DType::GS, GS, kBlendModeNormal,
		"uvChecker.png", "");

}

/*////////////////////////////////////////////////////////////////////////////////
*								  デストラクタ
////////////////////////////////////////////////////////////////////////////////*/
GameScene::~GameScene() {

	camera2D_.reset();
	camera3D_.reset();
	plane_.reset();
	triangle_.reset();
}


/*////////////////////////////////////////////////////////////////////////////////
*								    更新処理
////////////////////////////////////////////////////////////////////////////////*/
void GameScene::Initialize() {

	/*======================================================*/
	// 2Dカメラ

	camera2D_->Initialize();

	/*======================================================*/
	// 3Dカメラ

	camera3D_->Initialize();

	/*======================================================*/
	// 2Dオブジェクト



	/*======================================================*/
	// 3Dオブジェクト



}



/*////////////////////////////////////////////////////////////////////////////////
*								    更新処理
////////////////////////////////////////////////////////////////////////////////*/
void GameScene::Update() {

	/*======================================================*/
	// ImGui



	/*======================================================*/
	// 2Dカメラ

	camera2D_->Update();

	/*======================================================*/
	// 3Dカメラ

	camera3D_->Update();

	/*======================================================*/
	// 2Dオブジェクト



	/*======================================================*/
	// 3Dオブジェクト

	/*--------------------------------------------*/
	/* 三角形 */

	//triangle_->UpdateImGui("triangle");
	triangle_->Update(camera3D_.get());

	/*--------------------------------------------*/
	/* plane */

	plane_->UpdateImGui("gs");
	plane_->Update(camera3D_.get());

}

/*////////////////////////////////////////////////////////////////////////////////
*								    描画処理
////////////////////////////////////////////////////////////////////////////////*/
void GameScene::Draw() {

	/*======================================================*/
	// 2Dオブジェクト



	/*======================================================*/
	// 3Dオブジェクト

	/*--------------------------------------------*/
	/* 三角形 */

	//triangle_->Draw();

	/*--------------------------------------------*/
	/* plane */

	plane_->Draw();

}