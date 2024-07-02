
// �W�I���g���V�F�[�_����s�N�Z���V�F�[�_�ւ̏o��
struct GSOutput {
	
	float4 svpos : SV_POSITION;
	float2 texcoord : TEXCOORD0;
};

/*===========================================================
                     Basic PS.GS  Shader
===========================================================*/

Texture2D<float4> gTexture : register(t0);
SamplerState gSampler : register(s0);

float4 main(GSOutput input) : SV_TARGET {
	
	return gTexture.Sample(gSampler, input.texcoord);
}