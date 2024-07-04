//#include "Particle.h"
//
//// Base
//#include "Engine.h"
//
//#include "Camera3D.h"
//#include "ImGuiManager.h"
//#include "ModelManager.h"
//#include "TextureManager.h"
//#include "DXCommon.h"
//
//
//
//// パーティクル生成関数
//ParticleData Particle::MakeNewParticle(std::mt19937& randomEngine, const Vector3& translate) {
//
//	std::uniform_real_distribution<float> distribution(-1.0f, 1.0f);
//	std::uniform_real_distribution<float> distColor(-1.0f, 1.0f);
//	std::uniform_real_distribution<float> distTime(1.0f, 3.0f);
//
//	ParticleData particleData;
//	particleData.transform.scale = { 1.0f,1.0f,1.0f };
//	particleData.transform.rotate = { 0.0f,0.0f,0.0f };
//	particleData.velocity = { distribution(randomEngine),distribution(randomEngine) ,distribution(randomEngine) };
//	particleData.color = { distColor(randomEngine),distColor(randomEngine) ,distColor(randomEngine) ,1.0f };
//	particleData.lifeTime = distTime(randomEngine);
//	particleData.currentTime = 0.0f;
//
//	Vector3 randomTranslate = { distribution(randomEngine),distribution(randomEngine) ,distribution(randomEngine) };
//	particleData.transform.translate = translate + randomTranslate;
//
//	return particleData;
//}
//
//// Emitterでパーティクルを生成する関数
//std::list<ParticleData> Particle::Emit(const Emitter& emitter, std::mt19937& randomEngine) {
//
//	std::list<ParticleData> particles;
//
//	for (uint32_t count = 0; count < emitter.count; ++count) {
//
//		particles.push_back(MakeNewParticle(randomEngine, emitter.transform.translate));
//	}
//
//
//	return particles;
//}
//
//// 衝突判定
//bool Particle::IsCollision(const AABB& aabb, const Vector3& point) {
//
//	// ポイントがAABBの範囲内にあるかをチェック
//	if ((point.x >= aabb.min.x && point.x <= aabb.max.x) &&
//		(point.y >= aabb.min.y && point.y <= aabb.max.y) &&
//		(point.z >= aabb.min.z && point.z <= aabb.max.z)) {
//
//		return true;
//	}
//	return false;
//}
//
//
//
///*////////////////////////////////////////////////////////////////////////////////
//*								コンストラクタ
//////////////////////////////////////////////////////////////////////////////////*/
//Particle::Particle(Camera3D* camera) {
//
//	cBuffer_ = std::make_unique<CBufferData>();
//
//	// CBufferの作成
//	cBuffer_->material = vertexResource_.CreateMaterial();
//	cBuffer_->particleMatrix = vertexResource_.CreateParticleWVP(instanceMaxCount_);
//
//	// srvDescの初期化
//	InitializeDXSrvDesc();
//
//	// モデル情報の作成
//
//	//左上
//	modelData_.vertices.push_back(
//		{ .pos = {1.0f,1.0f,0.0f,1.0f},.texcoord = {0.0f,0.0f},.normal = {0.0f,0.0f,1.0f} });
//
//	// 右上
//	modelData_.vertices.push_back(
//		{ .pos = {-1.0f,1.0f,0.0f,1.0f},.texcoord = {1.0f,0.0f},.normal = {0.0f,0.0f,1.0f} });
//
//	// 左下
//	modelData_.vertices.push_back(
//		{ .pos = {1.0f,-1.0f,0.0f,1.0f},.texcoord = {0.0f,1.0f},.normal = {0.0f,0.0f,1.0f} });
//
//	// 左下
//	modelData_.vertices.push_back(
//		{ .pos = {1.0f,-1.0f,0.0f,1.0f},.texcoord = {0.0f,1.0f},.normal = {0.0f,0.0f,1.0f} });
//
//	// 右上
//	modelData_.vertices.push_back(
//		{ .pos = {-1.0f,1.0f,0.0f,1.0f},.texcoord = {1.0f,0.0f},.normal = {0.0f,0.0f,1.0f} });
//
//	// 右下
//	modelData_.vertices.push_back(
//		{ .pos = {-1.0f,-1.0f,0.0f,1.0f},.texcoord = {1.0f,1.0f},.normal = {0.0f,0.0f,1.0f} });
//
//	// モデルの名前
//	modelName_ = "particle";
//	// テクスチャの名前
//	textureName_ = "circle";
//
//	// ブレンドモード
//	blendMode_ = kBlendModeAdd;
//
//	// 自作モデルデータの作成
//	ModelManager::GetInstance()->MakeModel(modelData_, "particle");
//	// テクスチャの読み込み
//	TextureManager::GetInstance()->LoadTexture("./Resources/Images/circle.png");
//
//	Initialize(camera);
//}
//
///*////////////////////////////////////////////////////////////////////////////////
//*								 デストラクタ
//////////////////////////////////////////////////////////////////////////////////*/
//Particle::~Particle() {
//
//	cBuffer_->material.reset();
//	cBuffer_->matrix.reset();
//	cBuffer_.reset();
//}
//
//
//
///*////////////////////////////////////////////////////////////////////////////////
//*								 SrvDescの初期化
//////////////////////////////////////////////////////////////////////////////////*/
//void Particle::InitializeDXSrvDesc() {
//
//	instancingSrvDesc_.Format = DXGI_FORMAT_UNKNOWN;
//	instancingSrvDesc_.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
//	instancingSrvDesc_.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
//	instancingSrvDesc_.Buffer.FirstElement = 0;
//	instancingSrvDesc_.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
//	instancingSrvDesc_.Buffer.NumElements = instanceMaxCount_;
//	instancingSrvDesc_.Buffer.StructureByteStride = sizeof(ParticleForGPU);
//
//	UINT descriptorSize = DXCommon::GetInstance()->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
//
//	instancingSrvHandleCPU_ =
//		TextureManager::GetInstance()->GetCPUDescriptorHandle(
//			DXCommon::GetInstance()->GetSRVDescriptorHeap(), descriptorSize, 2);
//	instancingSrvHandleGPU_ =
//		TextureManager::GetInstance()->GetGPUDescriptorHandle(
//			DXCommon::GetInstance()->GetSRVDescriptorHeap(), descriptorSize, 2);
//
//	DXCommon::GetInstance()->GetDevice()
//		->CreateShaderResourceView(cBuffer_->particleMatrix->resource.Get(), &instancingSrvDesc_, instancingSrvHandleCPU_);
//}
//
//
//
///*////////////////////////////////////////////////////////////////////////////////
//*									初期化
//////////////////////////////////////////////////////////////////////////////////*/
//void Particle::Initialize(Camera3D* camera) {
//
//	// 乱数の生成
//	std::random_device seedGenerator;
//	std::mt19937 randomEngine(seedGenerator());
//
//	// エミッタ情報の初期化
//	emitter_.count = 3;
//	// 0.5秒置きに発生
//	emitter_.frequency = 0.5f;
//	// 発生頻度用の時刻
//	emitter_.frequencyTime = 0.0f;
//
//	//エミッタのSRT
//	emitter_.transform.scale = { 1.0f,1.0f,1.0f };
//	emitter_.transform.rotate = { 0.0f,0.0f,0.0f };
//	emitter_.transform.translate = { 0.0f,0.0f,0.0f };
//
//	// 加速フィールドの初期化
//	accelerationField_.acceleration = { 15.0f,0.0f,0.0f };
//	accelerationField_.area.min = { -1.0f,-1.0f,-1.0f };
//	accelerationField_.area.max = { 1.0f,1.0f,1.0f };
//
//	// SRT
//	transform_.scale = { 1.0f,1.0f,1.0f };
//	transform_.rotate = { 0.0f,0.0f,0.0f };
//	transform_.translate = { 0.0f,0.0f,0.0f };
//
//	// 色
//	material_.color = { 1.0f,1.0f,1.0f,1.0f };
//
//	// ライトの向き
//	directionlLight_.color = { 0.0f,-1.0f,0.0f };
//
//	//useBillboradの有無
//	useBillboard_ = true;
//
//	particles_.splice(particles_.end(), Emit(emitter_, randomEngine));
//
//	// billboard変換
//	Matrix4x4 backToFrontMatrix = Matrix4x4::MakeYawMatrix(std::numbers::pi_v<float>);
//	Matrix4x4 billboardMatrix = Matrix4x4::Multiply(backToFrontMatrix, camera->GetCameraMatrix());
//	// 平行移動成分はなし
//	billboardMatrix.m[3][0] = 0.0f;
//	billboardMatrix.m[3][1] = 0.0f;
//	billboardMatrix.m[3][2] = 0.0f;
//
//	for (std::list<ParticleData>::iterator it = particles_.begin(); it != particles_.end(); ++it) {
//
//		it->transform.scale = transform_.scale;
//		it->transform.rotate = transform_.rotate;
//
//		Matrix4x4 scaleMatrix = Matrix4x4::MakeScaleMatrix(it->transform.scale);
//		Matrix4x4 translateMatrix = Matrix4x4::MakeTranslateMatrix(it->transform.translate);
//
//		if (useBillboard_) {
//
//			worldMatrix_ = Matrix4x4::Multiply(scaleMatrix, Matrix4x4::Multiply(billboardMatrix, translateMatrix));
//		} else {
//
//			worldMatrix_ = Matrix4x4::Multiply(scaleMatrix, Matrix4x4::Multiply(Matrix4x4::MakeIdentity4x4(), translateMatrix));
//		}
//
//		wvpMatrix_ =
//			Matrix4x4::Multiply(worldMatrix_, Matrix4x4::Multiply(camera->GetViewMatrix(), camera->GetProjectionMatrix()));
//
//		// Matrix
//		cBuffer_->particleMatrix->particleData[std::distance(particles_.begin(), it)].World = worldMatrix_;
//		cBuffer_->particleMatrix->particleData[std::distance(particles_.begin(), it)].WVP = wvpMatrix_;
//		cBuffer_->particleMatrix->particleData[std::distance(particles_.begin(), it)].color = it->color;
//	}
//
//	// Material
//	cBuffer_->material->data->enableLighting = false;
//	cBuffer_->material->data->enableHalfLambert = false;
//	cBuffer_->material->data->uvTransform = Matrix4x4::MakeIdentity4x4();
//}
//
//
//
///*////////////////////////////////////////////////////////////////////////////////
//*								    更新処理
//////////////////////////////////////////////////////////////////////////////////*/
//void Particle::Update(Camera3D* camera) {
//
//	// 乱数の生成
//	std::random_device seedGenerator;
//	std::mt19937 randomEngine(seedGenerator());
//
//	ImGui::Begin("particle");
//
//	ImGui::ColorEdit4("color", &material_.color.x);
//	ImGui::SliderFloat3("scale", &transform_.scale.x, 0.0f, 1.0f);
//	ImGui::SliderAngle("rotateX", &transform_.rotate.x);
//	ImGui::SliderAngle("rotateY", &transform_.rotate.y);
//	ImGui::SliderAngle("rotateZ", &transform_.rotate.z);
//	ImGui::Checkbox("useBillboard", &useBillboard_);
//	ImGui::DragFloat3("EmitterTranslate", &emitter_.transform.translate.x, 0.01f, -100.0f, 100.0f);
//
//	if (ImGui::Button("Add Particle")) {
//
//		particles_.splice(particles_.end(), Emit(emitter_, randomEngine));
//	}
//
//	ImGui::End();
//
//	// 描画すべきインスタンス数
//	uint32_t numInstance = 0;
//
//	// billboard変換
//	Matrix4x4 backToFrontMatrix = Matrix4x4::MakeYawMatrix(std::numbers::pi_v<float>);
//	Matrix4x4 billboardMatrix = Matrix4x4::Multiply(backToFrontMatrix, camera->GetCameraMatrix());
//	// 平行移動成分はなし
//	billboardMatrix.m[3][0] = 0.0f;
//	billboardMatrix.m[3][1] = 0.0f;
//	billboardMatrix.m[3][2] = 0.0f;
//
//	// 時刻を進める
//	emitter_.frequencyTime += kDeltaTime;
//
//	if (emitter_.frequency <= emitter_.frequencyTime) {
//
//		// 発生処理
//		particles_.splice(particles_.end(), Emit(emitter_, randomEngine));
//
//		// 余計に過ぎた時間を加味して頻度計算
//		emitter_.frequencyTime -= emitter_.frequency;
//	}
//
//	for (std::list<ParticleData>::iterator it = particles_.begin(); it != particles_.end();) {
//
//		if (numInstance < instanceMaxCount_) {
//
//			if ((*it).lifeTime <= (*it).currentTime) {
//
//				// 生存期間が過ぎたparticleをリストから消す
//				it = particles_.erase(it);
//
//				continue;
//			}
//
//			// 衝突判定していたら加速させる
//			if (IsCollision(accelerationField_.area, it->transform.translate)) {
//
//				it->velocity +=
//				{accelerationField_.acceleration.x* kDeltaTime,
//					accelerationField_.acceleration.y* kDeltaTime,
//					accelerationField_.acceleration.z* kDeltaTime};
//			}
//
//			// 速度を適用
//			it->transform.translate += {
//
//				it->velocity.x* kDeltaTime,
//					it->velocity.y* kDeltaTime,
//					it->velocity.z* kDeltaTime
//			};
//
//			// 経過時間を足す
//			it->currentTime += kDeltaTime;
//
//			it->transform.scale = transform_.scale;
//			it->transform.rotate = transform_.rotate;
//
//			Matrix4x4 scaleMatrix = Matrix4x4::MakeScaleMatrix(it->transform.scale);
//			Matrix4x4 translateMatrix = Matrix4x4::MakeTranslateMatrix(it->transform.translate);
//
//			if (useBillboard_) {
//
//				worldMatrix_ = Matrix4x4::Multiply(scaleMatrix, Matrix4x4::Multiply(billboardMatrix, translateMatrix));
//			} else {
//
//				worldMatrix_ = Matrix4x4::Multiply(scaleMatrix, Matrix4x4::Multiply(Matrix4x4::MakeIdentity4x4(), translateMatrix));
//			}
//
//			wvpMatrix_ = Matrix4x4::Multiply(worldMatrix_, Matrix4x4::Multiply(camera->GetViewMatrix(), camera->GetProjectionMatrix()));
//
//			float alpha = 1.0f - (it->currentTime / it->lifeTime);
//
//			// Matrix
//			cBuffer_->particleMatrix->particleData[numInstance].World = worldMatrix_;
//			cBuffer_->particleMatrix->particleData[numInstance].WVP = wvpMatrix_;
//			cBuffer_->particleMatrix->particleData[numInstance].color = it->color;
//			cBuffer_->particleMatrix->particleData[numInstance].color.w = alpha;
//
//			++numInstance;
//		}
//
//		++it;
//	}
//
//	// インスタンス数の更新
//	numInstance_ = numInstance;
//
//	// Material
//	cBuffer_->material->data->color = material_.color;
//	cBuffer_->material->data->enableLighting = material_.enableLighting;
//	cBuffer_->material->data->enableHalfLambert = material_.enableLighting;
//}
//
//
//
///*////////////////////////////////////////////////////////////////////////////////
//*								    描画処理
//////////////////////////////////////////////////////////////////////////////////*/
//void Particle::Draw() {
//
//	Engine::DrawParticle(
//		cBuffer_.get(), static_cast<UINT>(modelData_.vertices.size()), numInstance_, instancingSrvHandleGPU_,
//		modelName_, textureName_, pParticle, blendMode_);
//}