#ifndef _LIGHTSHADERFUNC_HLSLI_
#define _LIGHTSHADERFUNC_HLSLI_

#include "LightShaderParam.hlsli"


float RimLight(float3 vWorldPos,  float3 vNormal,int index)
{
    float3 vCamLook = normalize(float3(vWorldPos.xyz - g_ViewProjInfoArr[g_CamID].vCamPosition.xyz));
    float4 vLightDir = normalize(g_tLightInfo[index].vDirection);
    vLightDir.w = 0.f;
    float fLightDensity = dot(vCamLook * -1.f, normalize(vNormal));

    float rimLightIntensity = max(0.f, 1.f - fLightDensity);
    rimLightIntensity = pow(rimLightIntensity, 12);
    rimLightIntensity = smoothstep(0.2, 0.8, rimLightIntensity);
   

    return rimLightIntensity;
}


//=========================================================================================================================================

#define DIRECTIONAL_LIGHT	0
#define POINT_LIGHT			1
#define SPOT_LIGHT			2

//#define _WITH_LOCAL_VIEWER_HIGHLIGHTING
#define _WITH_THETA_PHI_CONES
#define _WITH_REFLECT




LIGHTCOLOR DirectionalLight(int index, float3 vNormal, float3 vToCamera)
{
    
    LIGHTCOLOR color = (LIGHTCOLOR) 0.f;
    float3 vToLight = -g_tLightInfo[index].vDirection.xyz;
    float fDiffuseFactor = dot(vToLight, vNormal);
    float fSpecularFactor = 0.0f;
  
    if (fDiffuseFactor > 0.0f)
    {
      
            float3 vHalf = normalize(vToCamera + vToLight);

        fSpecularFactor = pow(max(dot(vHalf, vNormal), 0.0f), g_tLightInfo[index].fSpecularPowValue);

        
    }
  
    color.vAmbient = g_tLightInfo[index].vAmbient;
    color.vDiffuse = g_tLightInfo[index].vDiffuse * fDiffuseFactor;
    color.vSpecular = g_tLightInfo[index].vSpecular * fSpecularFactor;
   
    
    return color;

}

LIGHTCOLOR PointLight(int index, float3 vPosition, float3 vNormal, float3 vToCamera)
{

    if (vPosition.x == 0 && vPosition.y == 0 && vPosition.z == 0) {
        return (LIGHTCOLOR)0.f;
    }
    LIGHTCOLOR color = (LIGHTCOLOR)0.f;

    float3 vToLight = g_tLightInfo[index].vPosition.xyz - vPosition;
    float fDistance = length(vToLight);

    if (fDistance <= g_tLightInfo[index].fRange)
    {
        float fSpecularFactor = 0.0f;
        vToLight /= fDistance;
        float fDiffuseFactor = dot(vToLight, vNormal);

        // ����ŧ�� �Ŀ�
        float fSpecularPowValue = g_tLightInfo[index].fSpecularPowValue;

        if (fDiffuseFactor > 0.0f) {
            float3 vHalf = normalize(vToCamera + vToLight);
            fSpecularFactor = pow(max(dot(vHalf, vNormal), 0.0f), fSpecularPowValue);
        }

        // ���� ��� ���
        float fAttenuationFactor = 1.0f;
        float attenuationDistance = g_tLightInfo[index].fRange * 0.8f; // ���� ���� �Ÿ�

        if (fDistance > attenuationDistance) {
            float normalizedDistance = (fDistance - attenuationDistance) / (g_tLightInfo[index].fRange - attenuationDistance);
            fAttenuationFactor = 1.0f - normalizedDistance;

            // ���� ����� �ּ� 0���� ����
            if (fAttenuationFactor < 0.0f) {
                fAttenuationFactor = 0.0f;
            }
        }
           // HDR(��ٵ�������) ȿ���� ���� �ȼ� ���� Ȯ��
   
        // ���� ���
        color.vAmbient = g_tLightInfo[index].vAmbient * fAttenuationFactor;
        color.vDiffuse = g_tLightInfo[index].vDiffuse * fDiffuseFactor * fAttenuationFactor;
        color.vSpecular = g_tLightInfo[index].vSpecular * fSpecularFactor * fAttenuationFactor;

        return color;
    }

    // ����Ʈ ����Ʈ ���� ���� �ȼ��� ���� ��ȯ ��
    return (LIGHTCOLOR)0.f;
}



LIGHTCOLOR SpotLight(int index, float3 vPosition, float3 vNormal, float3 vToCamera)
{
    if (vPosition.x == 0 && vPosition.y == 0 && vPosition.z == 0)
    {
        return (LIGHTCOLOR) 0.f;
    }

    LIGHTCOLOR color = (LIGHTCOLOR) 0.f;
    float3 vToLight = g_tLightInfo[index].vPosition.xyz - vPosition;
    float fDistance = length(vToLight);

    // ���̶���Ʈ ���� ���� ���� ���� ���
    if (fDistance <= g_tLightInfo[index].fRange)
    {
        float fSpecularFactor = 0.0f;
        vToLight /= fDistance;
        float fDiffuseFactor = dot(vToLight, vNormal);

        // �⺻ ����ŧ�� �Ŀ� ��
        float defaultShininess = 32.0f; //���� fSpecularPowValue�� ��ü

        // �⺻ ����ŧ�� ���� (��: ���)
        float4 defaultSpecularColor = float4(0.7f, 0.7f, 0.7f, 1.0f);
        if (fDiffuseFactor > 0.0f)
        {
            // ���� ���� ���
            float3 vHalf = normalize(vToCamera + vToLight);

            // fSpecularFactor ���
            fSpecularFactor = pow(max(dot(vHalf, vNormal), 0.0f), g_tLightInfo[index].fSpecularPowValue);

            // �����÷� ���� fSpecularFactor ����
        }


        float3 LightDir = normalize(g_tLightInfo[index].vDirection.xyz);
        float fAlpha = max(dot(-vToLight, LightDir), 0.0f);
        float fSpotFactor = pow(max(((fAlpha - g_tLightInfo[index].fPhi) / (g_tLightInfo[index].fTheta - g_tLightInfo[index].fPhi)), 0.0f), g_tLightInfo[index].fFallOff);

        // ���� ���� �Ÿ��� ���̶���Ʈ �ִ� �������� �ణ ���� ������ ����
        float fStartAttenuationDistance = g_tLightInfo[index].fRange * 0.8f;

        // ���� ��� ���
        float fAttenuationFactor = 1.0f;
        if (fDistance > fStartAttenuationDistance)
        {
            float normalizedDistance = (fDistance - fStartAttenuationDistance) / (g_tLightInfo[index].fRange - fStartAttenuationDistance);
            fAttenuationFactor = 1.0f - normalizedDistance;
            if (fAttenuationFactor < 0.0f)
            {
                fAttenuationFactor = 0.0f;
            }
        }

        // color ���
        color.vAmbient = g_tLightInfo[index].vAmbient * fAttenuationFactor * fSpotFactor;
        color.vDiffuse = g_tLightInfo[index].vDiffuse * fDiffuseFactor * fSpotFactor * fAttenuationFactor;
        color.vSpecular = g_tLightInfo[index].vSpecular * defaultSpecularColor * fAttenuationFactor * fSpotFactor * fSpecularFactor;

        return color;
    }

    // ���̶���Ʈ ���� ���� �ȼ��� ���� ��ȯ ��
    return (LIGHTCOLOR) 0.f;
}





//==============================================



LIGHTCOLOR LightingInWorld(float3 vWorldPosition, float3 vWorldNormal)
{
    
    LIGHTCOLOR color = (LIGHTCOLOR) 0.f;
   // float3 vCameraPosition = float3(g_tLightParam.vLightCamPos.x, g_tLightParam.vLightCamPos.y, g_tLightParam.vLightCamPos.z);
    float3 vToCamera = normalize(g_ViewProjInfoArr[MAIN_CAM_ID].vCamPosition - vWorldPosition);
   
    float4 vCameraViewPosition = mul(float4(g_ViewProjInfoArr[MAIN_CAM_ID].vCamPosition, 1.0f), g_ViewProjInfoArr[g_CamID].mViewMatrix);
    float3 vViewToCamera = normalize(vCameraViewPosition.xyz - vWorldPosition);

      [unroll(MAX_LIGHTS)]
    for (int i = 0; i < nLights; i++)
    {
        if (g_tLightInfo[i].eLightActive)
        {
       
        if (g_tLightInfo[i].eLightType == DIRECTIONAL_LIGHT)
        {
       
            color.vDiffuse += DirectionalLight(i,vWorldNormal, vToCamera).vDiffuse;

            color.vAmbient += DirectionalLight(i,vWorldNormal, vToCamera).vAmbient;

            color.vSpecular += DirectionalLight(i,vWorldNormal, vToCamera).vSpecular;

        }
        else if (g_tLightInfo[i].eLightType == POINT_LIGHT)
        {
            color.vDiffuse += PointLight(i,vWorldPosition, vWorldNormal, vToCamera).vDiffuse;

            color.vAmbient += PointLight(i,vWorldPosition, vWorldNormal, vToCamera).vAmbient;

                color.vSpecular += PointLight(i, vWorldPosition, vWorldNormal, vToCamera).vSpecular; //+ RimLight(vWorldPosition, vWorldNormal,i)

        }
        else if (g_tLightInfo[i].eLightType == SPOT_LIGHT)
        {
            color.vDiffuse += SpotLight(i,vWorldPosition, vWorldNormal, vToCamera).vDiffuse;

            color.vAmbient += SpotLight(i,vWorldPosition, vWorldNormal, vToCamera).vAmbient;

            color.vSpecular += SpotLight(i,vWorldPosition, vWorldNormal, vToCamera).vSpecular;
        }
        }
    }
    color.vAmbient += g_vGlobalAmbient;
  //  color.vDiffuse = saturate(color.vDiffuse * g_tLightInfo.fLightPower);
 
    color.vDiffuse.a = 1.f;
    
    return (color);
}

     




#endif // _LIGHTSHADERFUNC_HLSLI_