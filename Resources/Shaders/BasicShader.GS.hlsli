
// ジオメトリシェーダからピクセルシェーダへの出力
struct GSOutput {
	
	float4 svpos : SV_POSITION;
	float2 texcoord : TEXCOORD0;
	float3 normal : NORMAL0;
};