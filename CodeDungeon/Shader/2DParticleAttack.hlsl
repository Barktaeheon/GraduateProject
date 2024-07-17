#ifndef _2DPARTICLE_
#define _2DPARTICLE_

#include "ShaderGrobalFunc.hlsli"


struct GROBALPARTICLEINFO
{
    int iMaxCount;
    int iAddCount;
    float fDeltaTime;
    float fAccTime;
    // ==============
  
    float fMinLifeTime;
    float fMaxLifeTime;
    float fMinSpeed;
    float fMaxSpeed;
    // ===============
   
    float fStartScaleParticle;
    
    float fEndScaleParticle;
    float fParticleThickness;
    int fParticleKind;
    //===============
    float3 fParticleDirection;
    float fAnimSizeX;
    
     //================
    float fAnimSizeY;
    float3 fParticlePosition;
    //=================       
    float fNextAnimTime;
    float3 fPadding;
};


struct PARTICLE
{
    float3 vWorldPos;
    float fCurTime;
    // ===============
    float3 vWorldDir;
    float fLifeTime;
    // ==============
    int iAlive;
    float2 vAnimUV;
    float fTransparency; //���� padding
};

// All Particle

cbuffer ALLPARTICLEBUFFER : register(b13)
{
    GROBALPARTICLEINFO g_GrobalParticleInfo;
};

StructuredBuffer<PARTICLE> g_ParticleData : register(t14);

// VS_MAIN
// g_float_0    : Start Scale
// g_float_1    : End Scale
// g_tex_0      : Particle Texture

struct VS_IN
{
    float3 vPosition : POSITION;
    float3 vNormal : NORMAL;
    float3 vTagent : TANGENT;
    float2 vTexUV : TEXCOORD;
    uint iInstanceID : SV_InstanceID;
};

struct VS_OUT
{
    float4 vViewPos : POSITION;
    float2 vTexUV : TEXCOORD0;
    float4 vWorldPos : TEXCOORD1;
    uint iInstanceID : ID;
};


VS_OUT VS_Main(VS_IN In)
{
    VS_OUT Out = (VS_OUT) 0.f;

    Out.vViewPos = mul(float4(In.vPosition, 1.f), g_WorldMatrix);
    Out.vViewPos += float4(g_ParticleData[In.iInstanceID].vWorldPos, 0.f);
    // ��ǻƮ ���̴����� RWStructuredBuffer�� ������� ���� ��ƼŬ numthreads 1024�� �� �츮�� 
    // maxParticle�� 1000�� �Ǵ� 500�� �� ���� ��ġ��ŭ �ش� ���ۿ� ������ �����Ͽ����Ƿ�
    // �ش� ������ instanceID�� �޾ƿ� �о� �����.
    
    Out.vViewPos = mul(Out.vViewPos, GetViewProjInfo().mViewMatrix);
    Out.vTexUV = In.vTexUV;
    Out.iInstanceID = In.iInstanceID;
    return Out;
}

struct GS_OUT
{
    float4 vPosition : SV_Position;
    float2 vTexUV : TEXCOORD0;
    float4 vWorldPos : TEXCOORD1;
    uint iInstanceID : SV_InstanceID;
    float2 vScreenTex : TEXCOORD2; // ȭ�� ��ǥ �߰�

};


[maxvertexcount(6)]
void GS_Main(point VS_OUT input[1], inout TriangleStream<GS_OUT> outputStream)
{
    GS_OUT output[4] =
    {
        (GS_OUT) 0.f, (GS_OUT) 0.f, (GS_OUT) 0.f, (GS_OUT) 0.f
    };

    VS_OUT Vertex = input[0];
    uint id = (uint) Vertex.iInstanceID;
    if (0 == g_ParticleData[id].iAlive)
        return;

    float ratio = g_ParticleData[id].fCurTime / g_ParticleData[id].fLifeTime;
    float scale = ((g_GrobalParticleInfo.fEndScaleParticle - g_GrobalParticleInfo.fStartScaleParticle) * ratio +
    g_GrobalParticleInfo.fStartScaleParticle) / 2.f;


    float4 positions[4] =
    {
        float4(-scale, scale, 0.f, 0.f),
    float4(scale, scale, 0.f, 0.f),
    float4(scale, -scale, 0.f, 0.f),
    float4(-scale, -scale, 0.f, 0.f)
    };

    float2 texCoords[4] =
    {
        float2(0.f, 0.f),
        float2(1.f, 0.f),
        float2(1.f, 1.f),
        float2(0.f, 1.f)
    };

    for (int i = 0; i < 4; ++i)
    {
        float x = positions[i].x;
        float y = positions[i].y;
 

        output[i].vPosition = Vertex.vViewPos + positions[i];
        output[i].vWorldPos = Vertex.vWorldPos + positions[i];

        // Projection Space
        VIEWPROJINFO stViewProjInfo = GetViewProjInfo();
        output[i].vPosition = mul(output[i].vPosition, stViewProjInfo.mProjMatrix);

        output[i].vTexUV = texCoords[i];
        output[i].iInstanceID = id;
        output[i].vScreenTex = output[i].vPosition.xy / output[i].vPosition.w;
    }
    // �ﰢ�� �� �� ����
    outputStream.Append(output[0]);
    outputStream.Append(output[1]);
    outputStream.Append(output[2]);
    outputStream.RestartStrip();

    outputStream.Append(output[0]);
    outputStream.Append(output[2]);
    outputStream.Append(output[3]);
    outputStream.RestartStrip();
}

struct PS_OUT
{
    float4 vColor : SV_TARGET0;
    float4 vSpecular : SV_TARGET1;
    float4 vNormal : SV_TARGET2;
    float4 vDepth : SV_TARGET3;
    float4 vPosition : SV_Target4;
    float4 vGlow : SV_Target5;
};


PS_OUT PS_Main(GS_OUT In) 
{
    PS_OUT Out = (PS_OUT) 0;
    float2 ScreenTex;
    ScreenTex.x = (In.vScreenTex.x) * 0.5 + 0.5f;
    ScreenTex.y = (In.vScreenTex.y) * -0.5 + 0.5f;

    vector vDepthDesc = g_Texture1.Sample(g_Sampler_Normal, ScreenTex);

    float fViewZ = vDepthDesc.x * GetViewProjInfo().fCamFar;

// ���� ���� ���
    float depthDifference = abs(fViewZ - In.vPosition.w);

// ���� ���̿� ����Ͽ� �ε巴�� ���� ���� ����
    float fDistance = smoothstep(0.0, 1.0, depthDifference * 0.05); // 0.0�� 0.1�� �ʿ�� ����

// �ؽ�ó ���ø�
    Out.vColor = g_Texture0.Sample(g_Sampler_Normal, In.vTexUV);
  //  Out.vColor.rgb *= 1.25;
// �ּ� ���� ���� �����Ͽ� ��ü������ ��ƼŬ�� ���̵��� ��
  //  float minAlpha = 0.2; // �ּ� ���� ��, �ʿ�� ����
  //  Out.vColor.a *= fDistance;

    
    
    float particleTransparency = g_ParticleData[In.iInstanceID].fCurTime / g_ParticleData[In.iInstanceID].fLifeTime;
    Out.vColor.a *= particleTransparency;
    //if (Out.vColor.a < 0.1)
    //    discard;
    Out.vGlow = Out.vColor;
  
        return Out;
}

#endif 