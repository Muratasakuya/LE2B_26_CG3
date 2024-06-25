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

	/*------------------------------------------------------*/
	// モデル plane

	// 生成
	plane_ = std::make_unique<Object3D>(Object3DType::Model);

	// 初期化
	plane_->SetEnableLighting(false);
	plane_->SetEnableHalfLambert(true);
	plane_->Initilize(camera3D_.get());

	// Y軸を180度回転
	plane_->SetRotate({ 0.0f,std::numbers::pi_v<float>,0.0f });

	// 描画タイプ
	planeDrawType_ = Texture;
	// ブレンドモード
	planeBlendMode_ = kBlendModeNormal;

	// テクスチャ読み込み
	TextureManager::GetInstance()->LoadTexture("./Resources/Images/uvChecker.png");
	planeTextureName_ = "uvChecker";

	// モデル読み込み
	ModelManager::GetInstance()->LoadModel("./Resources/Obj", "plane.obj");
	planeModelName_ = "plane";

	/*------------------------------------------------------*/
	// モデル fence

	// 生成
	fence_ = std::make_unique<Object3D>(Object3DType::Model);

	// 初期化
	fence_->SetEnableLighting(false);
	fence_->SetEnableHalfLambert(true);
	fence_->Initilize(camera3D_.get());

	fence_->SetRotate({ std::numbers::pi_v<float> / 6.0f,std::numbers::pi_v<float>,0.0f });

	// 描画タイプ
	fenceDrawType_ = Texture;
	// ブレンドモード
	fenceBlendMode_ = kBlendModeNormal;

	// テクスチャ読み込み
	TextureManager::GetInstance()->LoadTexture("./Resources/Images/fence.png");
	fenceTextureName_ = "fence";

	// モデル読み込み
	ModelManager::GetInstance()->LoadModel("./Resources/Obj", "fence.obj");
	fenceModelName_ = "fence";
}

/*////////////////////////////////////////////////////////////////////////////////
*								  デストラクタ
////////////////////////////////////////////////////////////////////////////////*/
GameScene::~GameScene() {

	camera2D_.reset();
	camera3D_.reset();
	plane_.reset();
	fence_.reset();
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

	// plane
	plane_->Initilize(camera3D_.get());

	// fence
	fence_->Initilize(camera3D_.get());

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



	/*======================================================*/
	// 3Dオブジェクト

	// BlendModeの文字列配列
	const char* BlendModeNames[] = {

		"None",
		"Normal",
		"Add",
		"Subtract",
		"Multiply",
		"Screen"
	};

	ImGui::Begin("fence");

	// BlendModeのドロップダウン
	int currentBlendMode = static_cast<int>(fenceBlendMode_);

	if (ImGui::Combo("Blend Mode", &currentBlendMode, BlendModeNames, IM_ARRAYSIZE(BlendModeNames))) {

		fenceBlendMode_ = static_cast<BlendMode>(currentBlendMode);
	}

	ImGui::End();

	// plane
	fence_->Update(camera3D_.get());

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

	// fence
	fence_->Draw(fenceDrawType_, fenceBlendMode_, fenceTextureName_, fenceModelName_);

}