#include "Object3D.hlsli"

/*===========================================================
                     Particle PS Shader
===========================================================*/

struct Material
{
    
    float4 color;
    int enableLighting;
    int enableHalfLambert;
    float4x4 uvTransform;
};

struct PixelShaderOutput
{
    
    float4 color : SV_TARGET0;
};

ConstantBuffer<Material> gMaterial : register(b0);
Texture2D<float4> gTexture : register(t0);
SamplerState gSampler : register(s0);

PixelShaderOutput main(VertexShaderOutput input)
{
    
    float4 transformUV = mul(float4(input.texcoord, 0.0f, 1.0f), gMaterial.uvTransform);
    float4 textureColor = gTexture.Sample(gSampler, transformUV.xy);
    
    PixelShaderOutput output;
    
     // textureのα値が0.5f以下の時にPixelを棄却
    if (textureColor.a <= 0.5f)
    {
            
        discard;
    }
    
    output.color.rgb = gMaterial.color.rgb * textureColor.rgb;
    output.color.a = gMaterial.color.a * textureColor.a;
    
    return output;
}