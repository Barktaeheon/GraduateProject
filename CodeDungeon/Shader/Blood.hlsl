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


VS_OUT VS_Main(VS_IN In) //�̸� VS_Main PS_Main ����.
{
	VS_OUT Out = (VS_OUT)0;

    Out.vPosition = Compute_FinalMatrix(In.vPosition);
    //Out.vPosition = float4(In.vPosition, 1.f);    
    //Out.vPosition = mul(Out.vPosition, g_WorldMatrix);
    //Out.vPosition = mul(Out.vPosition, g_ViewProjInfoArr[2].mViewMatrix);
    //Out.vPosition = mul(Out.vPosition, g_ViewProjInfoArr[2].mProjMatrix);
    
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

    Out.vColor = g_Texture0.Sample(g_Sampler_Normal, In.vTexUV);
   
    Out.vSpecular = float4(Out.vColor.rgb, 0.5f);
    
    return Out;
}

#endif // _TRAIL_HLSL_