#include "Object3D.h"

// Base
#include "Engine.h"

#include "Camera3D.h"



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
Object3D::~Object3D() {}



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

	// Y軸回転
	transform_.rotate.y += 0.03f;

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
void Object3D::Draw(PipelineType pipelineType, const std::string& textureName, const std::optional<std::string>& modelName) {

	switch (objectType_) {

		// 三角形
	case Object3DType::Triangle:

		Engine::DrawTriangle(cBuffer_.get(), pipelineType, textureName);

		break;

		// 球
	case Object3DType::Sphere:

		Engine::DrawSphere(cBuffer_.get(), pipelineType, textureName);

		break;

		// モデル
	case Object3DType::Model:

		Engine::DrawModel(cBuffer_.get(), pipelineType, *modelName, textureName);

		break;
	}
}