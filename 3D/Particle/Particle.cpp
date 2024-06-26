#include "Particle.h"

// Base
#include "Engine.h"

#include "Camera3D.h"
#include "ImGuiManager.h"
#include "ModelManager.h"
#include "TextureManager.h"
#include "DXCommon.h"



// パーティクル生成関数
ParticleData Particle::MakeNewParticle(std::mt19937& randomEngine) {

	std::uniform_real_distribution<float> distribution(-1.0f, 1.0f);
	std::uniform_real_distribution<float> distColor(-1.0f, 1.0f);
	std::uniform_real_distribution<float> distTime(1.0f, 3.0f);

	ParticleData particleData;
	particleData.transform.scale = { 1.0f,1.0f,1.0f };
	particleData.transform.rotate = { 0.0f,0.0f,0.0f };
	particleData.transform.translate = { distribution(randomEngine),distribution(randomEngine) ,distribution(randomEngine) };
	particleData.velocity = { distribution(randomEngine),distribution(randomEngine) ,distribution(randomEngine) };
	particleData.color = { distColor(randomEngine),distColor(randomEngine) ,distColor(randomEngine) ,1.0f };
	particleData.lifeTime = distTime(randomEngine);
	particleData.currentTime = 0.0f;

	return particleData;
}



/*////////////////////////////////////////////////////////////////////////////////
*								コンストラクタ
////////////////////////////////////////////////////////////////////////////////*/
Particle::Particle(Camera3D* camera) {

	vertexResource_ = std::make_unique<VertexResource>();
	cBuffer_ = std::make_unique<CBufferData>();

	// CBufferの作成
	cBuffer_->material = vertexResource_->CreateMaterial();
	cBuffer_->particleMatrix = vertexResource_->CreateParticleWVP(instanceMaxCount_);

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
	textureName_ = "circle";

	// ブレンドモード
	blendMode_ = kBlendModeAdd;

	// 自作モデルデータの作成
	ModelManager::GetInstance()->MakeModel(modelData_, "particle");
	// テクスチャの読み込み
	TextureManager::GetInstance()->LoadTexture("./Resources/Images/circle.png");

	Initialize(camera);
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
	instancingSrvDesc_.Buffer.NumElements = instanceMaxCount_;
	instancingSrvDesc_.Buffer.StructureByteStride = sizeof(ParticleForGPU);

	UINT descriptorSize = DXCommon::GetInstance()->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	instancingSrvHandleCPU_ =
		TextureManager::GetInstance()->GetCPUDescriptorHandle(
			DXCommon::GetInstance()->GetSRVDescriptorHeap(), descriptorSize, 2);
	instancingSrvHandleGPU_ =
		TextureManager::GetInstance()->GetGPUDescriptorHandle(
			DXCommon::GetInstance()->GetSRVDescriptorHeap(), descriptorSize, 2);

	DXCommon::GetInstance()->GetDevice()
		->CreateShaderResourceView(cBuffer_->particleMatrix->resource.Get(), &instancingSrvDesc_, instancingSrvHandleCPU_);
}



/*////////////////////////////////////////////////////////////////////////////////
*									初期化
////////////////////////////////////////////////////////////////////////////////*/
void Particle::Initialize(Camera3D* camera) {

	// 乱数の生成
	std::random_device seedGenerator;
	std::mt19937 randomEngine(seedGenerator());

	// SRT
	transform_.scale = { 1.0f,1.0f,1.0f };
	transform_.rotate = { 0.0f,0.0f,0.0f };
	transform_.translate = { 0.0f,0.0f,0.0f };

	// 色
	color_ = { 1.0f,1.0f,1.0f,1.0f };

	// ライトの向き
	lightDirection_ = { 0.0f,-1.0f,0.0f };

	//useBillboradの有無
	useBillboard_ = true;

	// billboard変換
	Matrix4x4 backToFrontMatrix = Matrix4x4::MakeYawMatrix(std::numbers::pi_v<float>);
	Matrix4x4 billboardMatrix = Matrix4x4::Multiply(backToFrontMatrix, camera->GetCameraMatrix());
	// 平行移動成分はなし
	billboardMatrix.m[3][0] = 0.0f;
	billboardMatrix.m[3][1] = 0.0f;
	billboardMatrix.m[3][2] = 0.0f;

	// アフィン
	for (uint32_t index = 0; index < instanceMaxCount_; ++index) {

		particles_[index] = MakeNewParticle(randomEngine);

		particles_[index].transform.scale = transform_.scale;
		particles_[index].transform.rotate = transform_.rotate;

		Matrix4x4 scaleMatrix = Matrix4x4::MakeScaleMatrix(particles_[index].transform.scale);
		Matrix4x4 translateMatrix = Matrix4x4::MakeTranslateMatrix(particles_[index].transform.translate);

		// Matrix
		if (useBillboard_) {

			worldMatrix_ = Matrix4x4::Multiply(scaleMatrix, Matrix4x4::Multiply(billboardMatrix, translateMatrix));
		} else {

			worldMatrix_ = Matrix4x4::Multiply(scaleMatrix, Matrix4x4::Multiply(Matrix4x4::MakeIdentity4x4(), translateMatrix));
		}

		wvpMatrix_ =
			Matrix4x4::Multiply(worldMatrix_, Matrix4x4::Multiply(camera->GetViewMatrix(), camera->GetProjectionMatrix()));

		cBuffer_->particleMatrix->particleData[index].World = worldMatrix_;
		cBuffer_->particleMatrix->particleData[index].WVP = wvpMatrix_;
		cBuffer_->particleMatrix->particleData[index].color = particles_[index].color;
	}

	// Material
	cBuffer_->material->data->enableLighting = false;
	cBuffer_->material->data->enableHalfLambert = false;
	cBuffer_->material->data->uvTransform = Matrix4x4::MakeIdentity4x4();
}



/*////////////////////////////////////////////////////////////////////////////////
*								    更新処理
////////////////////////////////////////////////////////////////////////////////*/
void Particle::Update(Camera3D* camera) {

	ImGui::Begin("particle");

	ImGui::ColorEdit4("color", &color_.x);
	ImGui::SliderFloat3("scale", &transform_.scale.x, 0.0f, 1.0f);
	ImGui::SliderAngle("rotateX", &transform_.rotate.x);
	ImGui::SliderAngle("rotateY", &transform_.rotate.y);
	ImGui::SliderAngle("rotateZ", &transform_.rotate.z);
	ImGui::Checkbox("useBillboard", &useBillboard_);

	ImGui::End();

	// 乱数の生成
	std::random_device seedGenerator;
	std::mt19937 randomEngine(seedGenerator());

	// 描画すべきインスタンス数
	uint32_t numInstance = 0;

	// billboard変換
	Matrix4x4 backToFrontMatrix = Matrix4x4::MakeYawMatrix(std::numbers::pi_v<float>);
	Matrix4x4 billboardMatrix = Matrix4x4::Multiply(backToFrontMatrix, camera->GetCameraMatrix());
	// 平行移動成分はなし
	billboardMatrix.m[3][0] = 0.0f;
	billboardMatrix.m[3][1] = 0.0f;
	billboardMatrix.m[3][2] = 0.0f;

	// アフィン
	for (uint32_t index = 0; index < instanceMaxCount_; ++index) {

		// パーティクルが消えている場合
		if (particles_[index].lifeTime <= particles_[index].currentTime) {

			// リスポーンタイマーを進める
			particles_[index].respawnTime += kDeltaTime;

			// リスポーンタイマーが遅延時間を超えた場合
			if (particles_[index].respawnTime >= particles_[index].respawnDelay) {

				// パーティクルを再生成する
				particles_[index].currentTime = 0.0f;
				particles_[index].respawnTime = 0.0f;
				particles_[index] = MakeNewParticle(randomEngine);
			}

			continue;
		}

		particles_[index].transform.translate +=
		{particles_[index].velocity.x* kDeltaTime, particles_[index].velocity.y* kDeltaTime, particles_[index].velocity.z* kDeltaTime};

		// 経過時間を足す
		particles_[index].currentTime += kDeltaTime;

		particles_[index].transform.scale = transform_.scale;
		particles_[index].transform.rotate = transform_.rotate;

		Matrix4x4 scaleMatrix = Matrix4x4::MakeScaleMatrix(particles_[index].transform.scale);
		Matrix4x4 translateMatrix = Matrix4x4::MakeTranslateMatrix(particles_[index].transform.translate);

		// Matrix
		if (useBillboard_) {

			worldMatrix_ = Matrix4x4::Multiply(scaleMatrix, Matrix4x4::Multiply(billboardMatrix, translateMatrix));
		} else {

			worldMatrix_ = Matrix4x4::Multiply(scaleMatrix, Matrix4x4::Multiply(Matrix4x4::MakeIdentity4x4(), translateMatrix));
		}

		wvpMatrix_ =
			Matrix4x4::Multiply(worldMatrix_, Matrix4x4::Multiply(camera->GetViewMatrix(), camera->GetProjectionMatrix()));

		float alpha = 1.0f - (particles_[index].currentTime / particles_[index].lifeTime);

		cBuffer_->particleMatrix->particleData[numInstance].World = worldMatrix_;
		cBuffer_->particleMatrix->particleData[numInstance].WVP = wvpMatrix_;
		cBuffer_->particleMatrix->particleData[numInstance].color = particles_[index].color;
		cBuffer_->particleMatrix->particleData[numInstance].color.w = alpha;

		++numInstance;
	}

	// インスタンス数の更新
	numInstance_ = numInstance;

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
		cBuffer_.get(), static_cast<UINT>(modelData_.vertices.size()), numInstance_, instancingSrvHandleGPU_,
		modelName_, textureName_, pParticle, blendMode_);
}