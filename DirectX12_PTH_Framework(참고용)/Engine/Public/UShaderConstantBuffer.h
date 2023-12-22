#ifndef _PTH_FRAMEWORK_ENGIEN_PUBLIC_USHADERCONSTANTBUFFER_H
#define _PTH_FRAMEWORK_ENGIEN_PUBLIC_USHADERCONSTANTBUFFER_H
#include "UBase.h"

BEGIN(Engine)
class UDevice;
class UGpuCommand;
class UTableDescriptor;

/*
�� ���̴����� ���� �޶�� �ϴ� Buffer���� ShaderFile�� Bind�ϱ� ���� Ŭ������
�� ���̴��� �����ϱ� ���ؼ��� �ݵ�� �ش� ��ü�� �� �� �����ϰų� ������ ���� ���� �̿��Ѵ�.

���� ����ϴ� ������ _iNum ���� 1�̻� �־� �̸� �Ҵ� �� ���� Buffer�� ����ϰ� 
���� ������� �ʰ� ���� �Ҵ� ������ �ʴ� �͵��� �׶� �׶����� ShaderConstantBuffer�� �Ҵ��Ѵ�. 
*/
class ENGINE_DLL DEF_CACHE_ALGIN UShaderConstantBuffer final : public UBase {
public:
	UShaderConstantBuffer();
	NO_COPY(UShaderConstantBuffer)
	DESTRUCTOR(UShaderConstantBuffer)
public:
	virtual void Free() override;
	HRESULT NativeConstruct(CSHPTRREF<UDevice> _spDevice, 	const CBV_REGISTER& _eReg,   const _uint& _iSize, const _uint& _iNum = 1);
	// Shader�� ���� �о� �ִ� �Լ��̴�. 
	HRESULT PushData(CSHPTRREF< UTableDescriptor> _spTableDescriptor, const void* _pBuffer, const _uint& _iSize);
private:
	D3D12_CPU_DESCRIPTOR_HANDLE GetCpuDescriptorHandle(const _uint& _iIndex);
private:
	// Use Upload
	ComPtr<Dx12Resource>						m_cpUploadBuffer;
	// Descriptor Heap
	ComPtr<Dx12DescriptorHeap>			m_cpCpuDescriptorHeap;
	D3D12_CPU_DESCRIPTOR_HANDLE	m_stD3DCpuDescriptorHandle;
	_int																m_iHandleIncrementSize;

	BYTE*															m_pMapBuffer;
	_uint															m_iElementSize;
	// CBV_REGISTER
	CBV_REGISTER										m_eCbvRegisterNumber;
	// elementIndex
	_uint															m_iShaderConstantBufferIndex;
	_int																m_iShaderConstantBufferCpuLocation;
	_uint															m_iElementNum;
};	

END

#endif // _PTH_FRAMEWORK_ENGIEN_PUBLIC_USHADERCONSTANTBUFFER_H