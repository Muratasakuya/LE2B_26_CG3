#include "Engine.h"

#include "DXCommon.h"
#include "WinApp.h"
#include "ImGuiManager.h"
#include "TextureManager.h"
#include "ModelManager.h"



// static members
WinApp* Engine::winApp_ = nullptr;
DXCommon* Engine::dxCommon_ = nullptr;
ImGuiManager* Engine::imguiManager_ = nullptr;
TextureManager* Engine::textureManger_ = nullptr;
ModelManager* Engine::modelManger_ = nullptr;
std::unique_ptr<Mesh> Engine::mesh_ = nullptr;
std::unique_ptr<Sprite> Engine::sprite_ = nullptr;
std::unique_ptr<PipelineManager> Engine::pipelineManager_ = nullptr;



/*////////////////////////////////////////////////////////////////////////////////

*								 フレーム開始処理

////////////////////////////////////////////////////////////////////////////////*/
void Engine::BeginFrame() {

	imguiManager_->Begin();
	dxCommon_->PreDraw();
}



/*////////////////////////////////////////////////////////////////////////////////

*								 フレーム終了処理

////////////////////////////////////////////////////////////////////////////////*/
void Engine::EndFrame() {

	imguiManager_->End();
	imguiManager_->Draw();
	dxCommon_->PostDraw();

	// メッシュ使用カウントリセット
	mesh_->ResetMeshCount();
	sprite_->ResetMeshCount();
}



/*////////////////////////////////////////////////////////////////////////////////

*							  メッセージの受け渡し処理

////////////////////////////////////////////////////////////////////////////////*/
bool Engine::ProcessMessage() {

	if (winApp_->ProcessMessage()) {

		return 1;
	} else {

		return 0;
	}
}



/*////////////////////////////////////////////////////////////////////////////////

*									終了処理

////////////////////////////////////////////////////////////////////////////////*/
void Engine::Finalize() {

	imguiManager_->Finalize();
	dxCommon_->Finalize(winApp_);

	mesh_.reset();
	sprite_.reset();
	pipelineManager_.reset();

	// ComFinalize
	CoUninitialize();
}



/*////////////////////////////////////////////////////////////////////////////////

*								メインシステムの初期化

////////////////////////////////////////////////////////////////////////////////*/
void Engine::Initialize(uint32_t width, uint32_t height) {

	// ComInitialize
	CoInitializeEx(nullptr, COINIT_MULTITHREADED);

	winApp_ = WinApp::GetInstance();
	dxCommon_ = DXCommon::GetInstance();
	textureManger_ = TextureManager::GetInstance();
	modelManger_ = ModelManager::GetInstance();
	mesh_ = std::make_unique<Mesh>();
	sprite_ = std::make_unique<Sprite>();
	pipelineManager_ = std::make_unique<PipelineManager>();

	// ウィンドウ生成
	winApp_->CreateMainWindow(width, height);

	// DirectXの初期化
	dxCommon_->Initialize(winApp_, width, height);

	// ImGuiの初期化
	imguiManager_->Initialize(winApp_, dxCommon_);

	// パイプラインの各種初期化
	pipelineManager_->CreatePipelineStateObject(dxCommon_);

	// モデルメッシュインスタンスの生成
	modelManger_->Initialize();

	// 各メッシュ生成
	mesh_->CreateMeshes();
	sprite_->CreateMesh();
}

/*------------------------------------------------------------------------------*/
/// 描画関数

// 三角形
void Engine::DrawTriangle(const CBufferData* cBufferData, const std::string textureName, PipelineType pipelineType, BlendMode blendMode) {

	// CommandListをdxCommonClassからもってくる
	ComPtr<ID3D12GraphicsCommandList> commandList = dxCommon_->GetCommandList();

	// 頂点バッファへデータ転送
	mesh_->VertexBufferMemcpy();
	// パイプラインのセット
	pipelineManager_->SetGraphicsPipeline(commandList.Get(), pipelineType, blendMode);
	// 頂点バッファの設定
	commandList->IASetVertexBuffers(0, 1, &mesh_->GetTriangle()->vertexBufferView);
	// マテリアルCBufferの場所を設定
	commandList->SetGraphicsRootConstantBufferView(0, cBufferData->material->resource->GetGPUVirtualAddress());
	// wvp用のCBufferの場所を設定
	commandList->SetGraphicsRootConstantBufferView(1, cBufferData->matrix->resource->GetGPUVirtualAddress());
	if (pipelineType == Texture) {
		// light用のCBufferの場所を設定
		commandList->SetGraphicsRootConstantBufferView(3, cBufferData->light->resource->GetGPUVirtualAddress());
	}
	if (pipelineType == Texture) {
		// SRVのセット
		textureManger_->SetGraphicsRootDescriptorTable(commandList.Get(), 2, textureName);
	}

	// DrawCall
	mesh_->TriangleDrawCall(commandList.Get());
}

// スプライト
void Engine::DrawSprite(const CBufferData* cBufferData, const std::string textureName, PipelineType pipelineType, BlendMode blendMode) {

	// CommandListをdxCommonClassからもってくる
	ComPtr<ID3D12GraphicsCommandList> commandList = dxCommon_->GetCommandList();

	// 頂点データの作成
	sprite_->SetSpriteData(sprite_->GetSprite()->data, sprite_->GetSprite()->index);
	// パイプラインのセット
	pipelineManager_->SetGraphicsPipeline(commandList.Get(), pipelineType, blendMode);
	// 頂点バッファの設定
	commandList->IASetVertexBuffers(0, 1, &sprite_->GetSprite()->vertexBufferView);
	// インデックスバッファの設定
	commandList->IASetIndexBuffer(&sprite_->GetSprite()->indexBufferView);
	// マテリアルCBufferの場所を設定
	commandList->SetGraphicsRootConstantBufferView(0, cBufferData->material->resource->GetGPUVirtualAddress());
	// wvp用のCBufferの場所を設定
	commandList->SetGraphicsRootConstantBufferView(1, cBufferData->matrix->resource->GetGPUVirtualAddress());
	if (pipelineType == Texture) {
		// light用のCBufferの場所を設定
		commandList->SetGraphicsRootConstantBufferView(3, cBufferData->light->resource->GetGPUVirtualAddress());
	}
	// SRVのセット
	textureManger_->SetGraphicsRootDescriptorTable(commandList.Get(), 2, textureName);

	// DrawCall
	sprite_->SpriteDrawCall(commandList.Get());
}

// 球
void Engine::DrawSphere(const CBufferData* cBufferData, const std::string textureName, PipelineType pipelineType, BlendMode blendMode) {

	// CommandListをdxCommonClassからもってくる
	ComPtr<ID3D12GraphicsCommandList> commandList = dxCommon_->GetCommandList();

	// 頂点データの作成
	mesh_->SetSphereData(mesh_->GetSphere()->data, mesh_->GetSphere()->index);
	// パイプラインのセット
	pipelineManager_->SetGraphicsPipeline(commandList.Get(), pipelineType, blendMode);
	// 頂点バッファの設定
	commandList->IASetVertexBuffers(0, 1, &mesh_->GetSphere()->vertexBufferView);
	// インデックスバッファの設定
	commandList->IASetIndexBuffer(&mesh_->GetSphere()->indexBufferView);
	// wvp用のCBufferの場所を設定
	commandList->SetGraphicsRootConstantBufferView(1, cBufferData->matrix->resource->GetGPUVirtualAddress());
	// light用のCBufferの場所を設定
	commandList->SetGraphicsRootConstantBufferView(3, cBufferData->light->resource->GetGPUVirtualAddress());
	if (pipelineType == Texture) {
		// マテリアルCBufferの場所を設定
		commandList->SetGraphicsRootConstantBufferView(0, cBufferData->material->resource->GetGPUVirtualAddress());
	}
	if (pipelineType == PhongReflection) {
		// マテリアルCBufferの場所を設定
		commandList->SetGraphicsRootConstantBufferView(0, cBufferData->phongRefMaterial->resource->GetGPUVirtualAddress());
		// light用のCBufferの場所を設定
		commandList->SetGraphicsRootConstantBufferView(4, cBufferData->camera->resource->GetGPUVirtualAddress());
	}
	// SRVのセット
	textureManger_->SetGraphicsRootDescriptorTable(commandList.Get(), 2, textureName);

	// DrawCall
	mesh_->SphereDrawCall(commandList.Get());
}

// モデル
void Engine::DrawModel(const CBufferData* cBufferData, const std::string modelName, const std::string textureName, PipelineType pipelineType, BlendMode blendMode) {

	// CommandListをdxCommonClassからもってくる
	ComPtr<ID3D12GraphicsCommandList> commandList = dxCommon_->GetCommandList();

	// 頂点バッファへデータ転送
	modelManger_->VertexBufferMemcpy(modelName);
	// パイプラインのセット
	pipelineManager_->SetGraphicsPipeline(commandList.Get(), pipelineType, blendMode);
	// 頂点バッファの設定
	modelManger_->IASetVertexBuffers(commandList.Get(), modelName);
	// マテリアルCBufferの場所を設定
	commandList->SetGraphicsRootConstantBufferView(0, cBufferData->material->resource->GetGPUVirtualAddress());
	// wvp用のCBufferの場所を設定
	commandList->SetGraphicsRootConstantBufferView(1, cBufferData->matrix->resource->GetGPUVirtualAddress());
	if (pipelineType == Texture) {
		// light用のCBufferの場所を設定
		commandList->SetGraphicsRootConstantBufferView(3, cBufferData->light->resource->GetGPUVirtualAddress());
	}
	// SRVのセット
	textureManger_->SetGraphicsRootDescriptorTable(commandList.Get(), 2, textureName);

	// DrawCall
	modelManger_->ModelDrawCall(commandList.Get(), modelName);
}

// パーティクル
void Engine::DrawParticle(
	const CBufferData* cBufferData, const UINT vertexNum, const UINT instanceCount, D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle,
	const std::string modelName, const std::string textureName, PipelineType pipelineType, BlendMode blendMode) {

	// CommandListをdxCommonClassからもってくる
	ComPtr<ID3D12GraphicsCommandList> commandList = dxCommon_->GetCommandList();

	// 頂点バッファへデータ転送
	modelManger_->VertexBufferMemcpy(modelName);
	// パイプラインのセット
	pipelineManager_->SetGraphicsPipeline(commandList.Get(), pipelineType, blendMode);
	// 頂点バッファの設定
	modelManger_->IASetVertexBuffers(commandList.Get(), modelName);
	// マテリアルCBufferの場所を設定
	commandList->SetGraphicsRootConstantBufferView(0, cBufferData->material->resource->GetGPUVirtualAddress());
	// wvp用のCBufferの場所を設定
	commandList->SetGraphicsRootConstantBufferView(1, cBufferData->particleMatrix->resource->GetGPUVirtualAddress());
	// SRVのセット
	textureManger_->SetGraphicsRootDescriptorTable(commandList.Get(), 2, textureName);
	// instacning
	commandList->SetGraphicsRootDescriptorTable(3, gpuHandle);
	
	// DrawCall
	commandList->DrawInstanced(vertexNum, instanceCount, 0, 0);
}