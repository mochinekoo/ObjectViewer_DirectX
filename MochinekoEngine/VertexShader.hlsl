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

struct VSInput
{
    float3 position : POSITION;
    float3 normal : NORMAL;
    float4 color : COLOR;
    float2 uv : TEXCOORD;
};

struct VSOutput
{
    float4 position : SV_POSITION;
    float3 normal : NORMAL;
    float4 color : COLOR;
    float2 uv : TEXCOORD;
};


VSOutput main(VSInput input) {
    VSOutput output;
    output.position = mul(float4(input.position, 1.0f), wvpMatrix);
    output.normal = input.normal;
    output.color = diffuse;
    output.uv = input.uv;

    return output;
}