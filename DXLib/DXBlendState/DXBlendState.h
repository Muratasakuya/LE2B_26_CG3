#pragma once

// DirectX
#include "DXInclude.h"

/*////////////////////////////////////////////////////////////////////////////////
*
*								DXBlendState Class
*
////////////////////////////////////////////////////////////////////////////////*/
class DXBlendState {
public:
	/*-----------------------------*/
	///			メンバ関数
	/*-----------------------------*/

	DXBlendState() {};
	~DXBlendState() {};

	void Create();

	// getter

	D3D12_BLEND_DESC GetBlendDesc();

private:
	/*-----------------------------*/
	///			メンバ変数
	/*-----------------------------*/

	D3D12_BLEND_DESC blendDesc_{};
};