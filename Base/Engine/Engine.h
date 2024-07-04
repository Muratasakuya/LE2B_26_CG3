#pragma once

// Windows
#include <Windows.h>

// c++
#include <cstdint>
#include <memory>
#include <iostream>
#include <string>
#include <tuple>
#include <utility>

// 2D
#include "Sprite.h"

// 3D
#include "Mesh.h"

// Managers
#include "PipelineManager.h"

class WinApp;
class DXCommon;
class ImGuiManager;
class TextureManager;
class ModelManager;
class ConstBufferManager;

/*////////////////////////////////////////////////////////////////////////////////
*
*								Engine Class
*
////////////////////////////////////////////////////////////////////////////////*/
class Engine {
public:
	/*-----------------------------*/
	///			メンバ関数
	/*-----------------------------*/

	// 各システムの初期化
	static void Initialize(uint32_t width, uint32_t height);

	// フレーム開始処理
	static void BeginFrame();

	// フレーム終了処理
	static void EndFrame();

	// システムの終了
	static void Finalize();

	// メッセージの受け渡し処理
	static bool ProcessMessage();

	/*--------------------------------------------------------------------*/
	/// 描画関数

	// 三角形
	static void DrawTriangle(const std::string& name, const std::string textureName, PipelineType pipelineType, BlendMode blendMode);

	// スプライト
	static void DrawSprite(const std::string& name, const std::string textureName, PipelineType pipelineType, BlendMode blendMode);

	// 球
	static void DrawSphere(const std::string& name, const std::string textureName, PipelineType pipelineType, BlendMode blendMode);

	// モデル
	static void DrawModel(const std::string& name, const std::string modelName, const std::string textureName, PipelineType pipelineType, BlendMode blendMode);

	// パーティクル
	static void DrawParticle(
		const std::string& name, const UINT vertexNum, const UINT instanceCount, D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle,
		const std::string modelName, const std::string textureName, PipelineType pipelineType, BlendMode blendMode);

	// GS
	static void DrawGSModel(const std::string& name, const std::string textureName, PipelineType pipelineType, BlendMode blendMode);

private:
	/*-----------------------------*/
	///			メンバ変数
	/*-----------------------------*/

	static WinApp* winApp_;
	static DXCommon* dxCommon_;
	static ImGuiManager* imguiManager_;
	static TextureManager* textureManger_;
	static ModelManager* modelManager_;
	static ConstBufferManager* constBufferManager_;
	static std::unique_ptr<Mesh> mesh_;
	static std::unique_ptr<Sprite> sprite_;
	static std::unique_ptr<PipelineManager> pipelineManager_;
};