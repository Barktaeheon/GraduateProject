#include "Default.hlsli"

struct VS_INPUT
{
    float3 position : POSITION;
    float4 color : COLOR;
};
//���� ���̴��� ���(�ȼ� ���̴��� �Է�)�� ���� ����ü�� �����Ѵ�. 
struct VS_OUTPUT
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
};
//���� ���̴��� �����Ѵ�.
VS_OUTPUT VSDiffused(VS_INPUT input)
{
    VS_OUTPUT output;
	//������ ��ȯ(���� ��ȯ, ī�޶� ��ȯ, ���� ��ȯ)�Ѵ�. 
    output.position = mul(mul(mul(float4(input.position, 1.0f), gmtxGameObject), gmtxView),
		gmtxProjection);
    output.color = input.color;
    return (output);
}
