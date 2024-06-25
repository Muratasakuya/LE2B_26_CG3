#include "DXRasterizerState.h"



// rasterizerDesc_ getter
D3D12_RASTERIZER_DESC DXRasterizerState::GetRasterizerDesc() {

	return rasterizerDesc_;
}



/*////////////////////////////////////////////////////////////////////////////////

*							DXRasterizerStateの生成

////////////////////////////////////////////////////////////////////////////////*/
void DXRasterizerState::Create() {

	// 裏面(時計周り)を表示しない、背面カリング
	rasterizerDesc_.CullMode = D3D12_CULL_MODE_BACK;
	//三角形の中を塗りつぶす
	rasterizerDesc_.FillMode = D3D12_FILL_MODE_SOLID;
}