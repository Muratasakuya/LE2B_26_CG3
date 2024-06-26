#include "PhongReflection.hlsli"

/*===========================================================
                PhongReflection VS Shader
===========================================================*/

struct Material
{
    
    float4 color;
    int enableLighting;
    int enablePhongReflection;
    int enableBlinnPhongReflection;
    float4x4 uvTransform;
    float3 specularColor;
    float shininess;
};

struct DirectionalLight
{

    float4 color;
    float3 direction;
    float intensity;
};

struct Camera
{
    
    float3 worldPosition;
};

struct PixelShaderOutput
{
    
    float4 color : SV_TARGET0;
};

ConstantBuffer<Material> gMaterial : register(b0);
ConstantBuffer<DirectionalLight> gDirectionalLight : register(b1);
ConstantBuffer<Camera> gCamera : register(b2);

Texture2D<float4> gTexture : register(t0);
SamplerState gSampler : register(s0);

PixelShaderOutput main(VertexShaderOutput input)
{
    
    float4 transformUV = mul(float4(input.texcoord, 0.0f, 1.0f), gMaterial.uvTransform);
    float4 textureColor = gTexture.Sample(gSampler, transformUV.xy);
    
    PixelShaderOutput output;
    
    if (gMaterial.enableLighting == 1)
    {
        if (gMaterial.enablePhongReflection == 1)
        {
            
            /*=================================================================================================*/
            ///
            ///                                    PhongReflection
            ///
            /*=================================================================================================*/
            
            // texture�̃��l��0.5f�ȉ��̎���Pixel�����p
            if (textureColor.a <= 0.5f)
            {
            
                discard;
            }
            
            // Camera�����Z�o
            float3 toEye = normalize(gCamera.worldPosition - input.worldPosition);
            
            // ���ˌ��̔��˃x�N�g���̌v�Z
            float3 reflectLight = reflect(gDirectionalLight.direction, normalize(input.normal));
            
            float RdotE = dot(reflectLight, toEye);
            float specularPow = pow(saturate(RdotE), gMaterial.shininess);
            
            float NdotL = dot(normalize(input.normal), -gDirectionalLight.direction);
            float cos = pow(NdotL * 0.5f + 0.5f, 2.0f);
            
            // �g�U����
            float3 diffuse =
            gMaterial.color.rgb * textureColor.rgb * gDirectionalLight.color.rgb * cos * gDirectionalLight.intensity;
            
            // ���ʔ���
            float3 specular =
            gDirectionalLight.color.rgb * gDirectionalLight.intensity * specularPow * gMaterial.specularColor;
            
            // �g�U���� + ���ʔ���
            output.color.rgb = diffuse + specular;
            
            // ��
            output.color.a = gMaterial.color.a * textureColor.a;
        }
        else if (gMaterial.enableBlinnPhongReflection == 1)
        {
             /*=================================================================================================*/
            ///
            ///                                 BlinnPhongReflection
            ///
            /*==================================================================================================*/
            
            // texture�̃��l��0.5f�ȉ��̎���Pixel�����p
            if (textureColor.a <= 0.5f)
            {
            
                discard;
            }
            
            // Camera�����Z�o
            float3 toEye = normalize(gCamera.worldPosition - input.worldPosition);
            
            // ���ˌ��̔��˃x�N�g���̌v�Z
            float3 reflectLight = reflect(gDirectionalLight.direction, normalize(input.normal));
            
            float3 halfVector = normalize(-gDirectionalLight.direction + toEye);
            float NDotH = dot(normalize(input.normal), halfVector);
            float specularPow = pow(saturate(NDotH), gMaterial.shininess);
            
            float NdotL = dot(normalize(input.normal), -gDirectionalLight.direction);
            float cos = pow(NdotL * 0.5f + 0.5f, 2.0f);
            
            // �g�U����
            float3 diffuse =
            gMaterial.color.rgb * textureColor.rgb * gDirectionalLight.color.rgb * cos * gDirectionalLight.intensity;
            
            // ���ʔ���
            float3 specular =
            gDirectionalLight.color.rgb * gDirectionalLight.intensity * specularPow * gMaterial.specularColor;
            
            // �g�U���� + ���ʔ���
            output.color.rgb = diffuse + specular;
            
            // ��
            output.color.a = gMaterial.color.a * textureColor.a;
        }
    }
    /*=================================================================================================*/
    ///
    ///                                    Lighting����
    ///
   /*=================================================================================================*/
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