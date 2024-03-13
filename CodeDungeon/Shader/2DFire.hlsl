#ifndef _2DFIRE_
#define _2DFIRE_

#include "ShaderGrobalFunc.hlsli"



cbuffer FireNoiseBuffer : register(b14)
{
    float fFrameTime;
    float3 fScrollSpeeds;
    float3 fScales;
    float padding;
};

cbuffer FireDistortionBuffer : register(b15)
{
    float2 fDistortion1;
    float2 fDistortion2;
    float2 fDistortion3;
    float fDistortionScale;
    float fDistortionBias;
}



struct VS_IN
{
    float3 vPosition : POSITION;
    float2 vTexCoord : TEXCOORD;
};

struct VS_OUT
{
    float4 vPosition : SV_POSITION0;
    float2 vTexCoord : TEXCOORD0;
    float2 vTexCoords1 : TEXCOORD1;
    float2 vTexCoords2 : TEXCOORD2;
    float2 vTexCoords3 : TEXCOORD3;
};


VS_OUT VS_Main(VS_IN In)
{
    VS_OUT Out = (VS_OUT) 0.f;
 
    In.vPosition = In.vPosition * 50;
    Out.vPosition = Compute_FinalMatrix(In.vPosition);
    
    Out.vTexCoord = In.vTexCoord;
    

    // ù ��° ������ �� ���� ��ũ�Ѹ� �ӵ� ���� ����Ͽ� ù ��° ������ �ؽ�ó�� ���� �ؽ�ó ��ǥ�� ����մϴ�.
    Out.vTexCoords1 = (In.vTexCoord * fScales.x);
    Out.vTexCoords1.y = Out.vTexCoords1.y + (fFrameTime * fScrollSpeeds.x);

    // �� ��° ������ �� ���� ��ũ�Ѹ� �ӵ� ���� ����Ͽ� �� ��° ������ �ؽ�ó�� �ؽ�ó ��ǥ�� ����մϴ�.
    Out.vTexCoords2 = (In.vTexCoord * fScales.y);
    Out.vTexCoords2.y = Out.vTexCoords2.y + (fFrameTime * fScrollSpeeds.y);

    // �� ��° ������ �� ���� ��ũ�Ѹ� �ӵ� ���� ����Ͽ� �� ��° ������ �ؽ�ó�� �ؽ�ó ��ǥ�� ����մϴ�.
    Out.vTexCoords3 = (In.vTexCoord * fScales.z);
    Out.vTexCoords3.y = Out.vTexCoords3.y + (fFrameTime * fScrollSpeeds.z);


    return Out;
}


struct PS_OUT
{
    float4 vColor : SV_TARGET0;
};


PS_OUT PS_Main(VS_OUT In)
{
    PS_OUT Out = (PS_OUT)0;

    float4 noise1;
    float4 noise2;
    float4 noise3;
    float4 finalNoise;
    float perturb;
    float2 noiseCoords;
    float4 fireColor;
    float4 alphaColor;

    // 3 ���� ���� �ٸ� �ؽ�ó ��ǥ�� ����Ͽ� ������ ������ �ؽ�ó�� ���ø��Ͽ� 3 ���� �ٸ� ������ �������� ����ϴ�.
    noise1 = g_Texture1.Sample(g_Sampler_Normal, In.vTexCoords1);
    noise2 = g_Texture1.Sample(g_Sampler_Normal, In.vTexCoords2);
    noise3 = g_Texture1.Sample(g_Sampler_Normal, In.vTexCoords3);

    // (0, 1) �������� (-1, +1) ������ ����� �̵��մϴ�.
    noise1 = (noise1 - 0.5f) * 2.0f;
    noise2 = (noise2 - 0.5f) * 2.0f;
    noise3 = (noise3 - 0.5f) * 2.0f;

    // 3 ���� ���� �ٸ� �ְ� x�� y ������ �� ���� ������ x�� y ��ǥ�� �ְ��մϴ�.
    noise1.xy = noise1.xy * fDistortion1.xy;
    noise2.xy = noise2.xy * fDistortion2.xy;
    noise3.xy = noise3.xy * fDistortion3.xy;

    // �� ���� �ְ� �� ������ ����� ��� �ϳ��� ������ ����� �����մϴ�.
    finalNoise = noise1 + noise2 + noise3;

    // �ְ� ������ �� ���̾ ���� ���� �Է� �ؽ�ó Y ��ǥ�� �ְ��մϴ�.
    // ���� ȿ������ �Ҳ��� ���� �Ÿ��� �ؽ�ó�� ���� �����̸� ������ �������ϴ�.
    perturb = ((1.0f - In.vTexCoord.y) * fDistortionScale) + fDistortionBias;

    // ���� ȭ�� ���� �ؽ�ó�� ���ø��ϴ� �� ����� �����ǰ� �ְ� �� �ؽ�ó ���ø� ��ǥ�� ����ϴ�.
    noiseCoords.xy = (finalNoise.xy * perturb) + In.vTexCoord.xy;

    // �����ǰ� �ְ� �� �ؽ�ó ���ø� ��ǥ�� ����Ͽ� ȭ�� �ؽ�ó���� ������ ���ø��մϴ�.
    // ȭ���� ���¸� ���δ� ��� Ŭ���� ���� ���¸� ����Ͽ� ȭ���� ���δ� ���� �����մϴ�.
    fireColor = g_Texture0.Sample(g_Sampler_Clamp, noiseCoords.xy);

    // �����ǰ� �ְ� �� �ؽ�ó ���ø� ��ǥ�� ����Ͽ� ���� �ؽ�ó���� ���� ���� ���ø��մϴ�.
    // �̰��� ���� ������ ���˴ϴ�.
    // ȭ���� ���¸� ���δ� ��� Ŭ���� ���� ���¸� ����Ͽ� ȭ���� ���δ� ���� �����մϴ�.
    alphaColor = g_Texture2.Sample(g_Sampler_Clamp, noiseCoords.xy);

    // ȭ���� ���� ������ �Ҿ����ϰ� �ְ� �� ���� �ؽ�ó ������ �����մϴ�.
    fireColor.a = alphaColor;
 
    Out.vColor = fireColor;
    
    return Out;
}

#endif 