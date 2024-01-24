#include "EngineDefine.h"
#include "UGraphicDevice.h"
#include "UDevice.h"
#include "UGpuCommand.h"
#include "USwapChain.h"
#include "URootSignature.h"
#include "UTableDescriptor.h"
#include "URenderTargetGroup.h"
#include "UTexture.h"
#include "UShaderConstantBuffer.h"

UGraphicDevice::UGraphicDevice() :
	m_spDevice{ nullptr },
	m_spGpuCommand{ nullptr },
	m_spSwapChain{ nullptr },
	m_spDefaultRenderTargetGroup{ nullptr },
	m_isGamePaused{ false },
	m_isScreenMinimized{ false },
	m_isScreenMaximized{ false },
	m_isScreenResize{ false },
	m_isScreenFullscreen{ false },
	m_isMssa4xState{ false },
	m_iMssa4xQuality{ 0 },
	m_stD3DViewport{},
	m_stD3DWindowSizeRect{},
	m_spGraphicDesc{ nullptr }
{
}

void UGraphicDevice::Free()
{
}

HRESULT UGraphicDevice::ReadyGraphicDevice(const GRAPHICDESC& _stGraphicsDesc)
{
	m_spGraphicDesc = std::make_shared<GRAPHICDESC>(_stGraphicsDesc);
	// Create DxObject Device Gpu 
	{
		m_spDevice = CreateNative<UDevice>();
		ComPtr<Dx12CommandQueue> m_cpCommandQueue;
		D3D12_COMMAND_QUEUE_DESC GraphicsCommandQueue{};
		GraphicsCommandQueue.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
		GraphicsCommandQueue.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
		RETURN_CHECK_DXOBJECT(m_spDevice->GetDV()->CreateCommandQueue(&GraphicsCommandQueue,
			IID_PPV_ARGS(&m_cpCommandQueue)), E_FAIL);
		m_spGpuCommand = CreateNative<UGpuCommand>(m_spDevice, m_cpCommandQueue);
	}
	// CheckMssa
	{
		D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS msQaulityLevels;
		msQaulityLevels.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		msQaulityLevels.SampleCount = 4;
		msQaulityLevels.Flags = D3D12_MULTISAMPLE_QUALITY_LEVELS_FLAG_NONE;
		msQaulityLevels.NumQualityLevels = 0;

		RETURN_CHECK_DXOBJECT(m_spDevice->GetDV()->CheckFeatureSupport(D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS, &msQaulityLevels,
			sizeof(msQaulityLevels)), E_FAIL);

		m_iMssa4xQuality = msQaulityLevels.NumQualityLevels;
		m_isMssa4xState = m_iMssa4xQuality >= 1 ? true : false;
		assert(m_iMssa4xQuality > 0 && "Unexpected MSAA Qaulity Level.");
		// Erase
		m_isMssa4xState = false;
	}
	// CreateViewPort
	{
		ResizeViewPort();
	}
	// Create DxObject
	{
		// swapChain
		m_spSwapChain = CreateNative<USwapChain>(*m_spGraphicDesc.get(), m_iMssa4xQuality, m_isMssa4xState,
			m_spDevice, m_spGpuCommand);
	}
	//Create RootSignature
	{
		_uint iRegister = static_cast<_uint>(ROOT_REGISTER_TYPE::COMMON);
		m_spRootSignature = CreateNative<URootSignature>(m_spDevice, ROOTREGISTER_SPACE_COMMON);
	}
	//Create TableDescriptor
	{
		const _uint MAX_GRAPHIC_TABLES = 2'0000;
		m_spTableDescriptor = CreateNative<UTableDescriptor>(m_spDevice, MAX_GRAPHIC_TABLES,
			CBV_REGISTER_END + SRV_REGISTER_END, GRAPHICS_CONSTANT_BUFFER_VALUE);
	}

	return S_OK;
}

void UGraphicDevice::OmSetDefaultRenderTarget()
{
	m_spDefaultRenderTargetGroup->OmSetRenderTargets(1, m_spSwapChain->GetBackBufferIndex(), m_spGraphicRenderObject);
}

SHPTR<UTexture> UGraphicDevice::GetDefaultBackTexture()
{
	return m_spDefaultRenderTargetGroup->GetRenderTargetTexture(m_spSwapChain->GetBackBufferIndex());
}

HRESULT UGraphicDevice::OnResize(const _uint& _iWinSizeX, const _uint& _iWinSizeY, const GRAPHICDESC::WINMODE _eWindowMode)
{
	m_spGraphicDesc->iWinCX = _iWinSizeX;
	m_spGraphicDesc->iWinCY = _iWinSizeY;
	m_spGraphicDesc->eWinMode = _eWindowMode;

	m_spSwapChain.reset();
	m_spSwapChain = CreateNative<USwapChain>(*m_spGraphicDesc.get(), m_iMssa4xQuality, m_isMssa4xState,
		m_spDevice, m_spGpuCommand);

	ResizeViewPort();
	return S_OK;
}

HRESULT UGraphicDevice::MainRenderBegin()
{
	UGpuCommand::FRAMECONTEXT* FrameContext = m_spGpuCommand->WaitForNextFrameResources(
		m_spSwapChain->GewtSwapChainWaitableObject(), m_spSwapChain->GetBackBufferIndex());

	ComPtr<Dx12GraphicsCommandList> pGraphicCmdList = m_spGpuCommand->GetGpuCmdList();

	SHPTR<UTexture> pDefaultTexture =
		m_spDefaultRenderTargetGroup->GetRenderTargetTexture(m_spSwapChain->GetBackBufferIndex());

	// �缳���� GPU�� ���� ��� ��ϵ��� ��� ó���� �Ŀ� �Ͼ��. 
	RETURN_CHECK_FAILED(FrameContext->cpAllocator->Reset(), E_FAIL);
	// ��� ����� ��⿭�� �߰��ߴٸ� ��� ����� �缳���ؾ� �Ѵ�. 
	RETURN_CHECK_FAILED(pGraphicCmdList->Reset(FrameContext->cpAllocator.Get(), nullptr), E_FAIL);

	// Set Root Signature
		//m_spCommand->Clear();
	// Bind And Clear 
	m_spTableDescriptor->BindAndClear(m_spGpuCommand);
	// Root Signature
	m_spGpuCommand->BindRootSignature(m_spRootSignature);

	D3D12_RESOURCE_BARRIER Barrier = CD3DX12_RESOURCE_BARRIER::Transition(pDefaultTexture->GetTex().Get(),
		D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
	pGraphicCmdList->ResourceBarrier(1, &Barrier);
	// Set the viewport and scissor rect.  This needs to be reset whenever the command list is reset.
	pGraphicCmdList->RSSetViewports(1, &m_stD3DViewport);
	pGraphicCmdList->RSSetScissorRects(1, &m_stD3DWindowSizeRect);

	m_spDefaultRenderTargetGroup->ClearRenderTargetView(m_spSwapChain->GetBackBufferIndex(), m_spGraphicRenderObject);
	m_spDefaultRenderTargetGroup->OmSetRenderTargets(1, m_spSwapChain->GetBackBufferIndex(), m_spGraphicRenderObject);
	return S_OK;
}

HRESULT UGraphicDevice::MainRenderEnd()
{
	ComPtr<Dx12CommandQueue> pCmdQueue = m_spGpuCommand->GetCmdQue();
	ComPtr<Dx12GraphicsCommandList> pGraphicCmdList = m_spGpuCommand->GetGpuCmdList();

	SHPTR<UTexture> pDefaultTexture =
		m_spDefaultRenderTargetGroup->GetRenderTargetTexture(m_spSwapChain->GetBackBufferIndex());

	D3D12_RESOURCE_BARRIER Barrier = CD3DX12_RESOURCE_BARRIER::Transition(pDefaultTexture->GetTex().Get(),
		D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
	pGraphicCmdList->ResourceBarrier(1, &Barrier);
	pGraphicCmdList->Close();

	// Ŀ�ǵ� ����Ʈ ����
	ID3D12CommandList* cmdListArr[] = { pGraphicCmdList.Get() };
	pCmdQueue->ExecuteCommandLists(_countof(cmdListArr), cmdListArr);

	m_spSwapChain->Present();
	m_spGpuCommand->WaitForEndFrameResource();
	m_spSwapChain->SwapChainIndx();
	return S_OK;
}

CSHPTRREF<UDevice> UGraphicDevice::GetDevice() const
{
	return m_spDevice;
}

CSHPTRREF<UGpuCommand> UGraphicDevice::GetGpuCommand() const
{
	return m_spGpuCommand;
}

CSHPTRREF<UTableDescriptor> UGraphicDevice::GetTableDescriptor() const
{
	return m_spTableDescriptor;
}

CSHPTRREF<URootSignature> UGraphicDevice::GetRootSignature() const
{
	return m_spRootSignature;
}

CSHPTRREF<USwapChain> UGraphicDevice::GetSwapChain() const
{
	return m_spSwapChain;
}

void UGraphicDevice::ResizeViewPort()
{
	// CreateViewPort
	m_stD3DViewport.TopLeftX = 0;
	m_stD3DViewport.TopLeftY = 0;
	m_stD3DViewport.Width = static_cast<_float>(m_spGraphicDesc->iWinCX);
	m_stD3DViewport.Height = static_cast<_float>(m_spGraphicDesc->iWinCY);
	m_stD3DViewport.MinDepth = 0.f;
	m_stD3DViewport.MaxDepth = 1.f;

	m_stD3DWindowSizeRect = CD3DX12_RECT(0, 0, (LONG)(m_spGraphicDesc->iWinCX), (LONG)(m_spGraphicDesc->iWinCY));
}
