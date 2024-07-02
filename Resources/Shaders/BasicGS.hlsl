
struct VertexShaderOutput {
    
	float4 position : POSITION;
};

// �W�I���g���V�F�[�_����s�N�Z���V�F�[�_�ւ̏o��
struct GSOutput {
	
	float4 svpos : SV_POSITION;
	float2 texcoord : TEXCOORD0;
};

/*===========================================================
					   Basic GS Shader
===========================================================*/

// �l�p�`�̒��_��
static const uint vNum = 4;

// �Z���^�[����̃I�t�Z�b�g
static const float4 offsetArray[vNum] = {
	
	float4(-0.5f, -0.5f, 0.0f, 0.0f),
	float4(-0.5f, 0.5f, 0.0f, 0.0f),
	float4(0.5f, -0.5f, 0.0f, 0.0f),
	float4(0.5f, 0.5f, 0.0f, 0.0f),
};

// ���オ0,0,�E�オ1,1
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

// �ő咸�_��
[maxvertexcount(vNum)]
void main(
	point VertexShaderOutput input[1],
	inout TriangleStream<GSOutput> output) {
	
	GSOutput element;
	
	// 4�_����
	for (uint i = 0; i < vNum; i++) {
		
		// ���[���h���W�x�[�X�ł��炷
		element.svpos = input[0].position + offsetArray[i];
		
		//�@�r���[�A�ˉe�ϊ�
		element.svpos = mul(element.svpos, gGSMatirx.gsMatix);
		element.texcoord = uvArray[i];
		
		output.Append(element);
	}
	
}