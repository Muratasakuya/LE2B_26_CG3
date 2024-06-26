#include "Particle.h"

// Base
#include "Engine.h"

#include "Camera3D.h"
#include "ImGuiManager.h"
#include "ModelManager.h"
#include "TextureManager.h"
#include "DXCommon.h"



/*////////////////////////////////////////////////////////////////////////////////
*								コンストラクタ
////////////////////////////////////////////////////////////////////////////////*/
Particle::Particle() {

	vertexResource_ = std::make_unique<VertexResource>();
	cBuffer_ = std::make_unique<CBufferData>();

	// CBufferの作成
	cBuffer_->material = vertexResource_->CreateMaterial();
	cBuffer_->matrix = vertexResource_->CreateParticleWVP(instanceCount_);

	// srvDescの初期化
	InitializeDXSrvDesc();

	// モデル情報の作成

	//左上
	modelData_.vertices.push_back(
		{ .pos = {1.0f,1.0f,0.0f,1.0f},.texcoord = {0.0f,0.0f},.normal = {0.0f,0.0f,1.0f} });

	// 右上
	modelData_.vertices.push_back(
		{ .pos = {-1.0f,1.0f,0.0f,1.0f},.texcoord = {1.0f,0.0f},.normal = {0.0f,0.0f,1.0f} });

	// 左下
	modelData_.vertices.push_back(
		{ .pos = {1.0f,-1.0f,0.0f,1.0f},.texcoord = {0.0f,1.0f},.normal = {0.0f,0.0f,1.0f} });

	// 左下
	modelData_.vertices.push_back(
		{ .pos = {1.0f,-1.0f,0.0f,1.0f},.texcoord = {0.0f,1.0f},.normal = {0.0f,0.0f,1.0f} });

	// 右上
	modelData_.vertices.push_back(
		{ .pos = {-1.0f,1.0f,0.0f,1.0f},.texcoord = {1.0f,0.0f},.normal = {0.0f,0.0f,1.0f} });

	// 右下
	modelData_.vertices.push_back(
		{ .pos = {-1.0f,-1.0f,0.0f,1.0f},.texcoord = {1.0f,1.0f},.normal = {0.0f,0.0f,1.0f} });

	// モデルの名前
	modelName_ = "particle";
	// テクスチャの名前
	textureName_ = "uvChecker";

	// ブレンドモード
	blendMode_ = kBlendModeNormal;

	// 自作モデルデータの作成
	ModelManager::GetInstance()->MakeModel(modelData_, "particle");
	// テクスチャの読み込み
	TextureManager::GetInstance()->LoadTexture("./Resources/Images/uvChecker.png");
}

/*////////////////////////////////////////////////////////////////////////////////
*								 デストラクタ
////////////////////////////////////////////////////////////////////////////////*/
Particle::~Particle() {

	cBuffer_->material.reset();
	cBuffer_->matrix.reset();
	cBuffer_.reset();
}



/*////////////////////////////////////////////////////////////////////////////////
*								 SrvDescの初期化
////////////////////////////////////////////////////////////////////////////////*/
void Particle::InitializeDXSrvDesc() {

	instancingSrvDesc_.Format = DXGI_FORMAT_UNKNOWN;
	instancingSrvDesc_.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	instancingSrvDesc_.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
	instancingSrvDesc_.Buffer.FirstElement = 0;
	instancingSrvDesc_.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
	instancingSrvDesc_.Buffer.NumElements = instanceCount_;
	instancingSrvDesc_.Buffer.StructureByteStride = sizeof(TransformationMatrix);

	UINT descriptorSize = DXCommon::GetInstance()->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	instancingSrvHandleCPU_ =
		TextureManager::GetInstance()->GetCPUDescriptorHandle(
			DXCommon::GetInstance()->GetSRVDescriptorHeap(), descriptorSize, 2);
	instancingSrvHandleGPU_ =
		TextureManager::GetInstance()->GetGPUDescriptorHandle(
			DXCommon::GetInstance()->GetSRVDescriptorHeap(), descriptorSize, 2);

	DXCommon::GetInstance()->GetDevice()
		->CreateShaderResourceView(cBuffer_->matrix->resource.Get(), &instancingSrvDesc_, instancingSrvHandleCPU_);
}



/*////////////////////////////////////////////////////////////////////////////////
*									初期化
////////////////////////////////////////////////////////////////////////////////*/
void Particle::Initialize(Camera3D* camera) {

	// 色
	color_ = { 1.0f,1.0f,1.0f,1.0f };

	// ライトの向き
	lightDirection_ = { 0.0f,-1.0f,0.0f };

	// アフィン
	for (uint32_t index = 0; index < instanceCount_; ++index) {

		transforms_[index].scale = { 1.0f,1.0f,1.0f };
		transforms_[index].rotate = { 0.0f,0.0f,0.0f };
		transforms_[index].translate = { index * 0.1f,index * 0.1f,index * 0.1f };

		// Matrix
		Matrix4x4 worldMatrix =
			Matrix4x4::MakeAffineMatrix(transforms_[index].scale, transforms_[index].rotate, transforms_[index].translate);
		Matrix4x4 wvpMatrix =
			Matrix4x4::Multiply(worldMatrix, Matrix4x4::Multiply(camera->GetViewMatrix(), camera->GetProjectionMatrix()));

		cBuffer_->matrix->data[index].World = worldMatrix;
		cBuffer_->matrix->data[index].WVP = wvpMatrix;
	}

	// Material
	cBuffer_->material->data->color = color_;
	cBuffer_->material->data->enableLighting = false;
	cBuffer_->material->data->enableHalfLambert = false;
	cBuffer_->material->data->uvTransform = Matrix4x4::MakeIdentity4x4();
}



/*////////////////////////////////////////////////////////////////////////////////
*								    更新処理
////////////////////////////////////////////////////////////////////////////////*/
void Particle::Update(Camera3D* camera) {

	// アフィン
	for (uint32_t index = 0; index < instanceCount_; ++index) {

		transforms_[index].scale = { 1.0f,1.0f,1.0f };
		transforms_[index].rotate = { 0.0f,0.0f,0.0f };
		transforms_[index].translate = { index * 0.1f,index * 0.1f,index * 0.1f };

		// Matrix
		Matrix4x4 worldMatrix =
			Matrix4x4::MakeAffineMatrix(transforms_[index].scale, transforms_[index].rotate, transforms_[index].translate);
		Matrix4x4 wvpMatrix =
			Matrix4x4::Multiply(worldMatrix, Matrix4x4::Multiply(camera->GetViewMatrix(), camera->GetProjectionMatrix()));

		cBuffer_->matrix->data[index].World = worldMatrix;
		cBuffer_->matrix->data[index].WVP = wvpMatrix;
	}

	// Material
	cBuffer_->material->data->color = color_;
	cBuffer_->material->data->enableLighting = enableLighting_;
	cBuffer_->material->data->enableHalfLambert = enableHalfLambert_;
}



/*////////////////////////////////////////////////////////////////////////////////
*								    描画処理
////////////////////////////////////////////////////////////////////////////////*/
void Particle::Draw() {

	Engine::DrawParticle(
		cBuffer_.get(), static_cast<UINT>(modelData_.vertices.size()), instanceCount_, instancingSrvHandleGPU_,
		modelName_, textureName_, pParticle, blendMode_);
}