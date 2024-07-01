#include "Object3D.hlsli"

// �W�I���g���V�F�[�_����s�N�Z���V�F�[�_�ւ̏o��
struct GSOutput {
	
	float4 svpos : SV_POSITION;
	float2 texcoord : TEXCOORD0;
	float3 normal : NORMAL0;
};

/*===========================================================
					   Basic GS Shader
===========================================================*/

// �ő咸�_��
[maxvertexcount(6)]
void main(
	triangle VertexShaderOutput input[3],
	inout TriangleStream<GSOutput> output) {
	
	// 1�ڂ̎O�p�`
	for (uint i = 0; i < 3; i++) {
		
		// �o�͗p���_�f�[�^
		GSOutput element;
		
		element.svpos = input[i].position;
		element.normal = input[i].normal;
		element.texcoord = input[i].texcoord;
		
		output.Append(element);
	}
	// ���݂̃X�g���b�v���I��
	output.RestartStrip();
	
	// 2�ڂ̎O�p�`
	for (uint j = 0; j < 3; j++) {
		
		// �o�͗p���_�f�[�^
		GSOutput element;
		
		// X������20.0f���炷
		element.svpos = input[j].position + float4(20.0f, 0.0f, 0.0f, 0.0f);
		element.normal = input[j].normal;
		
		// UV5�{
		element.texcoord = input[j].texcoord * 5.0f;
		
		output.Append(element);
	}
	
}