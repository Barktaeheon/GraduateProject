#include "EngineDefines.h"
#include "ULightRootSignature.h"

ULightRootSignature::ULightRootSignature()
{
}

void ULightRootSignature::Free()
{
}

HRESULT ULightRootSignature::CreateRootSignature(CSHPTRREF<UDevice> _spDevice)
{
	{
		// B0 �� ���� ����
		ARRAY< CD3DX12_DESCRIPTOR_RANGE, 2> Ranges{};
		Ranges[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, CBV_REGISTER_END - LIGHT_CONSTANT_BUFFER_VALUE,
			LIGHT_CONSTANT_BUFFER_VALUE, GetRegisterSpace());
		Ranges[1].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, SRV_REGISTER_END, 0, GetRegisterSpace());

		// ConstantBuffer View�� �����
		const _uint PARAM_SIZE = 2;
		ARRAY<CD3DX12_ROOT_PARAMETER, PARAM_SIZE> Param = {};
		Param[0].InitAsConstantBufferView(static_cast<_uint>(CBV_REGISTER::B0), GetRegisterSpace());
		// DescriptorTable�� Range ��ŭ �����. 
		Param[1].InitAsDescriptorTable(static_cast<_uint>(Ranges.size()), Ranges.data()); // 0�� -> b0 -> CBV
		Param[1].Descriptor.RegisterSpace = GetRegisterSpace();


		D3D12_ROOT_SIGNATURE_DESC tDesc = CD3DX12_ROOT_SIGNATURE_DESC(static_cast<_uint>(Param.size()), Param.data(),
			static_cast<_uint>(GetSamplers().size()), GetSamplers().data());

		// �Է� ������ �ܰ� ���
		tDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT; // �Է� ������ �ܰ�

		ComPtr<DxBlob> BlobSignature = nullptr;
		ComPtr<DxBlob> BlobError = nullptr;
		// RootSignature ���¸� �˸��� 
		RETURN_CHECK_DXOBJECT(::D3D12SerializeRootSignature(&tDesc, D3D_ROOT_SIGNATURE_VERSION_1,
			&BlobSignature, &BlobError), E_FAIL);
		RETURN_CHECK(nullptr != BlobError, E_FAIL);
		// CreateRootSignature 
		RETURN_CHECK_FAILED(Create(_spDevice, BlobSignature), E_FAIL);
	}
	return S_OK;
}
