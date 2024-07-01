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
[maxvertexcount(3)]
void main(
	point VertexShaderOutput input[1],
	inout TriangleStream<GSOutput> output) {
	
	GSOutput element;
	
	// ����
	element.normal = input[0].normal;
	element.texcoord = input[0].texcoord;
	
	// 1�_��
	element.svpos = input[0].position;
	output.Append(element);
	
	// 2�_��
	element.svpos = input[0].position + float4(10.0f, 10.0f, 0.0f, 0.0f);
	output.Append(element);
	
	// 3�_��
	element.svpos = input[0].position + float4(10.0f, 0.0f, 0.0f, 0.0f);
	output.Append(element);
	
}