#include "Object3D.h"

// Base
#include "Engine.h"

#include "Camera3D.h"
#include "ImGuiManager.h"
#include "ModelManager.h"
#include "TextureManager.h"



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
Object3D::Object3D() {

	vertexResource_ = std::make_unique<VertexResource>();
	cBuffer_ = std::make_unique<CBufferData>();

	// CBufferの作成
	cBuffer_->material = vertexResource_->CreateMaterial();
	cBuffer_->phongRefMaterial = vertexResource_->CreatePhongRefMaterial();
	cBuffer_->matrix = vertexResource_->CreateWVP();
	cBuffer_->light = vertexResource_->CreateLight();
	cBuffer_->pointLight = vertexResource_->CreatePointLight();
	cBuffer_->spotLight = vertexResource_->CreateSpotLight();
	cBuffer_->camera = vertexResource_->CreateCamera();
}

/*////////////////////////////////////////////////////////////////////////////////
*								 デストラクタ
////////////////////////////////////////////////////////////////////////////////*/
Object3D::~Object3D() {

	cBuffer_->material.reset();
	cBuffer_->phongRefMaterial.reset();
	cBuffer_->matrix.reset();
	cBuffer_->light.reset();
	cBuffer_->pointLight.reset();
	cBuffer_->spotLight.reset();
	cBuffer_->camera.reset();
	cBuffer_.reset();
}



/*////////////////////////////////////////////////////////////////////////////////
*									初期化
////////////////////////////////////////////////////////////////////////////////*/
void Object3D::Initialize(
	Camera3D* camera, const Object3DType& objectType, const PipelineType& drawType, const BlendMode& blendMode,
	const std::string& textureName, const std::optional<std::string>& modelName) {

#pragma region /// cBufferDefaultInitialize ///

	// 色
	color_ = { 1.0f,1.0f,1.0f,1.0f };
	specularColor_ = { 1.0f,1.0f,1.0f };
	shininess_ = 64.0f;

	// ライトの向き
	lightDirection_ = { 0.0f,-1.0f,0.0f };

	// ポイントライト
	pointLight_.color = { 1.0f,1.0f,1.0f,1.0f };
	pointLight_.pos = { 0.0f,2.0f,0.0f };
	pointLight_.intensity = 1.0f;
	pointLight_.radius = 5.0f;
	pointLight_.decay = 1.0f;

	// スポットライト
	spotLight_.color = { 1.0f,1.0f,1.0f,1.0f };
	spotLight_.pos = { 2.0f,1.25f,0.0f };
	spotLight_.distance = 5.0f;
	spotLight_.direction = Vector3::Normalize({ -1.0f,-1.0f,0.0 });
	spotLight_.intensity = 1.0f;
	spotLight_.decay = 1.0f;
	spotLight_.cosAngle = std::cos(std::numbers::pi_v<float> / 3.0f);
	spotLight_.cosFalloffStart = 1.0f;

	// アフィン
	transform_.scale = { 1.0f,1.0f,1.0f };
	transform_.rotate = { 0.0f,0.0f,0.0f };
	transform_.translate = { 0.0f,0.0f,0.0f };

	enableLighting_ = false;
	enableHalfLambert_ = false;
	enablePhongReflection_ = false;
	enableBlinnPhongReflection_ = false;

	// デフォルトはfalse
	isUseGltfModel_ = false;
	gltfModelName_ = "";

	// Matrix
	matrix_.World =
		Matrix4x4::MakeAffineMatrix(transform_.scale, transform_.rotate, transform_.translate);
	matrix_.WVP =
		Matrix4x4::Multiply(matrix_.World, Matrix4x4::Multiply(camera->GetViewMatrix(), camera->GetProjectionMatrix()));
	matrix_.WorldInverseTranspose =
		Matrix4x4::Transpose(Matrix4x4::Inverse(matrix_.World));

	// Material
	cBuffer_->material->data->color = color_;
	cBuffer_->material->data->enableLighting = enableLighting_;
	cBuffer_->material->data->enableHalfLambert = enableHalfLambert_;
	cBuffer_->material->data->uvTransform = Matrix4x4::MakeIdentity4x4();

	// PhongRefMaterial
	cBuffer_->phongRefMaterial->data->color = color_;
	cBuffer_->phongRefMaterial->data->enableLighting = enableLighting_;
	cBuffer_->phongRefMaterial->data->enablePhongReflection = enablePhongReflection_;
	cBuffer_->phongRefMaterial->data->enableBlinnPhongReflection = enableBlinnPhongReflection_;
	cBuffer_->phongRefMaterial->data->uvTransform = Matrix4x4::MakeIdentity4x4();;
	cBuffer_->phongRefMaterial->data->specularColor = specularColor_;
	cBuffer_->phongRefMaterial->data->shininess = shininess_;

	// DirectionalLight
	cBuffer_->light->data->color = { 1.0f,1.0f,1.0f,1.0f };
	cBuffer_->light->data->direction = lightDirection_;
	cBuffer_->light->data->intensity = 0.0f;

	// PointLight
	cBuffer_->pointLight->data->color = pointLight_.color;
	cBuffer_->pointLight->data->pos = pointLight_.pos;
	cBuffer_->pointLight->data->intensity = pointLight_.intensity;
	cBuffer_->pointLight->data->radius = pointLight_.radius;
	cBuffer_->pointLight->data->decay = pointLight_.decay;

	// SpotLight
	cBuffer_->spotLight->data->color = spotLight_.color;
	cBuffer_->spotLight->data->pos = spotLight_.pos;
	cBuffer_->spotLight->data->direction = spotLight_.direction;
	cBuffer_->spotLight->data->distance = spotLight_.distance;
	cBuffer_->spotLight->data->intensity = spotLight_.intensity;
	cBuffer_->spotLight->data->cosAngle = spotLight_.cosAngle;
	cBuffer_->spotLight->data->cosFalloffStart = spotLight_.cosFalloffStart;
	cBuffer_->spotLight->data->decay = spotLight_.decay;

	// Matrix
	if (isUseGltfModel_) {

		cBuffer_->matrix->data->World = Matrix4x4::Multiply
		(ModelManager::GetInstance()->GetModelData(gltfModelName_).rootNode.localMatrix, matrix_.World);
		cBuffer_->matrix->data->WVP = Matrix4x4::Multiply
		(ModelManager::GetInstance()->GetModelData(gltfModelName_).rootNode.localMatrix, matrix_.WVP);
		cBuffer_->matrix->data->WorldInverseTranspose = matrix_.WorldInverseTranspose;
	} else {

		cBuffer_->matrix->data->World = matrix_.World;
		cBuffer_->matrix->data->WVP = matrix_.WVP;
		cBuffer_->matrix->data->WorldInverseTranspose = matrix_.WorldInverseTranspose;
	}

	// Camera
	cBuffer_->camera->data->worldPosition = camera->GetWorldPos();

#pragma endregion

	// 3Dオブジェクトタイプのセット
	objectType_ = objectType;

	// 描画タイプのセット
	drawType_ = drawType;

	// ブレンドの設定
	blendMode_ = blendMode;

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
*								ImGui更新処理
////////////////////////////////////////////////////////////////////////////////*/
void Object3D::UpdateImGui(const std::string& objectName) {

	ImGui::Text(objectName.c_str());
	if (ImGui::TreeNode(objectName.c_str(),"Transform")) {

		ImGui::SliderFloat3("scale", &transform_.scale.x, 0.0f, 3.0f);
		ImGui::SliderAngle("rotateX", &transform_.rotate.x);
		ImGui::SliderAngle("rotateY", &transform_.rotate.y);
		ImGui::SliderAngle("rotateZ", &transform_.rotate.z);
		ImGui::DragFloat3("translate", &transform_.translate.x, 0.05f, -20.0f, 20.0f);

		ImGui::TreePop();
	}
}



/*////////////////////////////////////////////////////////////////////////////////
*								    更新処理
////////////////////////////////////////////////////////////////////////////////*/
void Object3D::Update(Camera3D* camera) {

#pragma region /// cBufferUpdate ///

	if (enableBlinnPhongReflection_) {

		enablePhongReflection_ = false;
	} else if (!enableBlinnPhongReflection_) {

		enablePhongReflection_ = true;
	}

	// Matrix
	matrix_.World =
		Matrix4x4::MakeAffineMatrix(transform_.scale, transform_.rotate, transform_.translate);
	matrix_.WVP =
		Matrix4x4::Multiply(matrix_.World, Matrix4x4::Multiply(camera->GetViewMatrix(), camera->GetProjectionMatrix()));
	matrix_.WorldInverseTranspose =
		Matrix4x4::Transpose(Matrix4x4::Inverse(matrix_.World));

	// Material
	cBuffer_->material->data->color = color_;
	cBuffer_->material->data->enableLighting = enableLighting_;
	cBuffer_->material->data->enableHalfLambert = enableHalfLambert_;

	// PhongRefMaterial
	cBuffer_->phongRefMaterial->data->color = color_;
	cBuffer_->phongRefMaterial->data->enableLighting = enableLighting_;
	cBuffer_->phongRefMaterial->data->enablePhongReflection = enablePhongReflection_;
	cBuffer_->phongRefMaterial->data->enableBlinnPhongReflection = enableBlinnPhongReflection_;
	cBuffer_->phongRefMaterial->data->specularColor = specularColor_;
	cBuffer_->phongRefMaterial->data->shininess = shininess_;

	// Light
	cBuffer_->light->data->direction = lightDirection_;

	// PointLight
	cBuffer_->pointLight->data->color = pointLight_.color;
	cBuffer_->pointLight->data->pos = pointLight_.pos;
	cBuffer_->pointLight->data->intensity = pointLight_.intensity;
	cBuffer_->pointLight->data->radius = pointLight_.radius;
	cBuffer_->pointLight->data->decay = pointLight_.decay;

	// SpotLight
	cBuffer_->spotLight->data->color = spotLight_.color;
	cBuffer_->spotLight->data->pos = spotLight_.pos;
	cBuffer_->spotLight->data->direction = spotLight_.direction;
	cBuffer_->spotLight->data->distance = spotLight_.distance;
	cBuffer_->spotLight->data->intensity = spotLight_.intensity;
	cBuffer_->spotLight->data->cosAngle = spotLight_.cosAngle;
	cBuffer_->spotLight->data->cosFalloffStart = spotLight_.cosFalloffStart;
	cBuffer_->spotLight->data->decay = spotLight_.decay;

	// Matrix
	if (isUseGltfModel_) {

		cBuffer_->matrix->data->World = Matrix4x4::Multiply
		(ModelManager::GetInstance()->GetModelData(gltfModelName_).rootNode.localMatrix, matrix_.World);
		cBuffer_->matrix->data->WVP = Matrix4x4::Multiply
		(ModelManager::GetInstance()->GetModelData(gltfModelName_).rootNode.localMatrix, matrix_.WVP);
		cBuffer_->matrix->data->WorldInverseTranspose = matrix_.WorldInverseTranspose;
	} else {

		cBuffer_->matrix->data->World = matrix_.World;
		cBuffer_->matrix->data->WVP = matrix_.WVP;
		cBuffer_->matrix->data->WorldInverseTranspose = matrix_.WorldInverseTranspose;
	}

	// Camera
	cBuffer_->camera->data->worldPosition = camera->GetWorldPos();

#pragma endregion

}



/*////////////////////////////////////////////////////////////////////////////////
*								    描画処理
////////////////////////////////////////////////////////////////////////////////*/
void Object3D::Draw() {

	switch (objectType_) {

		// 三角形
	case Object3DType::Triangle:

		Engine::DrawTriangle(cBuffer_.get(), textureName_, drawType_, blendMode_);

		break;

		// 球
	case Object3DType::Sphere:

		Engine::DrawSphere(cBuffer_.get(), textureName_, drawType_, blendMode_);

		break;

		// モデル
	case Object3DType::Model:

		Engine::DrawModel(cBuffer_.get(), modelName_, textureName_, drawType_, blendMode_);

		break;
	}
}