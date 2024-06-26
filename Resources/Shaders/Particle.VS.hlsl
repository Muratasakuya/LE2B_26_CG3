#include "Particle.hlsli"

/*===========================================================
                     Particle VS Shader
===========================================================*/

struct TransformationMatrix
{
    float4x4 World;
    float4x4 WVP;
};

struct VertexShaderInput
{
    float4 position : POSITION0;
    float2 texcoord : TEXCOORD0;
    float3 normal : NORMAL0;
};

StructuredBuffer<TransformationMatrix> gTransformationMatrices : register(t0);

VertexShaderOutput main(VertexShaderInput input, uint32_t instanceId : SV_InstanceID)
{
    VertexShaderOutput output;
    output.position = mul(input.position, gTransformationMatrices[instanceId].WVP);
    output.texcoord = input.texcoord;
    output.normal = normalize(mul(input.normal, (float3x3) gTransformationMatrices[instanceId].World));
    
    return output;
}