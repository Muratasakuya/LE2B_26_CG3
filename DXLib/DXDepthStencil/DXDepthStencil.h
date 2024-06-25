#pragma once

// DirectX
#include "DXInclude.h"

/*////////////////////////////////////////////////////////////////////////////////
*
*								DXDepthStencil Class
*
////////////////////////////////////////////////////////////////////////////////*/
class DXDepthStencil{
public:
	/*-----------------------------*/
	///			メンバ関数
	/*-----------------------------*/

	DXDepthStencil() {};
	~DXDepthStencil() {};

	void Create(bool depthEnable);

	// getter

	D3D12_DEPTH_STENCIL_DESC GetDepthStencilDesc();

private:
	/*-----------------------------*/
	///			メンバ変数
	/*-----------------------------*/

	D3D12_DEPTH_STENCIL_DESC depthStencilDesc_{};
};

