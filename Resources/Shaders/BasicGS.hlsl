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
[maxvertexcount(3)]
void main(
	point VertexShaderOutput input[1],
	inout TriangleStream<GSOutput> output) {
	
	GSOutput element;
	
	// 共通
	element.normal = input[0].normal;
	element.texcoord = input[0].texcoord;
	
	// 1点目
	element.svpos = input[0].position;
	output.Append(element);
	
	// 2点目
	element.svpos = input[0].position + float4(10.0f, 10.0f, 0.0f, 0.0f);
	output.Append(element);
	
	// 3点目
	element.svpos = input[0].position + float4(10.0f, 0.0f, 0.0f, 0.0f);
	output.Append(element);
	
}