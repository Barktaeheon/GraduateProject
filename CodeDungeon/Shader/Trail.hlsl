#ifndef _TRAIL_HLSL_
#define _TRAIL_HLSL_

#include "ShaderGrobalFunc.hlsli"


struct VS_IN
{
	float3 vPosition : POSITION;
	float2 vTexUV : TEXCOORD0;
};

struct VS_OUT
{
	float4 vPosition : SV_POSITION0;
	float2 vTexUV : TEXCOORD0;
};

cbuffer HasNormalBuffer : register(b14)
{
    float4 gf4Color;
};

VS_OUT VS_Main(VS_IN In) //�̸� VS_Main PS_Main ����.
{
	VS_OUT Out = (VS_OUT)0;

	Out.vPosition = Compute_FinalMatrix(In.vPosition);
	Out.vTexUV = In.vTexUV;

	return Out;
}

struct PS_In
{
    float4 vPosition : SV_POSITION0;
    float2 vTexUV : TEXCOORD0;
};

struct PS_OUT
{
    float4 vColor : SV_TARGET0;
    float4 vSpecular : SV_TARGET1;
    float4 vNormal : SV_TARGET2;
    float4 vDepth : SV_TARGET3;
    float4 vPosition : SV_Target4;
    float4 vGlow : SV_Target5;
};

PS_OUT PS_Main(PS_In In)
{
    PS_OUT Out = (PS_OUT) 0;

     
    //float4 baseColor = g_Texture3.Sample(g_Sampler_Normal, In.vTexUV);
    //float4 noise = g_Texture0.Sample(g_Sampler_Normal, In.vTexUV * 5.0f); // ������ �ؽ�ó�� ������ ����
    //baseColor.rgb += noise.rgb * 0.2f; // ������ ���� ����

    //float2 distortion = g_Texture1.Sample(g_Sampler_Normal, In.vTexUV).rg * 0.1f; // �ְ� ���� ����
    //float4 distortedColor = g_Texture0.Sample(g_Sampler_Normal, In.vTexUV + distortion);

   
    //float4 finalColor = lerp(baseColor, distortedColor, 0.5f);
    //Out.vColor = finalColor;
    //Out.vGlow = float4(finalColor.xyz, 0.5);
   
    float2 uv = In.vTexUV.xy;

// �⺻ Ʈ���� �ؽ�ó ���ø�
    float4 BaseColor = g_Texture2.Sample(g_Sampler_Normal, uv);

// ������ �ؽ�ó ���ø�
    float4 NoiseColor = g_Texture0.Sample(g_Sampler_Normal, uv * 5.0f - float2(0.0f, 1.0f)); // ���⼭ ������ Ÿ���� ����Ͽ� y�� �̵��� ����

// Emissive ���� ���
    float EmissiveFactor = 2.0f; // ���� ��� ������ ���
    float ColorUV = min(1.0f, NoiseColor.r * 2.0f);

// Emissive ���� ���
    float4 EmissiveColor;
    EmissiveColor.r = lerp(0.0f, 1.0f, ColorUV);
    EmissiveColor.g = lerp(0.0f, 1.0f, ColorUV * 0.5f);
    EmissiveColor.b = lerp(0.0f, 1.0f, ColorUV * 0.25f);
    EmissiveColor.a = 1.0f;

    EmissiveColor.rgb *= EmissiveFactor * NoiseColor.r;

// �ְ� �ؽ�ó ���ø� �� UV �ְ�
    float2 distortion = g_Texture1.Sample(g_Sampler_Normal, uv).rg * 0.1f;
    float4 DistortedColor = g_Texture2.Sample(g_Sampler_Normal, uv + distortion);

// ���� ���� ���
    float4 finalColor = lerp(BaseColor, DistortedColor, 0.5f);
    finalColor.rgb += EmissiveColor.rgb;

    Out.vColor = finalColor;
    Out.vGlow = float4(finalColor.rgb, 0.5f);

    return Out;
    
   
    
    return Out;
}

#endif // _TRAIL_HLSL_