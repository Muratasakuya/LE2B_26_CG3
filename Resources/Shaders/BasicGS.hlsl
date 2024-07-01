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
	triangle VertexShaderOutput input[3],
	inout TriangleStream<GSOutput> output) {
	
	for (uint i = 0; i < 3; i++) {
		
		GSOutput element;
		element.svpos = input[i].position;
		element.texcoord = input[i].texcoord;
		element.normal = input[i].normal;
		
		output.Append(element);
	}
}