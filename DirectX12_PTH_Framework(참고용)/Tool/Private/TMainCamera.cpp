#include "ToolDefines.h"
#include "TMainCamera.h"
#include "UGameInstance.h"
#include "UTransform.h"

TMainCamera::TMainCamera(CSHPTRREF<UDevice> _spDevice,  const _wstring& _wstrLayer, const CLONETYPE& _eCloneType) 
    : UCamera(_spDevice,  _wstrLayer, _eCloneType),
    m_isMoveState{true}
{
}

TMainCamera::TMainCamera(const TMainCamera& _rhs) : 
    UCamera(_rhs),
    m_isMoveState{ true }
{
}

void TMainCamera::Free()
{
}

HRESULT TMainCamera::NativeConstruct()
{
    if (FAILED(__super::NativeConstruct()))
        return E_FAIL;

    return S_OK;
}

HRESULT TMainCamera::NativeConstructClone(const VOIDDATAS& _vecDatas)
{
    if (FAILED(__super::NativeConstructClone(_vecDatas)))
        return E_FAIL;

    return S_OK;
}

void TMainCamera::TickActive(const _double& _dTimeDelta)
{
}

void TMainCamera::LateTickActive(const _double& _dTimeDelta)
{
    if (true == m_isMoveState)
    {
        SHPTR<UGameInstance> pGameInstance = GET_INSTANCE(UGameInstance);

        if (pGameInstance->GetDIKeyPressing(DIK_W))
            GetTransform()->MoveForward(_dTimeDelta, 20.f);
        if (pGameInstance->GetDIKeyPressing(DIK_A))
            GetTransform()->MoveLeft(_dTimeDelta, 20.f);
        if (pGameInstance->GetDIKeyPressing(DIK_D))
            GetTransform()->MoveRight(_dTimeDelta, 20.f);
        if (pGameInstance->GetDIKeyPressing(DIK_S))
            GetTransform()->MoveBack(_dTimeDelta, 20.f);

        _float2 MousePos = pGameInstance->GetMousePosition();
        if (MousePos.x > 0 && MousePos.x < WINDOW_WIDTH)
        {
            if (MousePos.y > 0 && MousePos.y < WINDOW_HEIGHT)
            {
                _long		MouseMove = 0;

                if (MouseMove = pGameInstance->GetDIMMoveState(DIMOUSEMOVE::DIMM_X))
                    GetTransform()->RotateTurn(_float3(0.f, 1.f, 0.f), DirectX::XMConvertToRadians(90.f) * MouseMove * 0.1f, _dTimeDelta);
            }
        }

        
    }
    else
    {

    }
}
