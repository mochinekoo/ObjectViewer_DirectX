Texture2D texture0 : register(t0);
SamplerState sampler0 : register(s0);

cbuffer ConstantBuffer : register(b0) {
    matrix wvpMatrix;
    float4 diffuse;
    float4 ambient;
    float4 specular;
    float3 emission;
    float shininess;
    int hasTexture;
    float3 lightDirection;
};

struct PSInput
{
    float4 position : SV_POSITION;
    float3 normal : NORMAL;
    float4 color : COLOR;
    float2 uv : TEXCOORD0;
};

float4 main(PSInput input) : SV_TARGET {
    float4 color = { 1.0f, 0.0f, 0.0f, 1.0f };
    
    if (hasTexture == 1) {
        color = texture0.Sample(sampler0, input.uv);
    }
    else {
        color = diffuse;
    }
    
    return color * input.color;
}