#pragma once 

#include "UBase.h"
#include "UProcessedData.h"

BEGIN(Engine)
class UNetworkBaseController;

/*
@ Date: 2024-02-03,  Writer: ������
@ Explain
- Network Data���� ������ ó���ϱ� ���� Manager
*/
class UNetworkManager final : public UBase {
public:
	UNetworkManager();
	NO_COPY(UNetworkManager)
	DESTRUCTOR(UNetworkManager)
public:
	HRESULT StartNetwork(CSHPTRREF<UNetworkBaseController> _spNetworkBaseController);
	void InsertProcessedDataToContainer(void* _pData, size_t _Size, _int _DataType);
	void PopProcessedData(POINTER_IN UProcessedData* _pData);
private:
	virtual void Free() override;
private:
	SHPTR<UNetworkBaseController>		m_spNetworkBaseController;
	CONQUEUE<UProcessedData>				m_ProcessedDataContainer;
};

END

