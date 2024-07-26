#ifndef _ANIMMODEL_HLSL_
#define _ANIMMODEL_HLSL_

#include "ShaderGrobalFunc.hlsli"


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
    bool g_isObjectDissolve = false;
    bool g_isObjectMotionblur = false;
    
    float g_fDissolveTimer = 0;
    float3 g_remains;
};

cbuffer BONEMATRIXPARAM : register(b12)
{
    BONEMATRIX g_BoneMatrix;
};

cbuffer PREVBONEMATRIXPARAM : register(b13)
{
    BONEMATRIX g_PrevBoneMatrix;
};

cbuffer DrawRim : register(b14)
{
    int g_DrawRim;
    float3 RimColor;
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
	float4 vNormal : NORMAL;
    float4 vTangent : TANGENT;
    float4 vBinormal : BINORMAL;
    float2 vTexUV0 : TEXCOORD0;
    float2 vTexUV1 : TEXCOORD1;
    float2 vTexUV2 : TEXCOORD2;
    float2 vTexUV3 : TEXCOORD3;
    float2 vTexUV4 : TEXCOORD4;
	float4 vWorldPos : TEXCOORD5;
	float4 vProjPos : TEXCOORD6;
	float4 vVelocity : TEXCOORD7;
    float3 vViewDir : TEXCOORD8;
};

VS_OUT VS_Main(VS_IN In)
{
	VS_OUT Out = (VS_OUT) 0;

    float fWeightW = 1.f - (In.vBlendWeight.x + In.vBlendWeight.y + In.vBlendWeight.z);
    
    matrix BoneMatrix = (g_BoneMatrix.BoneMatrix[In.vBlendIndex.x] * In.vBlendWeight.x) +
		g_BoneMatrix.BoneMatrix[In.vBlendIndex.y] * In.vBlendWeight.y +
		g_BoneMatrix.BoneMatrix[In.vBlendIndex.z] * In.vBlendWeight.z +
		g_BoneMatrix.BoneMatrix[In.vBlendIndex.w] * fWeightW;
    

    vector vPosition = mul(float4(In.vPosition, 1.f), BoneMatrix);
   // vector vPosition = (float4(In.vPosition, 1.f));
    
    vPosition = Compute_FinalMatrix(vPosition);
   

    vector vNormal = mul(float4(In.vNormal, 0.f), BoneMatrix);
    vector vTangent = mul(float4(In.vTangent, 0.f), BoneMatrix);

    Out.vPosition = vPosition;
    Out.vNormal = normalize(mul(vNormal, g_WorldMatrix));
    Out.vTangent = normalize(mul(vTangent, g_WorldMatrix));
    Out.vBinormal = normalize(vector(cross(Out.vNormal.xyz, Out.vTangent.xyz), 0.f));
    Out.vTexUV0 = In.vTexUV0;
    Out.vTexUV1 = In.vTexUV1;
    Out.vTexUV2 = In.vTexUV2;
    Out.vTexUV3 = In.vTexUV3;
    Out.vTexUV4 = In.vTexUV4;
    Out.vWorldPos = mul(float4(In.vPosition, 1.f), g_WorldMatrix);
    Out.vProjPos = Out.vPosition;
    Out.vViewDir = normalize(g_ViewProjInfoArr[g_CamID].vCamPosition - Out.vWorldPos.xyz); // Compute the view direction

   
    
    if (true == g_isObjectMotionBlur || 0 != g_TransformMotionBlurOn)
    {
        matrix PrevBoneMatrix = (g_PrevBoneMatrix.BoneMatrix[In.vBlendIndex.x] * In.vBlendWeight.x) +
                        (g_PrevBoneMatrix.BoneMatrix[In.vBlendIndex.y] * In.vBlendWeight.y) +
                        (g_PrevBoneMatrix.BoneMatrix[In.vBlendIndex.z] * In.vBlendWeight.z) +
                        (g_PrevBoneMatrix.BoneMatrix[In.vBlendIndex.w] * fWeightW);
        
        vector vPrevPosition = mul(float4(In.vPosition, 1.f), PrevBoneMatrix);
        
        VIEWPROJINFO tMainViewProj = GetViewProjInfo();
        
        vPrevPosition = mul(vPrevPosition, g_PrevWorldMatrix);
        vPrevPosition = mul(vPrevPosition, tMainViewProj.mPrevViewMatirx);
        vPrevPosition = mul(vPrevPosition, tMainViewProj.mProjMatrix);
        
        float3 dir = Out.vPosition.xyz - vPrevPosition.xyz;
        float4 NORMAL = mul(Out.vNormal, tMainViewProj.mPrevViewMatirx);
        float a = dot(normalize(dir), normalize(NORMAL.xyz));
     //   if (a < 0.f)
      //      Out.vPosition = vPrevPosition;
        
        float2 velocity = ((Out.vPosition.xy / Out.vPosition.w - (vPrevPosition.xy / vPrevPosition.w)) * 0.5f);
        velocity.y *= -1.f;
        Out.vVelocity.xy = velocity;
        Out.vVelocity.zw = Out.vPosition.zw;
    }
    
	return Out;
}

struct PS_IN
{
    float4 vPosition : SV_POSITION;
    float4 vNormal : NORMAL;
    float4 vTangent : TANGENT;
    float4 vBinormal : BINORMAL;
    float2 vTexUV0 : TEXCOORD0;
    float2 vTexUV1 : TEXCOORD1;
    float2 vTexUV2 : TEXCOORD2;
    float2 vTexUV3 : TEXCOORD3;
    float2 vTexUV4 : TEXCOORD4;
    float4 vWorldPos : TEXCOORD5;
    float4 vProjPos : TEXCOORD6;
    float4 vVelocity : TEXCOORD7;
    float3 vViewDir : TEXCOORD8;
};

struct PS_OUT
{
    float4 vDiffuse : SV_TARGET0;
    float4 vSpecular : SV_TARGET1;
    float4 vNormal : SV_TARGET2;
    float4 vDepth : SV_TARGET3;
    float4 vPosition : SV_Target4;
    float4 vGlow : SV_Target5;
    float4 vVelocity : SV_Target6;
};

/* �ȼ��� ���� �����Ѵ�. */
// float4 PS_MAIN(PS_IN In) : SV_TARGET0
PS_OUT PS_Main(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    VIEWPROJINFO tMainViewProj = GetViewProjInfo();
    // Out Color    
   
    if (true == g_isObjectDissolve)
    {
        vector vDissorveColor = g_Texture2.Sample(g_Sampler_Normal, In.vTexUV0);
   //     vector vShow_DisorveColor = g_Texture3.Sample(g_Sampler_Normal, In.vTexUV0);
        float fColorLength = vDissorveColor.r + vDissorveColor.g + vDissorveColor.b;
        if (fColorLength <= g_fDissolveTimer)
        {
            if (fColorLength - 0.2f <= g_fDissolveTimer)
            {
                discard;
            }
            Out.vDiffuse += float4(1.f, -0.22f, -0.22f, 0.f);
        }
    }
    else
    {
        Out.vDiffuse = g_Texture0.Sample(g_Sampler_Normal, In.vTexUV0);
 
        if (Out.vDiffuse.a <= 0.05)
            discard;
        
        if (g_DrawRim)
        {
            float rimFactor = 1.0 - saturate(dot(normalize(In.vViewDir), normalize(In.vNormal.xyz)));
            rimFactor = pow(rimFactor, 3.5) * 3;
            float4 rimLight = rimFactor * float4(RimColor, 1);

    // Combine rim lighting with the final color
            Out.vDiffuse.rgb += rimLight.rgb;
        }
    }
   
    Out.vNormal = float4(float3(In.vNormal.xyz) * 0.5f + 0.5f, 1.f);
  
    Out.vDepth = float4(In.vProjPos.w / tMainViewProj.fCamFar, In.vProjPos.z / In.vProjPos.w, 1.f, In.vPosition.w);
    Out.vPosition = In.vWorldPos;
    
    if (true == g_isObjectMotionBlur || 0 != g_TransformMotionBlurOn)
    {
        Out.vVelocity = In.vVelocity;
        Out.vVelocity.z = 1.f;
        Out.vVelocity.w = In.vVelocity.z / In.vVelocity.w;
    }
        
    return Out;
}



#endif // _ANIMMODEL_HLSL_