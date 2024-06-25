#include "DXBlendState.h"



// blendDesc_ getter
D3D12_BLEND_DESC DXBlendState::GetBlendDesc() {

	return blendDesc_;
}



/*////////////////////////////////////////////////////////////////////////////////

*								DXBlendStateの生成

////////////////////////////////////////////////////////////////////////////////*/
void DXBlendState::Create() {

	// 全ての色要素を書き込む
	// ブレンドモードNone D3D12_COLOR_WRITE_ENABLE_ALLだけ
	blendDesc_.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	blendDesc_.RenderTarget[0].BlendEnable = TRUE;
	blendDesc_.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blendDesc_.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
	blendDesc_.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
	blendDesc_.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
	blendDesc_.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blendDesc_.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;
}