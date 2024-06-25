#include "Object3D.hlsli"

/*===========================================================
                     Object3D PS Shader
===========================================================*/

struct Material
{
    
    float4 color;
    int enableLighting;
    int enableHalfLambert;
    float4x4 uvTransform;
};

struct DirectionalLight
{

    float4 color;
    float3 direction;
    float intensity;
};

struct PixelShaderOutput
{
    
    float4 color : SV_TARGET0;
};

ConstantBuffer<Material> gMaterial : register(b0);
ConstantBuffer<DirectionalLight> gDirectionalLight : register(b1);
Texture2D<float4> gTexture : register(t0);
SamplerState gSampler : register(s0);

PixelShaderOutput main(VertexShaderOutput input)
{
    
    float4 transformUV = mul(float4(input.texcoord, 0.0f, 1.0f), gMaterial.uvTransform);
    float4 textureColor = gTexture.Sample(gSampler, transformUV.xy);
    
    PixelShaderOutput output;
    
    if (gMaterial.enableLighting != 0)
    {
        if (gMaterial.enableHalfLambert != 0)
        {
            
            // texture�̃��l��0.5f�ȉ��̎���Pixel�����p
            if (textureColor.a <= 0.5f)
            {
            
                discard;
            }
            
            float NdotL = dot(normalize(input.normal), -gDirectionalLight.direction);
            float cos = pow(NdotL * 0.5f + 0.5f, 2.0f);
            output.color.rgb = gMaterial.color.rgb * textureColor.rgb * gDirectionalLight.color.rgb * cos * gDirectionalLight.intensity;
        
            output.color.a = gMaterial.color.a * textureColor.a;
        }
        else
        {
            
            // texture�̃��l��0.5f�ȉ��̎���Pixel�����p
            if (textureColor.a <= 0.5f)
            {
            
                discard;
            }
            
            float cos = saturate(dot(normalize(input.normal), -gDirectionalLight.direction));
        
            output.color.rgb =
            gMaterial.color.rgb * textureColor.rgb * gDirectionalLight.color.rgb * cos * gDirectionalLight.intensity;
        
            output.color.a = gMaterial.color.a * textureColor.a;
        }
    }
    else
    {
        
        // texture�̃��l��0.5f�ȉ��̎���Pixel�����p
        if (textureColor.a <= 0.5f)
        {
            
            discard;
        }
        
        output.color = gMaterial.color * textureColor;
    }
        
    return output;
}