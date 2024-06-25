#include "Object2D.h"

// Base
#include "Engine.h"

#include "Camera2D.h"



/*////////////////////////////////////////////////////////////////////////////////
*								コンストラクタ
////////////////////////////////////////////////////////////////////////////////*/
Object2D::Object2D() {

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
Object2D::~Object2D() {}



/*////////////////////////////////////////////////////////////////////////////////
*									初期化
////////////////////////////////////////////////////////////////////////////////*/
void Object2D::Initilize(Camera2D* camera) {

	// 色
	color_ = { 1.0f,1.0f,1.0f,1.0f };

	// アフィン
	transform_.scale = { 1.0f,1.0f,1.0f };
	transform_.rotate = { 0.0f,0.0f,0.0f };
	transform_.translate = { 0.0f,0.0f,0.0f };

	// Matrix
	Matrix4x4 worldMatrix =
		Matrix4x4::MakeAffineMatrix(transform_.scale, transform_.rotate, transform_.translate);
	matrix_.WVP =
		Matrix4x4::Multiply(worldMatrix, Matrix4x4::Multiply(camera->GetViewMatrix(), camera->GetOrthoMatrix()));

	// Material
	cBuffer_->material->data->color = color_;
	cBuffer_->material->data->enableLighting = false;
	cBuffer_->material->data->uvTransform = Matrix4x4::MakeIdentity4x4();

	// Matrix
	cBuffer_->matrix->data->WVP = matrix_.WVP;
}



/*////////////////////////////////////////////////////////////////////////////////
*								    更新処理
////////////////////////////////////////////////////////////////////////////////*/
void Object2D::Update(Camera2D* camera) {

	// Matrix
	matrix_.World =
		Matrix4x4::MakeAffineMatrix(transform_.scale, transform_.rotate, transform_.translate);
	matrix_.WVP =
		Matrix4x4::Multiply(matrix_.World, Matrix4x4::Multiply(camera->GetViewMatrix(), camera->GetOrthoMatrix()));

	// Material
	cBuffer_->material->data->color = color_;

	// Matrix
	cBuffer_->matrix->data->WVP = matrix_.World;
	cBuffer_->matrix->data->WVP = matrix_.WVP;
}



/*////////////////////////////////////////////////////////////////////////////////
*								    描画処理
////////////////////////////////////////////////////////////////////////////////*/
void Object2D::Draw(const std::string textureName) {

	Engine::DrawSprite(cBuffer_.get(), Texture, textureName);
}