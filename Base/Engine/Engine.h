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

/*////////////////////////////////////////////////////////////////////////////////
*
*									Engine Class
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
	static void DrawTriangle(const CBufferData* cBufferData, PipelineType pipelineType, const std::string textureName);

	// スプライト
	static void DrawSprite(const CBufferData* cBufferData, PipelineType pipelineType,const std::string textureName);

	// 球
	static void DrawSphere(const CBufferData* cBufferData, PipelineType pipelineType, const std::string textureName);

	// モデル
	static void DrawModel(const CBufferData* cBufferData, PipelineType pipelineType, const std::string modelName, const std::string textureName);

private:
	/*-----------------------------*/
	///			メンバ変数
	/*-----------------------------*/

	static WinApp* winApp_;
	static DXCommon* dxCommon_;
	static ImGuiManager* imguiManager_;
	static TextureManager* textureManger_;
	static ModelManager* modelManger_;
	static std::unique_ptr<Mesh> mesh_;
	static std::unique_ptr<Sprite> sprite_;
	static std::unique_ptr<PipelineManager> pipelineManager_;
};