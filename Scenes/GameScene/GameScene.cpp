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

	// ポイントライト
	pointLight_.color = { 1.0f,1.0f,1.0f,1.0f };
	pointLight_.pos = { 0.0f,2.0f,0.0f };
	pointLight_.intensity = 0.0f;
	pointLight_.radius = 5.0f;
	pointLight_.decay = 1.0f;

	// スポットライト
	spotLight_.color = { 1.0f,1.0f,1.0f,1.0f };
	spotLight_.pos = { 2.0f,1.25f,0.0f };
	spotLight_.distance = 7.0f;
	spotLight_.direction = Vector3::Normalize({ -1.0f,-1.0f,0.0 });
	spotLight_.intensity = 4.0f;
	spotLight_.decay = 2.0f;
	spotLight_.cosAngle = std::cos(std::numbers::pi_v<float> / 3.0f);
	spotLight_.cosFalloffStart = 1.0f;

	/*--------------------------------------------*/
	/*  Sphere 球  */

	// 生成
	sphere_ = std::make_unique<Object3D>(Object3DType::Sphere);

	// 初期化
	sphere_->Initialize(camera3D_.get());

	// Lighting設定
	sphere_->SetEnableLighting(true);
	sphere_->SetEnableBlinnPhongReflection(true);

	// 描画タイプ
	sphereDrawType_ = PhongReflection;

	// ブレンドモード
	sphereBlendMode_ = kBlendModeNormal;

	// テクスチャ読み込み
	TextureManager::GetInstance()->LoadTexture("./Resources/Images/monsterBall.png");
	sphereTextureName_ = "monsterBall";

	/*--------------------------------------------*/
	/*  terrain 地形  */

	// 生成
	terrain_ = std::make_unique<Object3D>(Object3DType::Model);

	// 初期化
	terrain_->Initialize(camera3D_.get());

	// Lighting設定
	terrain_->SetEnableLighting(true);
	terrain_->SetEnableBlinnPhongReflection(true);

	// 描画タイプ
	terrainDrawType_ = PhongReflection;

	// ブレンドモード
	terrainBlendMode_ = kBlendModeNormal;

	// モデル読み込み
	ModelManager::GetInstance()->LoadModel("./Resources/Obj", "terrain.obj");
	terrainModelName_ = "terrain";

	// テクスチャ読み込み
	TextureManager::GetInstance()->LoadTexture("./Resources/Images/grass.png");
	terrainTextureName_ = "grass";

}

/*////////////////////////////////////////////////////////////////////////////////
*								  デストラクタ
////////////////////////////////////////////////////////////////////////////////*/
GameScene::~GameScene() {

	camera2D_.reset();
	camera3D_.reset();
	sphere_.reset();
	terrain_.reset();
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

	// 球
	sphere_->Initialize(camera3D_.get());

	// 地形
	terrain_->Initialize(camera3D_.get());

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

	ImGui::Begin("PointLight");

	ImGui::ColorEdit3("color", &pointLight_.color.x);
	ImGui::DragFloat3("pos", &pointLight_.pos.x, 0.05f, -10.0f, 10.0f);
	ImGui::DragFloat("intensity", &pointLight_.intensity, 0.05f);
	ImGui::DragFloat("radius", &pointLight_.radius, 0.01f);
	ImGui::DragFloat("decay", &pointLight_.decay, 0.01f, 0.0f, 20.0f);

	ImGui::End();

	ImGui::Begin("SpotLight");

	ImGui::ColorEdit3("color", &spotLight_.color.x);
	ImGui::DragFloat3("pos", &spotLight_.pos.x, 0.05f, -10.0f, 10.0f);
	ImGui::DragFloat("intensity", &spotLight_.intensity, 0.05f);
	ImGui::DragFloat("distance", &spotLight_.distance, 0.01f);
	ImGui::DragFloat("cosFalloffStart", &spotLight_.cosFalloffStart, 0.01f, 0.5f, 5.0f);
	ImGui::DragFloat("decay", &spotLight_.decay, 0.01f, 0.0f, 20.0f);

	ImGui::End();

	// 減衰率の制御
	if (pointLight_.decay <= 0.0f) {

		pointLight_.decay = 0.0f;
	}

	if (spotLight_.decay <= 0.0f) {

		spotLight_.decay = 0.0f;
	}

	// 球
	sphere_->UpdateImGui("sphere");
	sphere_->SetPointLight(pointLight_);
	sphere_->SetSpotLight(spotLight_);
	sphere_->Update(camera3D_.get());

	// 地形
	terrain_->UpdateImGui("terrain");
	terrain_->SetPointLight(pointLight_);
	terrain_->SetSpotLight(spotLight_);
	terrain_->Update(camera3D_.get());

}

/*////////////////////////////////////////////////////////////////////////////////
*								    描画処理
////////////////////////////////////////////////////////////////////////////////*/
void GameScene::Draw() {

	// 3DCameraのImGui
	camera3D_->ImGuiDraw();

	/*======================================================*/
	// 2Dオブジェクト



	/*======================================================*/
	// 3Dオブジェクト

	// 地形
	terrain_->Draw(terrainDrawType_, terrainBlendMode_, terrainTextureName_, terrainModelName_);

	// 球
	sphere_->Draw(sphereDrawType_, sphereBlendMode_, sphereTextureName_);

}