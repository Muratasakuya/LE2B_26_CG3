#pragma once

// DirectX
#include "DXInclude.h"

#include "DXRootSignature.h"
#include "DXInputLayout.h"
#include "DXBlendState.h"
#include "DXRasterizerState.h"
#include "DXDepthStencil.h"

// c++
#include <memory>
#include <array>

class DXCommon;

/*////////////////////////////////////////////////////////////////////////////////
*
*							PipelineManager Class
*
////////////////////////////////////////////////////////////////////////////////*/
class PipelineManager {
public:
	/*-----------------------------*/
	///			メンバ関数
	/*-----------------------------*/

	PipelineManager() {};
	~PipelineManager() {};

	void CreatePipelineStateObject(DXCommon* dxCommon);

	void SetGraphicsPipeline(ID3D12GraphicsCommandList* commandList, PipelineType pipeType);

private:
	/*-----------------------------*/
	///			メンバ変数
	/*-----------------------------*/

	std::array<PipelineType, pipelineTypeNum> pipelineTypes_{};

	std::unique_ptr<DXRootSignature> rootSignature_;
	std::unique_ptr<DXInputLayout> inputLayout_;
	std::unique_ptr<DXBlendState> blendState_;
	std::unique_ptr<DXRasterizerState> rasterizerState_;
	std::unique_ptr<DXDepthStencil> depthStencil_;

	std::array<ComPtr<IDxcBlob>, pipelineTypeNum> vsBlob_;
	std::array<ComPtr<IDxcBlob>, pipelineTypeNum> psBlob_;

	std::array<ComPtr<ID3D12PipelineState>, pipelineTypeNum> pipelineState_;
	D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelineStateDesc_{};

	// シェーダのコンパイル
	IDxcBlob* CompileShader(
		// CompilerするShaderファイルへのパス
		const std::wstring& filePath,
		// Compilerに使用するProfile
		const wchar_t* profile,
		// 初期化で生成したものを3つ
		IDxcUtils* dxcUtils,
		IDxcCompiler3* dxcCompiler,
		IDxcIncludeHandler* includeHandler
	);

	void CreatePipelineState(
		DXCommon* dxCommon, IDxcBlob* vs, IDxcBlob* ps, ID3D12RootSignature* rootSigature, D3D12_INPUT_LAYOUT_DESC inputLayout,
		D3D12_BLEND_DESC blendDesc, D3D12_RASTERIZER_DESC rasterizerDesc, D3D12_DEPTH_STENCIL_DESC depthStencilDesc, PipelineType pipelineType);

	void ShaderCompile(DXCommon* dxCommon, PipelineType pipelineType);
};