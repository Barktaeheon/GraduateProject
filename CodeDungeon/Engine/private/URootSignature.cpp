#include "EngineDefine.h"
#include "URootSignature.h"
#include "UDevice.h"
#include "UGpuCommand.h"

URootSignature::URootSignature() :
	m_iRegisterSpace{ 0 },
	m_cpRootSignature{ NULL },
	m_arrSamplerDescs{}
{
}

void URootSignature::Free()
{
}

HRESULT URootSignature::NativeConstruct(CSHPTRREF<UDevice> _spDevice, const _uint _iRegisterSpace)
{
	RETURN_CHECK(nullptr == _spDevice, E_FAIL);
	m_iRegisterSpace = _iRegisterSpace;
	NativeSampler();
	// GraphicRoot
	RETURN_CHECK_FAILED(CreateRootSignature(_spDevice), E_FAIL);
	return S_OK;
}

void URootSignature::NativeSampler()
{
	// Sampler
	{
		m_arrSamplerDescs[S0] = CD3DX12_STATIC_SAMPLER_DESC(S0);
		m_arrSamplerDescs[S1] = CD3DX12_STATIC_SAMPLER_DESC(S1, // ���÷� �������� ����
			D3D12_FILTER_MIN_MAG_MIP_LINEAR, // ���� ����
			D3D12_TEXTURE_ADDRESS_MODE_CLAMP, // U �� �ּ� ���
			D3D12_TEXTURE_ADDRESS_MODE_CLAMP, // V �� �ּ� ���
			D3D12_TEXTURE_ADDRESS_MODE_CLAMP  // W �� �ּ� ���
		);
	}

	for (auto& iter : m_arrSamplerDescs)
	{
		iter.RegisterSpace = GetRegisterSpace();
	}
}

HRESULT URootSignature::CreateRootSignature(CSHPTRREF<UDevice> _spDevice)
{
	{
		// B0 �� ���� ����
		ARRAY< CD3DX12_DESCRIPTOR_RANGE, 2> Ranges =
		{
			// Constant Buffer ���� �� ���̴�.  b0, b1 �����ϰ� ������ b2���ʹ� tabledescriptor�� ���ؼ� ���ε��ϰڴ�.
			CD3DX12_DESCRIPTOR_RANGE(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, CBV_REGISTER_END - GRAPHICS_CONSTANT_BUFFER_VALUE,
			GRAPHICS_CONSTANT_BUFFER_VALUE, GetRegisterSpace()),
			CD3DX12_DESCRIPTOR_RANGE(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, SRV_REGISTER_END, 0, GetRegisterSpace()),
		};

		// ConstantBuffer View�� �����, B0, B1 (����) (GPUCOmmand)
		const _uint PARAM_SIZE = 3;
		ARRAY<CD3DX12_ROOT_PARAMETER, PARAM_SIZE> Param = {};
		Param[0].InitAsConstantBufferView(static_cast<_uint>(CBV_REGISTER::B0));
		Param[1].InitAsConstantBufferView(static_cast<_uint>(CBV_REGISTER::B1));
		// DescriptorTable�� Range ��ŭ �����. Table
		Param[2].InitAsDescriptorTable(static_cast<_uint>(Ranges.size()), Ranges.data()); // 0�� -> b0 -> CBV

		for (auto& iter : Param)
		{
			iter.Constants.RegisterSpace = GetRegisterSpace();
			iter.Descriptor.RegisterSpace = GetRegisterSpace();
		}

		D3D12_ROOT_SIGNATURE_DESC tDesc = CD3DX12_ROOT_SIGNATURE_DESC(static_cast<_uint>(Param.size()), Param.data(),
			static_cast<_uint>(m_arrSamplerDescs.size()), m_arrSamplerDescs.data());

		// �Է� ������ �ܰ� ���
		tDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT; // �Է� ������ �ܰ�

		ComPtr<DxBlob> BlobSignature = nullptr;
		ComPtr<DxBlob> BlobError = nullptr;
		// RootSignature ���¸� �˸��� 
		RETURN_CHECK_DXOBJECT(::D3D12SerializeRootSignature(&tDesc, D3D_ROOT_SIGNATURE_VERSION_1,
			&BlobSignature, &BlobError), E_FAIL);
		RETURN_CHECK(nullptr != BlobError, E_FAIL);
		// CreateRootSignature 
		RETURN_CHECK_DXOBJECT(_spDevice->GetDV()->CreateRootSignature(0, BlobSignature->GetBufferPointer(),
			BlobSignature->GetBufferSize(), IID_PPV_ARGS(&m_cpRootSignature)), E_FAIL);
	}
	return S_OK;
}

HRESULT URootSignature::CreateComputeRootSignature(CSHPTRREF<UDevice> _spDevice)
{
	ARRAY< CD3DX12_DESCRIPTOR_RANGE, 3> Ranges =
	{
		CD3DX12_DESCRIPTOR_RANGE(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, CBV_REGISTER_END, 0, GetRegisterSpace()), // b0~b14
		CD3DX12_DESCRIPTOR_RANGE(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, SRV_REGISTER_END, 0, GetRegisterSpace()), // t0~t14
		CD3DX12_DESCRIPTOR_RANGE(D3D12_DESCRIPTOR_RANGE_TYPE_UAV, UAV_REGISTER_END, 0, GetRegisterSpace()), // u0~u14
	};
	// ConstantBuffer View�� �����
	const _uint PARAM_SIZE = 1;
	ARRAY<CD3DX12_ROOT_PARAMETER, PARAM_SIZE> Param = {};
	// DescriptorTable�� Range ��ŭ �����. 
	Param[0].InitAsDescriptorTable(static_cast<_uint>(Ranges.size()), Ranges.data()); // 0�� -> b0 -> CBV

	D3D12_ROOT_SIGNATURE_DESC tDesc = CD3DX12_ROOT_SIGNATURE_DESC(static_cast<_uint>(Param.size()), Param.data(),
		static_cast<_uint>(GetSamplers().size()), GetSamplers().data());

	tDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_NONE;

	ComPtr<DxBlob> BlobSignature = nullptr;
	ComPtr<DxBlob> BlobError = nullptr;
	// RootSignature ���¸� �˸��� 
	RETURN_CHECK_DXOBJECT(::D3D12SerializeRootSignature(&tDesc, D3D_ROOT_SIGNATURE_VERSION_1,
		&BlobSignature, &BlobError), E_FAIL);
	return Create(_spDevice, BlobSignature);
}

HRESULT URootSignature::Create(CSHPTRREF<UDevice> _spDevice, const ComPtr<DxBlob>& _spBlob)
{
	RETURN_CHECK_DXOBJECT(_spDevice->GetDV()->CreateRootSignature(0, _spBlob->GetBufferPointer(),
		_spBlob->GetBufferSize(), IID_PPV_ARGS(&m_cpRootSignature)), E_FAIL);

	return S_OK;
}
