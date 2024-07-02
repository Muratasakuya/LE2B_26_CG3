
struct VertexShaderOutput {
    
	float4 position : POSITION;
};

// ジオメトリシェーダからピクセルシェーダへの出力
struct GSOutput {
	
	float4 svpos : SV_POSITION;
	float2 texcoord : TEXCOORD0;
};

/*===========================================================
					   Basic GS Shader
===========================================================*/

// 四角形の頂点数
static const uint vNum = 4;

// センターからのオフセット
static const float4 offsetArray[vNum] = {
	
	float4(-0.5f, -0.5f, 0.0f, 0.0f),
	float4(-0.5f, 0.5f, 0.0f, 0.0f),
	float4(0.5f, -0.5f, 0.0f, 0.0f),
	float4(0.5f, 0.5f, 0.0f, 0.0f),
};

// 左上が0,0,右上が1,1
static const float2 uvArray[vNum] = {
	
	float2(0.0f, 1.0f),
	float2(0.0f, 0.0f),
	float2(1.0f, 1.0f),
	float2(1.0f, 0.0f),
};

struct GSMatrix {
	
	float4x4 gsMatix;
};

ConstantBuffer<GSMatrix> gGSMatirx : register(b0);

// 最大頂点数
[maxvertexcount(vNum)]
void main(
	point VertexShaderOutput input[1],
	inout TriangleStream<GSOutput> output) {
	
	GSOutput element;
	
	// 4点分回す
	for (uint i = 0; i < vNum; i++) {
		
		// ワールド座標ベースでずらす
		element.svpos = input[0].position + offsetArray[i];
		
		//　ビュー、射影変換
		element.svpos = mul(element.svpos, gGSMatirx.gsMatix);
		element.texcoord = uvArray[i];
		
		output.Append(element);
	}
	
}