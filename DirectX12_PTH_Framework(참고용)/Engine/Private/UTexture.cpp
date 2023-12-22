#include "EngineDefines.h"
#include "UTexture.h"
#include "UDevice.h"
#include "UGpuCommand.h"
#include <DirectXTK/ResourceUploadBatch.h>
#include <DirectXTK/DDSTextureLoader.h>
#include <DirectXTK/WICTextureLoader.h>
#include <filesystem>
#include "UGameInstance.h"

namespace fs = std::filesystem;

UTexture::UTexture() : 
	m_eTextureDescriptorType{ DESCRIPTOR_TYPE::TYPE_END },
	m_cpTexture{ NULL },
	m_eDXTextureFormat{ DXGI_FORMAT_UNKNOWN },
	m_fTextureWidth{ 0.f },
	m_fTextureHeight{ 0.f }
{
}

const D3D12_CPU_DESCRIPTOR_HANDLE& UTexture::GetShaderViewBeginHandle(const DESCRIPTOR_TYPE& _eType) 
{
	const DESCRIPTORHANDLES::iterator& iter = m_DescriptorHandles.find(_eType);
	RETURN_CHECK(iter == m_DescriptorHandles.end(), NULL_HANDLE);
	return iter->second;
}

void UTexture::Free()
{
	m_DescriptorHeaps.clear();
}

HRESULT UTexture::NativeConstruct(CSHPTRREF<UDevice> _spDevice,  const _wstring& _wstrPath, const TEXTURECREATETYPE _eTextureType)
{
	RETURN_CHECK(nullptr == _spDevice, E_FAIL);

	// Create Texture
	{
		{
			// Resource ���ε� �غ�
			DirectX::ResourceUploadBatch ResoureceUpLoad{_spDevice->GetDV().Get()};
			ResoureceUpLoad.Begin();

		// ������ �����ͼ� Ȯ���ڿ� ���� �ؽ��� �ε��ϱ� 
		_wstring ext = fs::path(_wstrPath).extension();
		// �ؽ��� �ε�
			 _wstring dds{L".dds"};
			 _wstring DDS{L".DDS"};
			 _wstring tga{ L".tga" };
			 _wstring TGA{ L".TGA" };
			Dx12Device* pDevice =  _spDevice->GetDV().Get();

			if (ext == dds || ext == DDS || ext == tga || ext == TGA)
			{
				RETURN_CHECK_FAILED(DirectX::CreateDDSTextureFromFile(pDevice, ResoureceUpLoad,
					_wstrPath.c_str(), &m_cpTexture), E_FAIL);
			}
			else // png, jpg, jpeg, bmp
			{
				RETURN_CHECK_FAILED(DirectX::CreateWICTextureFromFile(pDevice, ResoureceUpLoad,
					_wstrPath.c_str(), &m_cpTexture), E_FAIL);
			}

			SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);

			auto uploadResourcesFinished = ResoureceUpLoad.End(spGameInstance->GetGpuCommandQueue().Get());
			// Wait for the upload thread to terminate
			uploadResourcesFinished.wait();
		}
		auto TextureDesc = m_cpTexture->GetDesc();
		m_eDXTextureFormat = TextureDesc.Format;

		m_fTextureWidth = static_cast<_float>(TextureDesc.Width);
		m_fTextureHeight = static_cast<_float>(TextureDesc.Height);
	}
	RETURN_CHECK_FAILED(CreateView(_spDevice, _eTextureType), E_FAIL);

	m_eTextureDescriptorType = DESCRIPTOR_TYPE::SRV;
	return S_OK;
}

HRESULT UTexture::NativeConstruct(CSHPTRREF<UDevice> _spDevice, const VECTOR<_wstring>& _convecPaths)
{
	return E_NOTIMPL;
}

HRESULT UTexture::NativeConstruct(CSHPTRREF<UDevice> _spDevice, const TEXTURETYPEDESC& _stTextureDesc)
{
	RETURN_CHECK(nullptr == _spDevice, E_FAIL);
	// Texture�� ����� ���� CD3DX12_RESOURCE_DESC 2D 
	D3D12_RESOURCE_DESC Desc = CD3DX12_RESOURCE_DESC::Tex2D(_stTextureDesc.eDxFormat, _stTextureDesc.iWidth,
		_stTextureDesc.iHeight);
	Desc.Flags = _stTextureDesc.eResourceFlags;

	D3D12_CLEAR_VALUE* pOptimizedClearValue = nullptr;
	D3D12_CLEAR_VALUE OptimizedClearValue{};
	D3D12_RESOURCE_STATES ResourceStates{ D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_COMMON };

	// RenderTargetDesc�� ���� DepthStencil Ȥ�� RenderTarget�� ���� �غ� �Ѵ�. 
	if (D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL & _stTextureDesc.eResourceFlags)
	{
		ResourceStates = D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_DEPTH_WRITE;
		OptimizedClearValue = CD3DX12_CLEAR_VALUE(DXGI_FORMAT_D32_FLOAT, 1.f, 0);
		pOptimizedClearValue = &OptimizedClearValue;
	}
	else if (D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET & _stTextureDesc.eResourceFlags )
	{
		ResourceStates = D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_COMMON;
		float arrFloat[4] = { _stTextureDesc.vClear.x, _stTextureDesc.vClear.y, _stTextureDesc.vClear.z, _stTextureDesc.vClear.w };
		OptimizedClearValue = CD3DX12_CLEAR_VALUE(_stTextureDesc.eDxFormat, arrFloat);
		pOptimizedClearValue = &OptimizedClearValue;
	}

	// �ش��ϴ� ���ҽ��� ����� 
	RETURN_CHECK_DXOBJECT(_spDevice->GetDV()->CreateCommittedResource(&_stTextureDesc.HeapProPerty, _stTextureDesc.eHeapFlags,
		&Desc, ResourceStates, pOptimizedClearValue, IID_PPV_ARGS(&m_cpTexture)), E_FAIL);
	// �� �ؽ��Ŀ� ���� ���ҽ��� �����ϱ� ���� �غ� �Ѵ�. 
	RETURN_CHECK_FAILED(CreateFromResource(_spDevice, m_cpTexture), E_FAIL);
	return S_OK;
}

HRESULT UTexture::NativeConstruct(CSHPTRREF<UDevice> _spDevice, const ComPtr<Dx12Resource>& _cpResource)
{
	RETURN_CHECK(nullptr == _spDevice , E_FAIL);
	RETURN_CHECK_FAILED(CreateFromResource(_spDevice, _cpResource), E_FAIL);
	return S_OK;
}

HRESULT UTexture::CreateView(CSHPTRREF<UDevice> _spDevice, const TEXTURECREATETYPE _eTextureType)
{
	// ShaderHeap �� �����ϳ��� �����. 
	D3D12_DESCRIPTOR_HEAP_DESC SrvHeapDesc{};
	SrvHeapDesc.NumDescriptors = 1;
	SrvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	SrvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	ComPtr<Dx12DescriptorHeap> cpDescriptor = nullptr;
	_spDevice->GetDV()->CreateDescriptorHeap(&SrvHeapDesc, IID_PPV_ARGS(&cpDescriptor));
	m_DescriptorHeaps.insert(std::pair <DESCRIPTOR_TYPE, ComPtr<Dx12DescriptorHeap>>(DESCRIPTOR_TYPE::SRV, cpDescriptor));
	m_DescriptorHandles.insert(std::pair <DESCRIPTOR_TYPE, D3D12_CPU_DESCRIPTOR_HANDLE>(DESCRIPTOR_TYPE::SRV, cpDescriptor->GetCPUDescriptorHandleForHeapStart()));
	// ShaderHeap�� �� �� �ִ� View�� �����. 
	D3D12_SHADER_RESOURCE_VIEW_DESC ShaderView{};
	ShaderView.Format = m_eDXTextureFormat;
	ShaderView.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;

	switch (_eTextureType)
	{
	case TEXTURECREATETYPE::TEX2D:
		ShaderView.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		ShaderView.Texture2D.MipLevels = 1;
		ShaderView.Texture2D.MostDetailedMip = 0;
		ShaderView.Texture2D.PlaneSlice = 0;
		ShaderView.Texture2D.ResourceMinLODClamp = 0.0f;
		break;
	case TEXTURECREATETYPE::TEXCUBE:
		ShaderView.ViewDimension = D3D12_SRV_DIMENSION_TEXTURECUBE;
		ShaderView.TextureCube.MipLevels = 1;
		ShaderView.TextureCube.MostDetailedMip = 0;
		ShaderView.TextureCube.ResourceMinLODClamp = 0.0f;
		break;
	case TEXTURECREATETYPE::TEXDIMMESION:

		break;
	case TEXTURECREATETYPE::TEXARRAY:
		ShaderView.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2DARRAY;
		ShaderView.Texture2DArray.MipLevels = -1;
		ShaderView.Texture2DArray.MostDetailedMip = 0;
		ShaderView.Texture2DArray.PlaneSlice = 0;
		ShaderView.Texture2DArray.ResourceMinLODClamp = 0.0f;
		ShaderView.Texture2DArray.FirstArraySlice = 0;
		ShaderView.Texture2DArray.ArraySize = 1;
		break;
	}

	_spDevice->GetDV()->CreateShaderResourceView(m_cpTexture.Get(), &ShaderView, cpDescriptor->GetCPUDescriptorHandleForHeapStart());
	return S_OK;
}

HRESULT UTexture::CreateFromResource(CSHPTRREF<UDevice> _spDevice, const ComPtr<Dx12Resource>& _cpResource)
{
	RETURN_CHECK(nullptr == _cpResource, E_FAIL);
	m_cpTexture = _cpResource;
	m_eDXTextureFormat = m_cpTexture->GetDesc().Format;

	D3D12_RESOURCE_DESC Desc{ m_cpTexture->GetDesc() };

	if (D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL & Desc.Flags)
	{
		// ���� DepthStencil�̶��, DepthStencil View�� Descriptor�� �����. 
		D3D12_DESCRIPTOR_HEAP_DESC HeapDesc{};
		HeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
		HeapDesc.NumDescriptors = 1;
		HeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		HeapDesc.NodeMask = 0;
		ComPtr<Dx12DescriptorHeap> cpDescriptor = nullptr;
		_spDevice->GetDV()->CreateDescriptorHeap(&HeapDesc, IID_PPV_ARGS(&cpDescriptor));
		m_DescriptorHeaps.insert(std::pair <DESCRIPTOR_TYPE, ComPtr<Dx12DescriptorHeap>>(DESCRIPTOR_TYPE::DSV, cpDescriptor));
		m_DescriptorHandles.insert(std::pair <DESCRIPTOR_TYPE, D3D12_CPU_DESCRIPTOR_HANDLE>(DESCRIPTOR_TYPE::DSV, cpDescriptor->GetCPUDescriptorHandleForHeapStart()));
		D3D12_CPU_DESCRIPTOR_HANDLE DSVHandle{ cpDescriptor->GetCPUDescriptorHandleForHeapStart() };
		_spDevice->GetDV()->CreateDepthStencilView(m_cpTexture.Get(), nullptr, DSVHandle);

		m_eTextureDescriptorType = DESCRIPTOR_TYPE::DSV;
	}
	else
	{
		if (D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET & Desc.Flags)
		{
			// RenderTarget ������ ��, View�� Desriptor�� �����. 
			D3D12_DESCRIPTOR_HEAP_DESC HeapDesc{};
			HeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
			HeapDesc.NumDescriptors = 1;
			HeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
			HeapDesc.NodeMask = 0;
			ComPtr<Dx12DescriptorHeap> cpDescriptor = nullptr;
			_spDevice->GetDV()->CreateDescriptorHeap(&HeapDesc, IID_PPV_ARGS(&cpDescriptor));
			m_DescriptorHeaps.insert(std::pair <DESCRIPTOR_TYPE, ComPtr<Dx12DescriptorHeap>>(DESCRIPTOR_TYPE::RTV, cpDescriptor));
			m_DescriptorHandles.insert(std::pair <DESCRIPTOR_TYPE, D3D12_CPU_DESCRIPTOR_HANDLE>(DESCRIPTOR_TYPE::RTV, cpDescriptor->GetCPUDescriptorHandleForHeapStart()));
			D3D12_CPU_DESCRIPTOR_HANDLE rtvHeapBegin = cpDescriptor->GetCPUDescriptorHandleForHeapStart();
			_spDevice->GetDV()->CreateRenderTargetView(m_cpTexture.Get(), nullptr, rtvHeapBegin);

			m_eTextureDescriptorType = DESCRIPTOR_TYPE::RTV;
		}
		else if (D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS & Desc.Flags)
		{
			D3D12_DESCRIPTOR_HEAP_DESC HeapDesc{};
			HeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
			HeapDesc.NumDescriptors = 1;
			HeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
			HeapDesc.NodeMask = 0;
			ComPtr<Dx12DescriptorHeap> cpDescriptor = nullptr;
			_spDevice->GetDV()->CreateDescriptorHeap(&HeapDesc, IID_PPV_ARGS(&cpDescriptor));
			m_DescriptorHeaps.insert(std::pair <DESCRIPTOR_TYPE, ComPtr<Dx12DescriptorHeap>>(DESCRIPTOR_TYPE::UAV, cpDescriptor));
			m_DescriptorHandles.insert(std::pair <DESCRIPTOR_TYPE, D3D12_CPU_DESCRIPTOR_HANDLE>(DESCRIPTOR_TYPE::UAV, cpDescriptor->GetCPUDescriptorHandleForHeapStart()));
			D3D12_CPU_DESCRIPTOR_HANDLE uavHeapBegin = cpDescriptor->GetCPUDescriptorHandleForHeapStart();
			// Create Uav View
			D3D12_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
			uavDesc.Format = m_eDXTextureFormat;
			uavDesc.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE2D;
			_spDevice->GetDV()->CreateUnorderedAccessView(m_cpTexture.Get(), nullptr, &uavDesc, uavHeapBegin);

			m_eTextureDescriptorType = DESCRIPTOR_TYPE::UAV;
		}

		RETURN_CHECK_FAILED(CreateView(_spDevice), E_FAIL);
	}
	return S_OK;
}

const ComPtr<Dx12DescriptorHeap> UTexture::FindDescriptor(const DESCRIPTOR_TYPE _eDescriptor)
{
	const DESCRIPTORHEAPS::iterator& iter = m_DescriptorHeaps.find(_eDescriptor);
	RETURN_CHECK(iter == m_DescriptorHeaps.end(), nullptr);
	return iter->second;
}
