#ifndef _SHADOW_HLSL_
#define _SHADOW_HLSL_

#include "ShaderGrobalFunc.hlsli"
#include "LightShaderParam.hlsli"
struct BONEMATRIX
{
    float4x4 BoneMatrix[512];
};

//cbuffer MODELDATAPARAM : register(b10)
//{
//    int g_iMaterialIndex;
//    float3 PaddingValue;
//};

cbuffer ANIMATIONPARAM : register(b11)
{
    bool g_isOutLineExist = false;
    bool g_isNomralExist = true;
    bool g_isAnimPadding;
    bool g_isAnimPadding2;
};

cbuffer BONEMATRIXPARAM : register(b12)
{
    BONEMATRIX g_BoneMatrix;
};

cbuffer PREVBONEMATRIXPARAM : register(b13)
{
    BONEMATRIX g_PrevBoneMatrix;
};

struct VS_IN
{
    float3 vPosition : POSITION;
    float3 vNormal : NORMAL;
    float2 vTexUV0 : TEXCOORD0;
    float2 vTexUV1 : TEXCOORD1;
    float2 vTexUV2 : TEXCOORD2;
    float2 vTexUV3 : TEXCOORD3;
    float2 vTexUV4 : TEXCOORD4;
    float3 vTangent : TANGENT;
    uint4 vBlendIndex : BLENDINDEX; /* ���� ������ ���⤷�� �ִ� ���� �ε���(�޽�Ŀ���̳ʿ� ������ �ִ� ���� ��), �ִ� 4��. */
    float4 vBlendWeight : BLENDWEIGHT; /* ������ ���ǰ� �ִ� �ε�����° ���� �������(�ִ� 1) . */

};

struct VS_OUT
{
    float4 vPosition : SV_POSITION;
    
};


VS_OUT VS_Main(VS_IN In)
{
    VS_OUT Out = (VS_OUT) 0.f;
    
    float fWeightW = 1.f - (In.vBlendWeight.x + In.vBlendWeight.y + In.vBlendWeight.z);
    
    
    matrix BoneMatrix = (g_BoneMatrix.BoneMatrix[In.vBlendIndex.x] * In.vBlendWeight.x) +
		g_BoneMatrix.BoneMatrix[In.vBlendIndex.y] * In.vBlendWeight.y +
		g_BoneMatrix.BoneMatrix[In.vBlendIndex.z] * In.vBlendWeight.z +
		g_BoneMatrix.BoneMatrix[In.vBlendIndex.w] * fWeightW;
    
    
    
    
    vector vNormal = mul(float4(In.vNormal, 0.f), BoneMatrix);
    vNormal = normalize(mul(vNormal, g_WorldMatrix));
    
    
    float gSize = 1.5;
    In.vPosition.xyz += normalize(vNormal) * gSize;
    
    vector vPosition = mul(float4(In.vPosition, 1.f), BoneMatrix);
    Out.vPosition = Compute_FinalMatrix(vPosition);
    
   
  

    return Out;
    
    
    //VS_OUT Out = (VS_OUT) 0.f;
    
    //float fWeightW = 1.f - (In.vBlendWeight.x + In.vBlendWeight.y + In.vBlendWeight.z);
    
    //matrix BoneMatrix = (g_BoneMatrix.BoneMatrix[In.vBlendIndex.x] * In.vBlendWeight.x) +
    //                    (g_BoneMatrix.BoneMatrix[In.vBlendIndex.y] * In.vBlendWeight.y) +
    //                    (g_BoneMatrix.BoneMatrix[In.vBlendIndex.z] * In.vBlendWeight.z) +
    //                    (g_BoneMatrix.BoneMatrix[In.vBlendIndex.w] * fWeightW);
    
    //// Scale matrix ����
    //float gScale = 1.01; // ���� ��� 1.1�� �����Ͽ� 10% Ȯ��
    //matrix scaleMatrix =
    //{
    //    gScale, 0.0f, 0.0f, 0.0f,
    //                       0.0f, gScale, 0.0f, 0.0f,
    //                       0.0f, 0.0f, gScale, 0.0f,
    //                       0.0f, 0.0f, 0.0f, 1.0f
    //};

    //// BoneMatrix�� scaleMatrix ���ϱ�
    //BoneMatrix = mul(BoneMatrix, scaleMatrix);
    
    //vector vNormal = mul(float4(In.vNormal, 0.f), BoneMatrix);
    //vNormal = normalize(mul(vNormal, g_WorldMatrix));
    
    //vector vPosition = mul(float4(In.vPosition, 1.f), BoneMatrix);
    //Out.vPosition = Compute_FinalMatrix(vPosition);
    
    //return Out;
}

struct PS_OUT
{
    float4 vColor : SV_TARGET0;
};


PS_OUT PS_Main(VS_OUT In) 
{
    PS_OUT Out = (PS_OUT) 0;
    Out.vColor.xyz = float3(255, 0, 0);
    Out.vColor.w = In.vPosition.z / In.vPosition.w;
    return Out;

}



#endif // _SHADOW_HLSL_