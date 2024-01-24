#include "EngineDefine.h"
#include "URenderTargetManager.h"
#include "UGraphicDevice.h"
#include "UDevice.h"
#include "UGpuCommand.h"
#include "UTexture.h"
#include "URenderTargetGroup.h"
#include "UGameInstance.h"
#include "UShader.h"
#include "USwapChain.h"

//#include "UVIBufferRect.h"
#include "UShaderConstantBuffer.h"

//#include "UTransform.h"


URenderTargetManager::URenderTargetManager() :
    m_spDepthStencilTexture{ nullptr },
    m_spDevice{ nullptr },
    m_spGraphicDesc{ nullptr },
    m_eDepthFormat{ DXGI_FORMAT_D32_FLOAT }
#ifdef _USE_DEBUGGING
    , m_vecRtObjects{}
    , m_spTransformConstantBuffer{ nullptr }
#endif
{
}

void URenderTargetManager::Free()
{
}

HRESULT URenderTargetManager::ReadyRenderTarget(CSHPTRREF<UGraphicDevice> _spGraphicDevice,
    CSHPTRREF<UDevice> _spDevice)
{
    RETURN_CHECK(nullptr == _spGraphicDevice, E_FAIL);
    m_spDevice = _spDevice;
    m_spGraphicDesc = _spGraphicDevice->GetGraphicDesc();
    // RenderTargetDesc
    CreateDepthStencilTexture();
    CreateDefaultRenderTargets(_spGraphicDevice);
#ifdef _USE_DEBUGGING
    SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);
    // �̹� ������� Shader ConstnatBuffer�� ������
    spGameInstance->GetPreAllocatedConstantBuffer(PREALLOCATED_TRANSFORM, m_spTransformConstantBuffer);
#endif
    return S_OK;
}

HRESULT URenderTargetManager::AddRenderTargetGroup(const RTGROUPID& _eGroupID, const std::vector<RTDESC>& _rtVec)
{
    auto find = m_RenderTargetGroups.find(_eGroupID);
    if (find != m_RenderTargetGroups.end())
        return E_FAIL;

    std::vector<RENDERTARGET> rtGroups{};
    rtGroups.reserve(_rtVec.size());

    for (const RTDESC& iter : _rtVec) {
        RENDERTARGET tTarget{ };
        tTarget.pTexture = CreateNative<UTexture>(m_spDevice, iter);
        tTarget.SetColor(iter.vClear);
        tTarget.stRtDesc = iter;
        rtGroups.push_back(tTarget);
    }

    SHPTR<URenderTargetGroup> pRtGroups = CreateNative<URenderTargetGroup>
        (m_spDevice, rtGroups, m_spDepthStencilTexture);

    m_RenderTargetGroups.insert(std::pair<RTGROUPID, SHPTR<URenderTargetGroup>>(_eGroupID, pRtGroups));
    return S_OK;
}

void URenderTargetManager::RemoveRenderTargetGroup(const RTGROUPID _eGroupID)
{
    const RTGROUPS::iterator& it = m_RenderTargetGroups.find(_eGroupID);
    if (it == m_RenderTargetGroups.end()) {
        return;
    }
    m_RenderTargetGroups.erase(it);
}

// OMSetRenderTarget 
void		URenderTargetManager::OmSetRenderTargets(CSHPTRREF<UCommand> _spCommand, const RTGROUPID& _eGroupID, const _uint _index, const _uint _iOffset)
{
    SHPTR<URenderTargetGroup> pFind = FindRtGroup(_eGroupID);
    if (nullptr != pFind)
        pFind->OmSetRenderTargets(_index, _iOffset, _spCommand);
}
void		URenderTargetManager::OmSetRenderTargets(CSHPTRREF<UCommand> _spCommand, const RTGROUPID& _eGroupID)
{
    SHPTR<URenderTargetGroup> pFind = FindRtGroup(_eGroupID);
    if (nullptr != pFind)
        pFind->OmSetRenderTargets(_spCommand);
}
// ClearRenderTargetView
void		URenderTargetManager::ClearRenderTargetView(CSHPTRREF<UCommand> _spCommand, const RTGROUPID& _eGroupID, const _uint _index)
{
    SHPTR<URenderTargetGroup> pFind = FindRtGroup(_eGroupID);
    if (nullptr != pFind)
        pFind->ClearRenderTargetView(_index, _spCommand);
}
void		URenderTargetManager::ClearRenderTargetView(CSHPTRREF<UCommand> _spCommand, const RTGROUPID& _eGroupID)
{
    SHPTR<URenderTargetGroup> pFind = FindRtGroup(_eGroupID);
    if (nullptr != pFind)
        pFind->ClearRenderTargetView(_spCommand);
}
// Waiting
void		URenderTargetManager::WaitTargetToResource(CSHPTRREF<UCommand> _spCommand, const RTGROUPID& _eGroupID)
{
    SHPTR<URenderTargetGroup> pFind = FindRtGroup(_eGroupID);
    if (nullptr != pFind)
        pFind->WaitTargetToResource(_spCommand);
}
void		URenderTargetManager::WaitResourceToTarget(CSHPTRREF<UCommand> _spCommand, const RTGROUPID& _eGroupID)
{
    SHPTR<URenderTargetGroup> pFind = FindRtGroup(_eGroupID);
    if (nullptr != pFind)
        pFind->WaitResourceToTarget(_spCommand);
}


SHPTR<URenderTargetGroup> URenderTargetManager::FindRenderTargetGroup(const RTGROUPID& _eGroupID)
{
    return FindRtGroup(_eGroupID);
}

SHPTR<UTexture> URenderTargetManager::FindRenderTargetTexture(const RTGROUPID _eGroupID, const RTOBJID _eObjID)
{
    SHPTR<URenderTargetGroup> pGroup = FindRtGroup(_eGroupID);
    RETURN_CHECK(nullptr == pGroup, nullptr);
    return   pGroup->GetRenderTargetTexture(_eObjID);
}

void URenderTargetManager::OnResizeWindow(CSHPTRREF<UGraphicDevice> _spGraphicDevice)
{
    RETURN_CHECK(nullptr == _spGraphicDevice, ;);
    m_spGraphicDesc = _spGraphicDevice->GetGraphicDesc();
    CreateDepthStencilTexture();
    m_RenderTargetGroups.erase(RTGROUPID::DEFAULT);

    for (auto& iter : m_RenderTargetGroups)
    {
        iter.second->RemakeRenderTargets(m_spDevice, m_spDepthStencilTexture, m_spGraphicDesc);
    }
    CreateDefaultRenderTargets(_spGraphicDevice);

#ifdef _USE_DEBUGGING
    OnResizeDebugRenderObject(_spGraphicDevice);
#endif
}

#ifdef _USE_DEBUGGING
void URenderTargetManager::AddDebugRenderObjects(const RTGROUPID _eGroupID, const RTOBJID _eRtObj, const _float2& _vPos, const _float2& _vScale,
    CSHPTRREF<GRAPHICDESC> _spGraphicDesc)
{
    _float4x4 WorldMatrix;
    WorldMatrix._11 = _vScale.x;
    WorldMatrix._22 = _vScale.y;

    WorldMatrix._41 = _vPos.x - _spGraphicDesc->iWinCX * 0.5f;
    WorldMatrix._42 = -_vPos.y + _spGraphicDesc->iWinCY * 0.5f;

    m_vecRtObjects.push_back({ _eGroupID, _eRtObj, WorldMatrix.Transpose(), _vScale, _vPos });
}

void URenderTargetManager::RenderDebugObjects(CSHPTRREF<UShader> _spShader, CSHPTRREF<UVIBufferRect> _spBuffer,
    CSHPTRREF<UCommand> _spCommand, const SRV_REGISTER _eSrvRegister)
{
    for (auto& iter : m_vecRtObjects)
    {
        _spShader->BindCBVBuffer(m_spTransformConstantBuffer, &iter.stTransformParam, UTransform::TRANSFORMPARAM_SIZE);
        _spShader->BindSRVBuffer(_eSrvRegister, FindRenderTargetTexture(iter.eRtGroupID, iter.eRtObjID));
        _spBuffer->Render(_spShader, _spCommand);
    }
}

#endif 

SHPTR<URenderTargetGroup> URenderTargetManager::FindRtGroup(const RTGROUPID& _eGroupID)
{
    RTGROUPS::iterator it = m_RenderTargetGroups.find(_eGroupID);
    if (it == m_RenderTargetGroups.end())
        return nullptr;

    return (*it).second;
}

void URenderTargetManager::CreateDepthStencilTexture()
{
    // RenderTargetDesc
    RTDESC stRtDesc{ RTOBJID::NONALPHA_DIFFUSE_DEFFERED, m_eDepthFormat,
           m_spGraphicDesc->iWinCX, m_spGraphicDesc->iWinCY, _float4(0.f, 0.f, 0.f, 1.f),
           D3D12_HEAP_TYPE_DEFAULT,
           D3D12_HEAP_FLAG_NONE, D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL };
    // Create DepthStencilDesc
    m_spDepthStencilTexture = CreateNative<UTexture>(m_spDevice, stRtDesc);
}

void URenderTargetManager::CreateDefaultRenderTargets(CSHPTRREF<UGraphicDevice> _spGraphicDevice)
{
    SHPTR<USwapChain> pSwapChain{ _spGraphicDevice->GetSwapChain() };
    // SwapChain Group
    {
        VECTOR<RENDERTARGET> rtVec(SWAP_CHAIN_END);
        // Swap Chain End
        for (_uint i = 0; i < SWAP_CHAIN_END; ++i)
        {
            ComPtr<Dx12Resource> resource{ nullptr };
            pSwapChain->GetSwapChain()->GetBuffer(i, IID_PPV_ARGS(resource.GetAddressOf()));
            rtVec[i].pTexture = CreateNative<UTexture>(m_spDevice, resource);
            rtVec[i].SetColor(_float4(0.f, 0.f, 255.f, 1.f));
        }
        // CreateToMethod
        SHPTR<URenderTargetGroup> pRtGroups = CreateNative<URenderTargetGroup>(m_spDevice,
            rtVec, m_spDepthStencilTexture);
        // Emplace RtGroups
        m_RenderTargetGroups.insert(std::pair<RTGROUPID, SHPTR<URenderTargetGroup>>
            (RTGROUPID::DEFAULT, pRtGroups));

        // Set Default RtGroup
        _spGraphicDevice->SetDefaultRtGroup(pRtGroups);
    }
}

