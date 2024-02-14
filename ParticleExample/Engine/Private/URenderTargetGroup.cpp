#include "EngineDefines.h"
#include "URenderTargetGroup.h"
#include "UDevice.h"
#include "UGpuCommand.h"
#include "UTexture.h"
#include "URenderObject.h"

URenderTargetGroup::URenderTargetGroup() : 
m_vecRenderTargets{},
m_FindRts{},
m_iRenderTargetCount{ 0 },
m_spDepthStencilTexture{nullptr },
m_stD3DDepthStencilHeapBegin{ NULL },
m_cpRenderTargetHeap{nullptr },
m_iRenderTargetHeapSize{ 0 },
m_stD3DRenderTargetHeapBegin{ NULL },
m_arrD3DTargetToResource{},
m_arrD3DResourceToTarget{}
{
}

void URenderTargetGroup::Free()
{
	m_vecRenderTargets.clear();
}

HRESULT URenderTargetGroup::NativeConstruct(CSHPTRREF<UDevice> _spDevice, const VECTOR<RTDESC>& _vecRtDesc, CSHPTRREF<UTexture> _spDepthStencilTexture)
{
	RETURN_CHECK(nullptr == _spDevice || nullptr == _spDepthStencilTexture, E_FAIL);
	m_spDepthStencilTexture = _spDepthStencilTexture;
	m_iRenderTargetCount = static_cast<_uint>(_vecRtDesc.size());
	m_vecRenderTargets.reserve(m_iRenderTargetCount);
	// RenderTarget
	for (auto& iter : _vecRtDesc)
	{
		RENDERTARGET stTarget;
		stTarget.pTexture = CreateNative<UTexture>(_spDevice, iter);
		stTarget.SetColor(iter.vClear);
		stTarget.stRtDesc = iter;
		m_vecRenderTargets.push_back(stTarget);
	}
	return ReadyTargets(_spDevice);
}

HRESULT URenderTargetGroup::NativeConstruct(CSHPTRREF<UDevice> _spDevice, const VECTOR<RENDERTARGET>& _vecRenderTargets, CSHPTRREF<UTexture> _spDepthStencilTexture)
{
	RETURN_CHECK(nullptr == _spDevice || nullptr == _spDepthStencilTexture, E_FAIL);
	// Setting Values 
	m_spDepthStencilTexture = _spDepthStencilTexture;
	m_iRenderTargetCount = static_cast<_uint>(_vecRenderTargets.size());
	m_vecRenderTargets = _vecRenderTargets;
	return ReadyTargets(_spDevice);
}

void URenderTargetGroup::RemakeRenderTargets(CSHPTRREF<UDevice> _spDevice, CSHPTRREF<UTexture> _spDepthStencilTexture, CSHPTRREF<GRAPHICDESC> _spGraphicDesc)
{
	RETURN_CHECK(nullptr == _spDepthStencilTexture || nullptr == _spGraphicDesc, ;);
	m_spDepthStencilTexture = _spDepthStencilTexture;
	for (auto& iter : m_vecRenderTargets)
	{
		iter.stRtDesc.iWidth = _spGraphicDesc->iWinCX;
		iter.stRtDesc.iHeight = _spGraphicDesc->iWinCY;
		iter.pTexture.reset();
		iter.pTexture = CreateNative<UTexture>(_spDevice, iter.stRtDesc);
	}
}

HRESULT URenderTargetGroup::ReadyTargets(CSHPTRREF<UDevice> _spDevice)
{
	// RenderTarget Heap�� �����. 
	D3D12_DESCRIPTOR_HEAP_DESC HeapDesc{};
	HeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	HeapDesc.NumDescriptors = m_iRenderTargetCount;
	HeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	HeapDesc.NodeMask = 0;

	// Create Descriptor Heap
	RETURN_CHECK_FAILED(_spDevice->GetDV()->CreateDescriptorHeap(&HeapDesc,
		IID_PPV_ARGS(&m_cpRenderTargetHeap)), E_FAIL);
	// RtvHeap�� ����� ������
	m_iRenderTargetHeapSize = _spDevice->GetDV()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	// RtvHeap�� ���� �ּҸ� ������
	m_stD3DRenderTargetHeapBegin = m_cpRenderTargetHeap->GetCPUDescriptorHandleForHeapStart();
	// DepthStencilHeap�� Begin�� ������
	m_stD3DDepthStencilHeapBegin = m_spDepthStencilTexture->GetHeap(DESCRIPTOR_TYPE::DSV)->GetCPUDescriptorHandleForHeapStart();

	// RTV ���ڸ�ŭ
	for (_uint i = 0; i < m_iRenderTargetCount; ++i)
	{
		// DescSize�� ������
		_uint DescSize = 1;
		D3D12_CPU_DESCRIPTOR_HANDLE DescHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE(m_stD3DRenderTargetHeapBegin, i * m_iRenderTargetHeapSize);
		// ������ RenderTargetHeap�� ������
		_uint SrcSize = 1;
		ComPtr<Dx12DescriptorHeap> SrcRtvHeapBegin = m_vecRenderTargets[i].pTexture->GetHeap(DESCRIPTOR_TYPE::RTV);
		D3D12_CPU_DESCRIPTOR_HANDLE SrcHandle = SrcRtvHeapBegin->GetCPUDescriptorHandleForHeapStart();
		_spDevice->GetDV()->CopyDescriptors(1, &DescHandle, &DescSize, 1, &SrcHandle, &SrcSize, D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

		m_FindRts.insert(std::pair<RTOBJID, _uint>(m_vecRenderTargets[i].stRtDesc.eRTObjID, i));
	} 
	for (_uint i = 0; i < m_iRenderTargetCount; ++i)
	{
		m_arrD3DTargetToResource[i]  = CD3DX12_RESOURCE_BARRIER::Transition(m_vecRenderTargets[i].pTexture->GetTex().Get(),
			D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_COMMON);

		m_arrD3DResourceToTarget[i] = CD3DX12_RESOURCE_BARRIER::Transition(m_vecRenderTargets[i].pTexture->GetTex().Get(),
			D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_RENDER_TARGET);
	}
	return S_OK;
}

// SetRenderTargets 
void URenderTargetGroup::OmSetRenderTargets(const _uint _index, const _uint _iOffset, CSHPTRREF<URenderObject> _spRenderObject)
{
	OmSetRenderTargets(_index, _iOffset, _spRenderObject->GetCommand());
}
void URenderTargetGroup::OmSetRenderTargets(CSHPTRREF<URenderObject> _spRenderObject)
{
	OmSetRenderTargets(_spRenderObject->GetCommand());
}
//Clear
void URenderTargetGroup::ClearRenderTargetView(const _uint _index, CSHPTRREF<URenderObject> _spRenderObject)
{
	ClearRenderTargetView(_index, _spRenderObject->GetCommand());
}
void URenderTargetGroup::ClearRenderTargetView(CSHPTRREF<URenderObject> _spRenderObject)
{
	ClearRenderTargetView(_spRenderObject->GetCommand());
}
void URenderTargetGroup::WaitTargetToResource(CSHPTRREF<URenderObject> _spRenderObject)
{
	WaitTargetToResource(_spRenderObject->GetCommand());
}
void URenderTargetGroup::WaitResourceToTarget(CSHPTRREF<URenderObject> _spRenderObject)
{
	WaitResourceToTarget(_spRenderObject->GetCommand());
}

void URenderTargetGroup::OmSetRenderTargets(const _uint _index, const _uint _iOffset, CSHPTRREF<UCommand> _spCommand)
{
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE(m_stD3DRenderTargetHeapBegin,
		_iOffset * m_iRenderTargetHeapSize);
	_spCommand->GetGpuCmdList()->OMSetRenderTargets(_index, &rtvHandle,
		FALSE, &m_stD3DDepthStencilHeapBegin);
}

void URenderTargetGroup::OmSetRenderTargets(CSHPTRREF<UCommand> _spCommand)
{
	_spCommand->GetGpuCmdList()->OMSetRenderTargets(m_iRenderTargetCount, &m_stD3DRenderTargetHeapBegin,
		TRUE/*����*/, &m_stD3DDepthStencilHeapBegin);
}

//Clear
void URenderTargetGroup::ClearRenderTargetView(const _uint _index, CSHPTRREF<UCommand> _spCommand)
{
	// RenderTargetHandle�� Index ��ŭ �����´�. 
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE(m_stD3DRenderTargetHeapBegin,
		_index * m_iRenderTargetHeapSize);
	// RenderTarget�� Clear�Ѵ�. 
	_spCommand->GetGpuCmdList()->ClearRenderTargetView(rtvHandle,
		m_vecRenderTargets[_index].arrClearColor.data(), 0, nullptr);
	_spCommand->GetGpuCmdList()->ClearDepthStencilView(m_stD3DDepthStencilHeapBegin,
		D3D12_CLEAR_FLAG_DEPTH, 1.f, 0, 0, nullptr);
}

void URenderTargetGroup::ClearRenderTargetView(CSHPTRREF<UCommand> _spCommand)
{
//	WaitResourceToTarget(_spCommand);

	for (_uint i = 0; i < m_iRenderTargetCount; i++)
	{
		D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE(
			m_cpRenderTargetHeap->GetCPUDescriptorHandleForHeapStart(), i * m_iRenderTargetHeapSize);

		_spCommand->GetGpuCmdList()->ClearRenderTargetView(rtvHandle,
			m_vecRenderTargets[i].arrClearColor.data(), 0, nullptr);
	}

	_spCommand->GetGpuCmdList()->ClearDepthStencilView(m_stD3DDepthStencilHeapBegin,
		D3D12_CLEAR_FLAG_DEPTH, 1.f, 0, 0, nullptr);
}

void URenderTargetGroup::WaitTargetToResource(CSHPTRREF<UCommand> _spCommand)
{
	_spCommand->GetGpuCmdList()->ResourceBarrier(m_iRenderTargetCount, m_arrD3DTargetToResource.data());
}
void URenderTargetGroup::WaitResourceToTarget(CSHPTRREF<UCommand> _spCommand)
{
	_spCommand->GetGpuCmdList()->ResourceBarrier(m_iRenderTargetCount, m_arrD3DResourceToTarget.data());
}


SHPTR<UTexture> URenderTargetGroup::GetRenderTargetTexture(const RTOBJID _eObJID)
{
	const UNORMAP<RTOBJID, _uint>::iterator& IDIterator = m_FindRts.find(_eObJID);
	RETURN_CHECK(m_FindRts.end() == IDIterator, nullptr);

	return m_vecRenderTargets[IDIterator->second].pTexture;
}

SHPTR<UTexture> URenderTargetGroup::GetRenderTargetTexture(const _uint _iObjID)
{
	if (m_vecRenderTargets.size() <= _iObjID)
		return nullptr;

	return m_vecRenderTargets[_iObjID].pTexture;
}