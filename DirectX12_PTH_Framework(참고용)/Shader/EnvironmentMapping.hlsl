#ifndef _ENVIRONMENTMAPPING_HLSL_
#define _ENVIRONMENTMAPPING_HLSL_

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

VS_OUT VS_Main(VS_IN In)
{
    VS_OUT Out = (VS_OUT) 0;

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
    float4 vDiffuse : SV_TARGET0;
    float4 vNormal : SV_TARGET1;
    float4 vDepth : SV_TARGET2;
    float4 vPosition : SV_Target3;
};

PS_OUT PS_Main(PS_In Input)
{
    PS_OUT Out = (PS_OUT) 0;

    Out.vDiffuse = g_Texture0.Sample(g_Sampler_Normal, Input.vTexUV);
    if (Out.vDiffuse.a == 0)
        discard;
    
    Out.vDiffuse += g_Texture1.Sample(g_Sampler_Normal, Input.vTexUV);
    Out.vNormal = (float4) 1.f;
    Out.vDepth = float4(0.f, 0.f, 0.f, 0.f);
    return Out;
}

#endif // _DEBUGDEFAULTOBJECT_HLSL_