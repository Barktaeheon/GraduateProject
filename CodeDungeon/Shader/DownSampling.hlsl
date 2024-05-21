#ifndef _DOWN_SAMPLE_HLSL_
#define _DOWN_SAMPLE_HLSL_

#include "ShaderGrobalFunc.hlsli"

struct VS_IN
{
    float3 vPosition : POSITION;
    float2 vTexUV : TEXCOORD0;
};

struct VS_OUT
{
    float4 vPosition : SV_POSITION;
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
    float4 vPosition : SV_POSITION;
    float2 vTexUV : TEXCOORD0;
};

struct PS_OUT
{
    float4 vColor : SV_Target0;
};

const float2 inverseTextureSize = float2(1.0 / 1280.0, 1.0 / 1080.0); // �Է� �ؽ�ó�� �ػ󵵿� ���� ����

PS_OUT PS_Main(PS_In In)
{
    PS_OUT Out = (PS_OUT) 0;

    float4 downScaled = float4(0, 0, 0, 0);

    // �� ���� �ȼ��� �ϳ��� �ȼ��� ��Ĩ�ϴ�.
    for (int i = 0; i < 2; ++i)
    {
        for (int j = 0; j < 2; ++j)
        {
            // �ٿ���ø��� ��ǥ ���
            float2 downSampledTexCoord = In.vTexUV * 2.0f + float2(i, j);

            // ���ø��Ͽ� �ȼ� ���� �ջ�
            downScaled += g_Texture0.Sample(g_Sampler_Normal, downSampledTexCoord);
        }
    }

    // �� ���� �ȼ� ���� ����Ͽ� �ٿ���ø��� �ȼ� ���� ���
    downScaled /= 4.0f;

    // ���� �ٿ���ø��� �ȼ� ���� ��¿� �����մϴ�.
    Out.vColor = downScaled;

    return Out;
}

#endif // _DOWN_SAMPLE_HLSL_
