#ifndef _COMPUTE2DPARTICLE_
#define _COMPUTE2DPARTICLE_

#include "ComputeFunc.hlsli"

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


struct ComputeParticleType
{
    int fParticleType;
    int fParticleLifeTimeType;
    float2 fPadding;
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

struct COMPUTESHARED
{
    int iAddCount;
    float3 padding;
};

// All Particle

cbuffer ALLPARTICLEBUFFER : register(b13)
{
    GROBALPARTICLEINFO g_GrobalParticleInfo;
};

cbuffer PARTICLETYPEBUFFER : register(b14)
{
    ComputeParticleType g_ParticleType;
}

RWStructuredBuffer<PARTICLE> g_ParticleWritedata : register(u0);
RWStructuredBuffer<COMPUTESHARED> g_SharedData : register(u1);

float3 g_Gravity = float3(0.0, -9.8, 0.0);


//[numthreads(512, 1, 1)]
//void CS_Main(int3 threadIndex : SV_DispatchThreadID)
//{
//    if (threadIndex.x >= g_GrobalParticleInfo.iMaxCount)
//        return;
    
//    g_SharedData[0].iAddCount = g_GrobalParticleInfo.iAddCount;
//    // ��� �� �� ��ٸ��� ��
//    GroupMemoryBarrierWithGroupSync();

//    if (g_ParticleWritedata[threadIndex.x].iAlive == 0)
//    {
//        while (true)
//        {
//            int remaining = g_SharedData[0].iAddCount;
//            if (remaining <= 0)
//                break;

//            int expected = remaining;
//            int desired = remaining - 1;
//            int originalValue;
//            // Atomic ���� ���� �� ���� �� ���� ����ȴ�. 
//            // 1���ڿ� 2���ڰ� ������ desire�� �ٸ��� OriginValue�� �־��ش�. 
//            InterlockedCompareExchange(g_SharedData[0].iAddCount, expected, desired, originalValue);

//            if (originalValue == expected)
//            {
//                g_ParticleWritedata[threadIndex.x].iAlive = 1;
//                break;
//            }
//        }

//        // ���� �κ� �����ϰ� ���� Noise TexTexture �̿�
//        if (g_ParticleWritedata[threadIndex.x].iAlive == 1)
//        {
//            float x = ((float) threadIndex.x / (float) g_GrobalParticleInfo.iMaxCount) + g_GrobalParticleInfo.fAccTime;

//            float r1 = Rand(float2(x, g_GrobalParticleInfo.fAccTime));
//            float RRr1 = Rand(float2(x, g_GrobalParticleInfo.fDeltaTime));
//            float r2 = Rand(float2(x * g_GrobalParticleInfo.fAccTime, g_GrobalParticleInfo.fAccTime));
//            float r3 = Rand(float2(x * g_GrobalParticleInfo.fAccTime * g_GrobalParticleInfo.fAccTime, g_GrobalParticleInfo.fAccTime * g_GrobalParticleInfo.fAccTime));
         
//            // [0.5~1] -> [0~1]
//            float3 noise =
//            {
//                2 * r1 - 1,
//                2 * r2 - 1,
//                2 * r3 - 1
//            };

//            // [0~1] -> [-1~1]
//            float3 dir;
//            dir.xz = (noise.xz - 0.5f) * 2.f;
//            dir.y = noise.y;
//            float offset = (noise - 0.5f) * 2.f;
  
//            dir.y *= 2;
//            g_ParticleWritedata[threadIndex.x].vWorldPos = g_GrobalParticleInfo.fParticlePosition + (dir.xyz);
//            g_ParticleWritedata[threadIndex.x].vWorldDir = normalize(dir);
           
         

         
//            g_ParticleWritedata[threadIndex.x].fLifeTime = ((g_GrobalParticleInfo.fMaxLifeTime - g_GrobalParticleInfo.fMinLifeTime))
//            + g_GrobalParticleInfo.fMinLifeTime - Rand(float2(((float) threadIndex.x / (float) g_GrobalParticleInfo.iMaxCount) + g_GrobalParticleInfo.fAccTime * g_GrobalParticleInfo.fAccTime * g_GrobalParticleInfo.fAccTime, g_GrobalParticleInfo.fAccTime * g_GrobalParticleInfo.fAccTime) / 4);
          
//            g_ParticleWritedata[threadIndex.x].fTransparency = 1.f;
//            g_ParticleWritedata[threadIndex.x].fCurTime = 0.f;
//        }
//    }
//    else
//    {
//        // ������ ����
//        g_ParticleWritedata[threadIndex.x].fCurTime += g_GrobalParticleInfo.fDeltaTime;
//        if (g_ParticleWritedata[threadIndex.x].fLifeTime < g_ParticleWritedata[threadIndex.x].fCurTime)
//        {
//            g_ParticleWritedata[threadIndex.x].iAlive = 0;
//            return;
//        }

//        float ratio = g_ParticleWritedata[threadIndex.x].fCurTime / g_ParticleWritedata[threadIndex.x].fLifeTime;
//        float speed = (g_GrobalParticleInfo.fMaxSpeed - g_GrobalParticleInfo.fMinSpeed) * ratio + g_GrobalParticleInfo.fMinSpeed;
//        if (ratio < 0.5f)
//        {
//            // ���� �ֱ��� ���� ����: ���� Ƣ�� ȿ��
           
//            g_ParticleWritedata[threadIndex.x].vWorldPos += g_ParticleWritedata[threadIndex.x].vWorldDir * speed * g_GrobalParticleInfo.fDeltaTime;
//        }
//        else
//        {
//            // ���� �ֱ��� ���� ����: �߷��� ������ �޾� �Ʒ��� ������
//            float3 gravityEffect = g_Gravity * 10.0f; // �߷� ȿ���� ���� ��ȭ
     
//            g_ParticleWritedata[threadIndex.x].vWorldPos += gravityEffect;
           
//        }
//        g_ParticleWritedata[threadIndex.x].fTransparency = (1.0f - ratio);
        
//    }
   
//}











[numthreads(512, 1, 1)]
void CS_Main(int3 threadIndex : SV_DispatchThreadID)
{
    if (threadIndex.x >= g_GrobalParticleInfo.iMaxCount)
        return;
    
    g_SharedData[0].iAddCount = g_GrobalParticleInfo.iAddCount;
    GroupMemoryBarrierWithGroupSync();

    if (g_ParticleWritedata[threadIndex.x].iAlive == 0)
    {
        while (true)
        {
            int remaining = g_SharedData[0].iAddCount;
            if (remaining <= 0)
                break;

            int expected = remaining;
            int desired = remaining - 1;
            int originalValue;
            InterlockedCompareExchange(g_SharedData[0].iAddCount, expected, desired, originalValue);

            if (originalValue == expected)
            {
                g_ParticleWritedata[threadIndex.x].iAlive = 1;
                break;
            }
        }

        if (g_ParticleWritedata[threadIndex.x].iAlive == 1)
        {
            float x = ((float)threadIndex.x / (float)g_GrobalParticleInfo.iMaxCount) + g_GrobalParticleInfo.fAccTime;

            float r1 = Rand(float2(x, g_GrobalParticleInfo.fAccTime));
            float r2 = Rand(float2(x * g_GrobalParticleInfo.fAccTime, g_GrobalParticleInfo.fAccTime));
            float r3 = Rand(float2(x * g_GrobalParticleInfo.fAccTime * g_GrobalParticleInfo.fAccTime, g_GrobalParticleInfo.fAccTime * g_GrobalParticleInfo.fAccTime));
         
            float3 noise =
            {
                2 * r1 - 1,
                2 * r2 - 1,
                2 * r3 - 1
            };

            float3 dir;
            dir.x = (noise.x - 0.5f) * 2.f;
            dir.yz = abs(noise.yz) * 2.0f; // Ensure upward movement

            g_ParticleWritedata[threadIndex.x].vWorldPos = g_GrobalParticleInfo.fParticlePosition; // Start at central point
            g_ParticleWritedata[threadIndex.x].vWorldDir = normalize(dir);
           
            g_ParticleWritedata[threadIndex.x].fLifeTime = ((g_GrobalParticleInfo.fMaxLifeTime - g_GrobalParticleInfo.fMinLifeTime))
            + g_GrobalParticleInfo.fMinLifeTime;
            g_ParticleWritedata[threadIndex.x].fTransparency = 1.f;
            g_ParticleWritedata[threadIndex.x].fCurTime = 0.f;
        }
    }
    else
    {
        g_ParticleWritedata[threadIndex.x].fCurTime += g_GrobalParticleInfo.fDeltaTime;
        if (g_ParticleWritedata[threadIndex.x].fLifeTime < g_ParticleWritedata[threadIndex.x].fCurTime)
        {
            g_ParticleWritedata[threadIndex.x].iAlive = 0;
            return;
        }

        float ratio = g_ParticleWritedata[threadIndex.x].fCurTime / g_ParticleWritedata[threadIndex.x].fLifeTime;
        float speed = (g_GrobalParticleInfo.fMaxSpeed - g_GrobalParticleInfo.fMinSpeed) * (1.0 - ratio) + g_GrobalParticleInfo.fMinSpeed;

        // Apply gravity effect
        float3 gravityEffect = float3(0.0, -9.8, 0.0) * g_GrobalParticleInfo.fDeltaTime ; // Amplify gravity

        // Update position
        if (ratio < 0.75)
        {
            g_ParticleWritedata[threadIndex.x].vWorldPos += g_ParticleWritedata[threadIndex.x].vWorldDir * speed * g_GrobalParticleInfo.fDeltaTime;

        }
        else
        {
            g_ParticleWritedata[threadIndex.x].vWorldPos += g_ParticleWritedata[threadIndex.x].vWorldDir * speed * g_GrobalParticleInfo.fDeltaTime + gravityEffect;

        }
       
       // g_ParticleWritedata[threadIndex.x].fTransparency = (1.0f - ratio);
    }
}
























//===============================================================================================


//[numthreads(10, 1, 1)]
//void CS_Main(int3 threadIndex : SV_DispatchThreadID)
//{
//    // ������ �ε����� g_GrobalParticleInfo.iMaxCount���� ũ�ų� ������ �Լ� ����
//    if (threadIndex.x >= g_GrobalParticleInfo.iMaxCount)
//        return;

//    // ù ��° �����忡���� iAddCount�� �ʱ�ȭ
//    if (g_ParticleWritedata[9].iAlive == 1)
//    {
//        g_SharedData[0].iAddCount = 0;
//    }
//    else
//    {
//        if ( g_SharedData[0].iAddCount == 0)
//        {
//            g_SharedData[0].iAddCount = g_GrobalParticleInfo.iAddCount;
//        }
//    }
  
//    GroupMemoryBarrierWithGroupSync();

//    if (g_ParticleWritedata[threadIndex.x].iAlive == 0)
//    {
//        int originalValue;
//        int expected;
//        int desired;

//        while (true)
//        {
//            // ���� ��ƼŬ �߰� ���� ��������
//            expected = g_SharedData[0].iAddCount;

//            // ���� ������ 0 �����̸� �ƹ� �۾��� ���� ����
//            if (expected <= 0)
//                return;

//            desired = expected - 1;

//            // Atomic ���� ����
//            InterlockedCompareExchange(g_SharedData[0].iAddCount, expected, desired, originalValue);

//            if (originalValue == expected)
//            {
//                // ��ƼŬ Ȱ��ȭ
//                g_ParticleWritedata[threadIndex.x].iAlive = 1;
//                break;
//            }
//        }

//        // ���� �κ� �����ϰ� ���� Noise TexTexture �̿�
//        float x = ((float) threadIndex.x / (float) g_GrobalParticleInfo.iMaxCount) + g_GrobalParticleInfo.fAccTime;

//        float r1 = Rand(float2(x, g_GrobalParticleInfo.fAccTime));
//        float RRr1 = Rand(float2(x, g_GrobalParticleInfo.fDeltaTime));
//        float r2 = Rand(float2(x * g_GrobalParticleInfo.fAccTime, g_GrobalParticleInfo.fAccTime));
//        float r3 = Rand(float2(x * g_GrobalParticleInfo.fAccTime * g_GrobalParticleInfo.fAccTime, g_GrobalParticleInfo.fAccTime * g_GrobalParticleInfo.fAccTime));
     
//        // [0.5~1] -> [0~1]
//        float3 noise =
//        {
//            2 * r1 - 1,
//            2 * r2 - 1,
//            2 * r3 - 1
//        };

//        // [0~1] -> [-1~1]
//        float3 dir;
//        dir.xz = (noise.xz - 0.5f) * 2.f;
//        dir.y = noise.y;
//        float offset = (noise - 0.5f) * 2.f;

//        dir.y *= 2;
//        g_ParticleWritedata[threadIndex.x].vWorldPos = g_GrobalParticleInfo.fParticlePosition + (dir.xyz);
//        g_ParticleWritedata[threadIndex.x].vWorldDir = normalize(dir);
       
//        g_ParticleWritedata[threadIndex.x].fLifeTime = g_GrobalParticleInfo.fMinLifeTime;
//        //((g_GrobalParticleInfo.fMaxLifeTime - g_GrobalParticleInfo.fMinLifeTime))
//        //    + g_GrobalParticleInfo.fMinLifeTime - Rand(float2(((float) threadIndex.x / (float) g_GrobalParticleInfo.iMaxCount) + g_GrobalParticleInfo.fAccTime * g_GrobalParticleInfo.fAccTime * g_GrobalParticleInfo.fAccTime, g_GrobalParticleInfo.fAccTime * g_GrobalParticleInfo.fAccTime) / 4);
        
//        g_ParticleWritedata[threadIndex.x].fTransparency = 1.f;
//        g_ParticleWritedata[threadIndex.x].fCurTime = 0.f;
//    }
//    else
//    {
//        // ������ ����
//        g_ParticleWritedata[threadIndex.x].fCurTime += g_GrobalParticleInfo.fDeltaTime;
//        if (g_ParticleWritedata[threadIndex.x].fLifeTime < g_ParticleWritedata[threadIndex.x].fCurTime)
//        {
//            g_ParticleWritedata[threadIndex.x].iAlive = 0;
           
//            return;
//        }

//        float ratio = g_ParticleWritedata[threadIndex.x].fCurTime / g_ParticleWritedata[threadIndex.x].fLifeTime;
//        float speed = (g_GrobalParticleInfo.fMaxSpeed - g_GrobalParticleInfo.fMinSpeed) * ratio + g_GrobalParticleInfo.fMinSpeed;
//        if (ratio < 0.5f)
//        {
//            // ���� �ֱ��� ���� ����: ���� Ƣ�� ȿ��
//            g_ParticleWritedata[threadIndex.x].vWorldPos += g_ParticleWritedata[threadIndex.x].vWorldDir * speed * g_GrobalParticleInfo.fDeltaTime;
//        }
//        else
//        {
//            // ���� �ֱ��� ���� ����: �߷��� ������ �޾� �Ʒ��� ������
//            float3 gravityEffect = g_Gravity * 10.0f; // �߷� ȿ���� ���� ��ȭ
//            g_ParticleWritedata[threadIndex.x].vWorldPos += gravityEffect;
//        }
//        g_ParticleWritedata[threadIndex.x].fTransparency = (1.0f - ratio);
//    }
//}


#endif // _COMPUTE2DPARTICLE_