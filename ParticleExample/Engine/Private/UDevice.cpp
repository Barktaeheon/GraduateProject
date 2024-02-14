#include "EngineDefines.h"
#include "UDevice.h"

UDevice::UDevice() : 
m_cpDevice{ nullptr },
m_cpFactory{nullptr },
#ifndef _DEBUG
m_cpAdaptor{ nullptr }
#else
m_cpAdaptor{ nullptr },
m_cpDebug{nullptr}
#endif
{
}

void UDevice::Free()
{
}

HRESULT UDevice::NativeConstruct()
{
	_uint iDxGiFactoryFlags{ 0 };
#ifdef _DEBUG
	{
		if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&m_cpDebug))))
		{
			m_cpDebug->EnableDebugLayer();
			// Enable additional debug layers.
			iDxGiFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
		}
	}
#endif
	RETURN_CHECK_DXOBJECT(CreateDXGIFactory2(iDxGiFactoryFlags, IID_PPV_ARGS(&m_cpFactory)), E_FAIL);

	m_cpFactory->EnumAdapters(0, (IDXGIAdapter**)m_cpAdaptor.GetAddressOf());
	// ���� �ϵ���� ��ġ ���� ����
	if (FAILED(D3D12CreateDevice(m_cpAdaptor.Get(), D3D_FEATURE_LEVEL_12_0, IID_PPV_ARGS(&m_cpDevice))))
	{
		// Enum Wrap Adapter�� ���� IDXGIAdapter ��ü ������ / ����Ʈ���� ���
		ComPtr<IDXGIAdapter> pWarpAdapter;
		RETURN_CHECK_DXOBJECT(m_cpFactory->EnumWarpAdapter(IID_PPV_ARGS(&pWarpAdapter)), E_FAIL);
		// �ٽ� �� �� ��ġ�� ����
		RETURN_CHECK_DXOBJECT(D3D12CreateDevice(pWarpAdapter.Get(), D3D_FEATURE_LEVEL_12_0, IID_PPV_ARGS(&m_cpDevice)), E_FAIL);
	}

	return S_OK;
}
