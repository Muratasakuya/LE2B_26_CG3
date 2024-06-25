#include "Object3D.h"

// Base
#include "Engine.h"

#include "Camera3D.h"
#include "ImGuiManager.h"



/*////////////////////////////////////////////////////////////////////////////////
*								コンストラクタ
////////////////////////////////////////////////////////////////////////////////*/
Object3D::Object3D(Object3DType objectType) {

	objectType_ = objectType;

	vertexResource_ = std::make_unique<VertexResource>();
	cBuffer_ = std::make_unique<CBufferData>();

	// CBufferの作成
	cBuffer_->material = vertexResource_->CreateMaterial();
	cBuffer_->matrix = vertexResource_->CreateWVP();
	cBuffer_->light = vertexResource_->CreateLight();
}

/*////////////////////////////////////////////////////////////////////////////////
*								 デストラクタ
////////////////////////////////////////////////////////////////////////////////*/
Object3D::~Object3D() {

	cBuffer_->material.reset();
	cBuffer_->matrix.reset();
	cBuffer_->light.reset();
	cBuffer_.reset();
}



/*////////////////////////////////////////////////////////////////////////////////
*									初期化
////////////////////////////////////////////////////////////////////////////////*/
void Object3D::Initilize(Camera3D* camera) {

	// 色
	color_ = { 1.0f,1.0f,1.0f,1.0f };

	// ライトの向き
	lightDirection_ = { 0.0f,-1.0f,0.0f };

	// アフィン
	transform_.scale = { 1.0f,1.0f,1.0f };
	transform_.rotate = { 0.0f,0.0f,0.0f };
	transform_.translate = { 0.0f,0.0f,0.0f };

	// Matrix
	matrix_.World =
		Matrix4x4::MakeAffineMatrix(transform_.scale, transform_.rotate, transform_.translate);
	matrix_.WVP =
		Matrix4x4::Multiply(matrix_.World, Matrix4x4::Multiply(camera->GetViewMatrix(), camera->GetProjectionMatrix()));

	// Material
	cBuffer_->material->data->color = color_;
	cBuffer_->material->data->enableLighting = enableLighting_;
	cBuffer_->material->data->enableHalfLambert = enableHalfLambert_;
	cBuffer_->material->data->uvTransform = Matrix4x4::MakeIdentity4x4();

	// Light
	cBuffer_->light->data->color = { 1.0f,1.0f,1.0f,1.0f };
	cBuffer_->light->data->direction = lightDirection_;
	cBuffer_->light->data->intensity = 1.0f;

	// Matrix
	cBuffer_->matrix->data->World = matrix_.World;
	cBuffer_->matrix->data->WVP = matrix_.WVP;
}



/*////////////////////////////////////////////////////////////////////////////////
*								    更新処理
////////////////////////////////////////////////////////////////////////////////*/
void Object3D::Update(Camera3D* camera) {

	/*----------------------------------------------------------------------------------------------------------------*/
	/// ImGui

	ImGui::Begin("fence");

	ImGui::ColorEdit4("color", &color_.x);
	ImGui::SliderFloat3("scale", &transform_.scale.x, 0.0f, 1.0f);
	ImGui::SliderAngle("rotateX", &transform_.rotate.x);
	ImGui::SliderAngle("rotateY", &transform_.rotate.y);
	ImGui::SliderAngle("rotateZ", &transform_.rotate.z);
	ImGui::SliderFloat3("translate", &transform_.translate.x, -5.0f, 5.0f);
	ImGui::SliderFloat3("LightDirection", &lightDirection_.x, -1.0f, 1.0f);

	ImGui::End();

	/*----------------------------------------------------------------------------------------------------------------*/

	// Matrix
	matrix_.World =
		Matrix4x4::MakeAffineMatrix(transform_.scale, transform_.rotate, transform_.translate);
	matrix_.WVP =
		Matrix4x4::Multiply(matrix_.World, Matrix4x4::Multiply(camera->GetViewMatrix(), camera->GetProjectionMatrix()));

	// Material
	cBuffer_->material->data->color = color_;
	cBuffer_->material->data->enableLighting = enableLighting_;
	cBuffer_->material->data->enableHalfLambert = enableHalfLambert_;

	// Light
	cBuffer_->light->data->direction = lightDirection_;

	// Matrix
	cBuffer_->matrix->data->World = matrix_.World;
	cBuffer_->matrix->data->WVP = matrix_.WVP;
}



/*////////////////////////////////////////////////////////////////////////////////
*								    描画処理
////////////////////////////////////////////////////////////////////////////////*/
void Object3D::Draw(PipelineType pipelineType, BlendMode blendMode, const std::string& textureName, const std::optional<std::string>& modelName) {

	switch (objectType_) {

		// 三角形
	case Object3DType::Triangle:

		Engine::DrawTriangle(cBuffer_.get(), textureName, pipelineType, blendMode);

		break;

		// 球
	case Object3DType::Sphere:

		Engine::DrawSphere(cBuffer_.get(), textureName, pipelineType, blendMode);

		break;

		// モデル
	case Object3DType::Model:

		Engine::DrawModel(cBuffer_.get(), *modelName, textureName, pipelineType, blendMode);

		break;
	}
}