#pragma once

// DirectX
#include "DXInclude.h"

// Lib
#include "Logger.h"

// c++
#include <array>

class DXCommon;

/*////////////////////////////////////////////////////////////////////////////////
*
*							DXRootSignature Class
*
////////////////////////////////////////////////////////////////////////////////*/
class DXRootSignature {
public:
	/*-----------------------------*/
	///			メンバ関数
	/*-----------------------------*/

	DXRootSignature() {};
	~DXRootSignature() {};

	void Create(DXCommon* dxCommon, PipelineType pipelineType);

	// getter

	ID3D12RootSignature* GetRootSignature(PipelineType pipelineType) const;
	ID3DBlob* GetErrorBlob(PipelineType pipelineType) const;

private:
	/*-----------------------------*/
	///			メンバ変数
	/*-----------------------------*/

	std::array<ComPtr<ID3D12RootSignature>, pipelineTypeNum> rootSignature_ = { nullptr,nullptr };
	ComPtr<ID3DBlob> signatureBlob_ = nullptr;
	std::array<ComPtr<ID3DBlob>, pipelineTypeNum> errorBlob_ = { nullptr };
};