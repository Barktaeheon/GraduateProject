#include "EngineDefine.h"
#include "UComputeCommand.h"
#include "UDevice.h"
#include "URootSignature.h"
#include "UGameInstance.h"


UComputeCommand::UComputeCommand() :
	UCommand()
{
}

void UComputeCommand::Free()
{
}

HRESULT UComputeCommand::NativeConstruct(CSHPTRREF<UDevice> _spDevice, const ComPtr<Dx12CommandQueue>& _cpCommandQueue)
{
	RETURN_CHECK_FAILED(__super::NativeConstruct(_spDevice, _cpCommandQueue), E_FAIL);
	const ComPtr<Dx12Device>& DEVICE = _spDevice->GetDV();
	D3D12_COMMAND_QUEUE_DESC ComputeQueuDesc{};
	ComputeQueuDesc.Type = D3D12_COMMAND_LIST_TYPE_COMPUTE;
	ComputeQueuDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	// Create 
	for (auto& iter : m_arrRenderAllocators)
	{
		// Create 
		RETURN_CHECK_DXOBJECT(DEVICE->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_COMPUTE,
			IID_PPV_ARGS(&iter)), E_FAIL);
	}

	RETURN_CHECK_FAILED(CreateGraphicsList(_spDevice, m_arrRenderAllocators[0],
		D3D12_COMMAND_LIST_TYPE_COMPUTE), E_FAIL);

	RETURN_CHECK_FAILED(GetGpuCmdList()->Close(), E_FAIL);

	Clear();
	return S_OK;
}

void UComputeCommand::WaitForSynchronization()
{
	GetGpuCmdList()->Close();
	// �׷��Ƚ� Ŀ�ǵ� ����
	ID3D12CommandList* CmdListArr[] = { GetGpuCmdList().Get() };
	GetCmdQue()->ExecuteCommandLists(CMD_LIST_VALUE, CmdListArr);
	// ����ȭ
	GpuCpuSynchronization();
	++m_iAllocatorIndex;
	Clear();
}

void UComputeCommand::BindRootSignature(CSHPTRREF<URootSignature> _spRootSignature)
{
	GetGpuCmdList()->SetComputeRootSignature(_spRootSignature->GetRootSignature().Get());
}

void UComputeCommand::Clear()
{
	if (m_iAllocatorIndex >= m_arrRenderAllocators.size())
	{
		m_iAllocatorIndex = 0;
	}

	m_arrRenderAllocators[m_iAllocatorIndex]->Reset();
	GetGpuCmdList()->Reset(m_arrRenderAllocators[m_iAllocatorIndex].Get(), nullptr);
}
