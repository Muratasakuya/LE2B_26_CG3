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

	/*------------------------------------------------------*/
	// スプライト

	// 生成
	sprite_ = std::make_unique<Object2D>();

	// 初期化
	sprite_->Initilize(camera2D_.get());

	// テクスチャ読み込み
	TextureManager::GetInstance()->LoadTexture("./Resources/Images/uvChecker.png");
	spriteTextureName_ = "uvChecker";

	/*======================================================*/
	// 3Dオブジェクト

	/*------------------------------------------------------*/
	// 三角形

	// 生成
	for (auto& triangle : triangles_) {

		triangle = std::make_unique<Object3D>(Object3DType::Triangle);

		triangle->SetEnableLighting(false);
		triangle->SetEnableHalfLambert(false);
		triangle->Initilize(camera3D_.get());
	}

	// ずらして描画
	triangles_[0]->SetRotate({ 0.0f,std::numbers::pi_v<float> / 3.0f,0.0f });

	// 描画タイプ
	triangleDrawType_ = Texture;

	// テクスチャ読み込み
	TextureManager::GetInstance()->LoadTexture("./Resources/Images/uvChecker.png");
	triangleTextureName_ = "uvChecker";

	/*------------------------------------------------------*/
	// 球

	// 生成
	sphere_ = std::make_unique<Object3D>(Object3DType::Sphere);

	// 初期化
	sphere_->SetEnableLighting(true);
	sphere_->SetEnableHalfLambert(true);
	sphere_->Initilize(camera3D_.get());

	// 描画タイプ
	sphereDrawType_ = Texture;

	// テクスチャ読み込み
	TextureManager::GetInstance()->LoadTexture("./Resources/Images/uvChecker.png");
	sphereTextureName_ = "uvChecker";

	/*------------------------------------------------------*/
	// モデル plane

	// 生成
	plane_ = std::make_unique<Object3D>(Object3DType::Model);

	// 初期化
	plane_->SetEnableLighting(true);
	plane_->SetEnableHalfLambert(true);
	plane_->Initilize(camera3D_.get());

	// Y軸を180度回転
	plane_->SetRotate({ 0.0f,std::numbers::pi_v<float>,0.0f });

	// 描画タイプ
	planeDrawType_ = Texture;

	// テクスチャ読み込み
	TextureManager::GetInstance()->LoadTexture("./Resources/Images/uvChecker.png");
	planeTextureName_ = "uvChecker";

	// モデル読み込み
	ModelManager::GetInstance()->LoadModel("./Resources/Obj", "plane.obj");
	planeModelName_ = "plane";
}

/*////////////////////////////////////////////////////////////////////////////////
*								  デストラクタ
////////////////////////////////////////////////////////////////////////////////*/
GameScene::~GameScene() {}



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

	// スプライト
	sprite_->Initilize(camera2D_.get());

	/*======================================================*/
	// 3Dオブジェクト

	// 三角形
	for (const auto& triangle : triangles_) {

		triangle->Initilize(camera3D_.get());
	}

	// 球
	sphere_->Initilize(camera3D_.get());

	// plane
	plane_->Initilize(camera3D_.get());

}



/*////////////////////////////////////////////////////////////////////////////////
*								    更新処理
////////////////////////////////////////////////////////////////////////////////*/
void GameScene::Update() {

	/*======================================================*/
	// 2Dカメラ

	camera2D_->Update();

	/*======================================================*/
	// 3Dカメラ

	camera3D_->Update();

	/*======================================================*/
	// 2Dオブジェクト

	// スプライト
	//sprite_->Update(camera2D_.get());

	/*======================================================*/
	// 3Dオブジェクト

	// 三角形
	/*for (const auto& triangle : triangles_) {

		triangle->Update(camera3D_.get());
	}*/

	// 球
	//sphere_->Update(camera3D_.get());

	// plane
	plane_->Update(camera3D_.get());

}

/*////////////////////////////////////////////////////////////////////////////////
*								    描画処理
////////////////////////////////////////////////////////////////////////////////*/
void GameScene::Draw() {

	/*======================================================*/
	// 2Dオブジェクト

	// スプライト
	//sprite_->Draw(spriteTextureName_);

	/*======================================================*/
	// 3Dオブジェクト

	// 三角形
	/*for (const auto& triangle : triangles_) {

		triangle->Draw(triangleDrawType_, triangleTextureName_);
	}*/

	// 球
	//sphere_->Draw(sphereDrawType_, sphereTextureName_);

	// plane
	plane_->Draw(planeDrawType_, planeTextureName_, planeModelName_);

}