#pragma once

// DirectX
#include "DXInclude.h"

/*////////////////////////////////////////////////////////////////////////////////
*
*								DXBlendState Class
*
////////////////////////////////////////////////////////////////////////////////*/
class DXRasterizerState {
public:
	/*-----------------------------*/
	///			メンバ関数
	/*-----------------------------*/

	void Create();

	// getter

	D3D12_RASTERIZER_DESC GetRasterizerDesc();

private:
	/*-----------------------------*/
	///			メンバ変数
	/*-----------------------------*/

	D3D12_RASTERIZER_DESC rasterizerDesc_{};
};