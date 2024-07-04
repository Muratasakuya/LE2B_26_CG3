#include "Object3D.h"

// Base
#include "Engine.h"

#include "Camera3D.h"
#include "ImGuiManager.h"
#include "ModelManager.h"
#include "TextureManager.h"
#include "ConstBufferManager.h"



// isUseGltfModel_ = useGltfModel, gltfModelName_ = modelName setter
void Object3D::SetIsUseGLTFModel(bool useGltfModel, const std::string& modelName) {

	isUseGltfModel_ = useGltfModel;
	if (isUseGltfModel_) {

		gltfModelName_ = modelName;
	}
}



/*////////////////////////////////////////////////////////////////////////////////
*								コンストラクタ
////////////////////////////////////////////////////////////////////////////////*/
Object3D::Object3D() {}

/*////////////////////////////////////////////////////////////////////////////////
*								 デストラクタ
////////////////////////////////////////////////////////////////////////////////*/
Object3D::~Object3D() {}



/*////////////////////////////////////////////////////////////////////////////////
*									初期化
////////////////////////////////////////////////////////////////////////////////*/
void Object3D::Initialize(
	Camera3D* camera, const Object3DType& objectType, const PipelineType& drawType, const BlendMode& blendMode,
	const std::string& textureName, const std::optional<std::string>& modelName) {

	// 3Dオブジェクトタイプのセット
	objectType_ = objectType;

	// 描画タイプのセット
	drawType_ = drawType;

	// ブレンドの設定
	blendMode_ = blendMode;

#pragma region /// DefaultInitialize ///

	/*--------------------------------------------------------------------------*/
	/// LightDefaultInitialize ↓

	// 平行光源
	light_.directional.color = { 1.0f,1.0f,1.0f,1.0f };
	// デフォルトは上から
	light_.directional.direction = { 0.0f,-1.0f,0.0f };
	light_.directional.intensity = 1.0f;

	// ポイントライト
	light_.point.color = { 1.0f,1.0f,1.0f,1.0f };
	// デフォルトは上から
	light_.point.pos = { 0.0f,1.0f,0.0f };
	light_.point.intensity = 1.0f;
	light_.point.radius = 5.0f;
	light_.point.decay = 1.0f;

	// スポットライト
	light_.spot.color = { 1.0f,1.0f,1.0f,1.0f };
	light_.spot.pos = { 0.0f,0.0f,0.0f };
	light_.spot.distance = 5.0f;
	// デフォルトは右から
	light_.spot.direction = Vector3::Normalize({ -1.0f,-1.0f,0.0 });
	light_.spot.intensity = 1.0f;
	light_.spot.decay = 1.0f;
	light_.spot.cosAngle = std::cos(std::numbers::pi_v<float> / 3.0f);
	light_.spot.cosFalloffStart = 1.0f;

	/*--------------------------------------------------------------------------*/
	/// MaterialDefaultInitialize ↓

	// オブジェクトの色
	material_.color = { 1.0f,1.0f,1.0f,1.0f };
	// デフォルトはLightingなし
	material_.enableLighting = false;
	material_.enableHalfLambert = false;
	material_.uvTransform = Matrix4x4::MakeIdentity4x4();

	// PhongReflectionの色
	phongRef_.color = { 1.0f,1.0f,1.0f,1.0f };
	// デフォルトはLightingなし
	phongRef_.enableLighting = false;
	phongRef_.enablePhongReflection = false;
	phongRef_.enableBlinnPhongReflection = false;
	phongRef_.uvTransform = Matrix4x4::MakeIdentity4x4();
	phongRef_.specularColor = { 1.0f,1.0f,1.0f };
	phongRef_.shininess = 32.0f;

	/*--------------------------------------------------------------------------*/

	// SRT
	transform_.scale = { 1.0f,1.0f,1.0f };
	transform_.rotate = { 0.0f,0.0f,0.0f };
	transform_.translate = { 0.0f,0.0f,0.0f };

	// Matrixの計算
	matrix_.World =
		Matrix4x4::MakeAffineMatrix(transform_.scale, transform_.rotate, transform_.translate);
	matrix_.WVP =
		Matrix4x4::Multiply(matrix_.World, Matrix4x4::Multiply(camera->GetViewMatrix(), camera->GetProjectionMatrix()));
	matrix_.WorldInverseTranspose =
		Matrix4x4::Transpose(Matrix4x4::Inverse(matrix_.World));
	// GSMatrixの計算
	gsMat.gsMatrix_ = Matrix4x4::Multiply(matrix_.World, Matrix4x4::Multiply(camera->GetViewMatrix(), camera->GetProjectionMatrix()));

	// デフォルトはfalse
	isUseGltfModel_ = false;
	gltfModelName_ = "";

	// カメラワールド座標の取得
	cameraWorldPos_ = camera->GetWorldPos();

#pragma endregion

	// テクスチャの読み込み
	TextureManager::GetInstance()->LoadTexture("./Resources/Images/" + textureName);
	// ファイルパスからテクスチャ名を取得
	std::filesystem::path texturePath(textureName);
	// 拡張子を除いたテクスチャ名を取得
	std::string identifierTexture = texturePath.stem().string();
	textureName_ = identifierTexture;

	if (modelName != "") {

		// モデルの読み込み
		ModelManager::GetInstance()->LoadModel("./Resources/Obj", *modelName);
		// ファイルパスからモデル名を取得
		std::filesystem::path modelPath(*modelName);
		// 拡張子を除いたモデル名を取得
		std::string identifierModel = modelPath.stem().string();
		modelName_ = identifierModel;
	}
}



/*////////////////////////////////////////////////////////////////////////////////
*							CBufferの初期化
////////////////////////////////////////////////////////////////////////////////*/
void Object3D::InitializeCBuffer(const std::string& name) {

	name_ = name;

	cBuffer_ = ConstBufferManager::GetInstance();

	// ConstBufferの初期化
	cBuffer_->CreateConstBufferData(name_);
	cBuffer_->InitializeConstBuffer(
		name_, material_, phongRef_, matrix_, gsMat, light_, cameraWorldPos_, drawType_, isUseGltfModel_);
}



/*////////////////////////////////////////////////////////////////////////////////
*								ImGui更新処理
////////////////////////////////////////////////////////////////////////////////*/
void Object3D::UpdateImGui(const std::string& objectName) {

	ImGui::Begin(objectName.c_str());

	ImGui::SliderFloat2("scale", &transform_.scale.x, 0.0f, 3.0f);
	ImGui::SliderAngle("rotateX", &transform_.rotate.x);
	ImGui::SliderAngle("rotateY", &transform_.rotate.y);
	ImGui::SliderAngle("rotateZ", &transform_.rotate.z);
	ImGui::DragFloat3("translate", &transform_.translate.x, 0.05f, -20.0f, 20.0f);

	ImGui::End();

}



/*////////////////////////////////////////////////////////////////////////////////
*								    更新処理
////////////////////////////////////////////////////////////////////////////////*/
void Object3D::Update(Camera3D* camera) {

	// Matrixの計算
	matrix_.World =
		Matrix4x4::MakeAffineMatrix(transform_.scale, transform_.rotate, transform_.translate);
	matrix_.WVP =
		Matrix4x4::Multiply(matrix_.World, Matrix4x4::Multiply(camera->GetViewMatrix(), camera->GetProjectionMatrix()));
	matrix_.WorldInverseTranspose =
		Matrix4x4::Transpose(Matrix4x4::Inverse(matrix_.World));
	// GSMatrixの計算
	gsMat.gsMatrix_ =
		Matrix4x4::Multiply(matrix_.World, Matrix4x4::Multiply(camera->GetViewMatrix(), camera->GetProjectionMatrix()));

	// カメラワールド座標の取得
	cameraWorldPos_ = camera->GetWorldPos();

	cBuffer_->UpdateConstBuffer(
		name_, material_, phongRef_, matrix_, gsMat, light_, cameraWorldPos_, drawType_, isUseGltfModel_);
}


/*////////////////////////////////////////////////////////////////////////////////
*								    描画処理
////////////////////////////////////////////////////////////////////////////////*/
void Object3D::Draw() {

	switch (objectType_) {

		// 三角形
	case Object3DType::Triangle:

		Engine::DrawTriangle(name_, textureName_, drawType_, blendMode_);

		break;

		// 球
	case Object3DType::Sphere:

		Engine::DrawSphere(name_, textureName_, drawType_, blendMode_);

		break;

		// モデル
	case Object3DType::Model:

		Engine::DrawModel(name_, modelName_, textureName_, drawType_, blendMode_);

		break;

		// GS
	case Object3DType::GS:

		Engine::DrawGSModel(name_, textureName_, drawType_, blendMode_);

		break;
	}
}