#ifndef _DEFAULTUI_HLSL_
#define _DEFAULTUI_HLSL_

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

cbuffer LoadingTimerBuffer : register(b13)
{
    float _ifPressed;
    float _durationTime;
    float2 _padding;
};


VS_OUT VS_Main(VS_IN In) //�̸� VS_Main PS_Main ����.
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
    float4 vColor : SV_TARGET0;
};

PS_OUT PS_Main(PS_In In)
{
    PS_OUT Out = (PS_OUT) 0;

    // �ؽ�ó ������ ���ø�
    float4 texColor = g_Texture0.Sample(g_Sampler_Normal, In.vTexUV);

    // ���� �� ���
    float alpha = 1.0f;

    if (_ifPressed)
    {
        // 1�� �ֱ�� �����̵��� ����
        float timeMod = fmod(_durationTime, 2.0f);
        if (timeMod > 1.0f)
        {
            alpha = 0.0f; // 1�� ���Ŀ��� �����
        }
    }

    // �ؽ�ó ���� ���� �� ����
    texColor.a *= alpha;

    Out.vColor = texColor;

    return Out;
}

#endif // _RECT_H_DEFAULTUI_HLSL_LSL_