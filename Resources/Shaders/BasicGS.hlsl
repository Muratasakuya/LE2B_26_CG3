#include "Object3D.hlsli"

// ジオメトリシェーダからピクセルシェーダへの出力
struct GSOutput {
	
	float4 svpos : SV_POSITION;
	float2 texcoord : TEXCOORD0;
	float3 normal : NORMAL0;
};

/*===========================================================
					   Basic GS Shader
===========================================================*/

// 最大頂点数
[maxvertexcount(6)]
void main(
	triangle VertexShaderOutput input[3],
	inout TriangleStream<GSOutput> output) {
	
	// 1つ目の三角形
	for (uint i = 0; i < 3; i++) {
		
		// 出力用頂点データ
		GSOutput element;
		
		element.svpos = input[i].position;
		element.normal = input[i].normal;
		element.texcoord = input[i].texcoord;
		
		output.Append(element);
	}
	// 現在のストリップを終了
	output.RestartStrip();
	
	// 2つ目の三角形
	for (uint j = 0; j < 3; j++) {
		
		// 出力用頂点データ
		GSOutput element;
		
		// X方向に20.0fずらす
		element.svpos = input[j].position + float4(20.0f, 0.0f, 0.0f, 0.0f);
		element.normal = input[j].normal;
		
		// UV5倍
		element.texcoord = input[j].texcoord * 5.0f;
		
		output.Append(element);
	}
	
}