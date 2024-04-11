#ifndef _COMPUTEPARAM_
#define _COMPUTEPARAM_


float Rand(float2 co)//0.5~1.0
{
    return 0.5 + (frac(sin(dot(co.xy, float2(12.9898, 78.233))) * 43758.5453)) * 0.5;
}

float RandNormalized(float2 co)
{
    // ���� Rand �Լ��� ���
    float randValue = Rand(co);
    
    // 0.5�� 1.0 ������ ���� 0�� 1 ���̷� ��ȯ
    return (randValue - 0.5f) / 0.5f;
}



#endif 