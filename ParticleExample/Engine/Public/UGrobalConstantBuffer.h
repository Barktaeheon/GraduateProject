#ifndef _PTH_FRAMEWORK_ENGIEN_PUBLIC_UGROBALCONSTANTBUFFER_H
#define _PTH_FRAMEWORK_ENGIEN_PUBLIC_UGROBALCONSTANTBUFFER_H

#include "UBase.h"

BEGIN(Engine)
class UDevice;
class UGpuCommand;

/*
모든 쉐이더마다 공통으로 쓰이는 객체
무조건 하나만 생성되야 한다. 
Engine에서만 생성한다. 
*/

class DEF_CACHE_ALGIN UGrobalConstantBuffer final : public UBase {
public:
	UGrobalConstantBuffer();
	NO_COPY(UGrobalConstantBuffer)
	DESTRUCTOR(UGrobalConstantBuffer)
public:
	virtual void Free() override;
	HRESULT NativeConstruct(CSHPTRREF<UDevice> _spDevice, 	const CBV_REGISTER& _eReg, const _uint& _iSize);

	void SettingGrobalData(CSHPTRREF<UCommand> _spCommand, const void* _pBuffer, const _uint _iSize);
private:
	// Use Upload
	ComPtr<Dx12Resource>									m_cpUploadBuffer;
	// Descriptor Heap
	ComPtr<Dx12DescriptorHeap>						m_cpCpuDescriptorHeap;
	D3D12_CPU_DESCRIPTOR_HANDLE				m_cpCpuDescriptorHandle;
	BYTE*																		m_pMapBuffer;
	_uint																		m_iElementSize;
	// CBV_REGISTER
	_uint																		m_iCbvRegisterNumber;
};

END

#endif // _PTH_FRAMEWORK_ENGIEN_PUBLIC_UGROBALCONSTANTBUFFER_H