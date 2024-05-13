#pragma once

#include "UBase.h"

/*
@ Date: 2024-04-24, Writer: �̼���
@ Explain
- ��, �׺���̼� �� �����͸� �����ϰ� �ε��ϴ� �Ŵ��� ��ü
*/
BEGIN(ENGINE)
class UStageManager;
END
BEGIN(Client)

class CModelManager;

class CDataManager final : public UBase
{
public:
	CDataManager();
	NO_COPY(CDataManager)
	DESTRUCTOR(CDataManager)

	// UBase��(��) ���� ��ӵ�
	void Free() override;
public:
	HRESULT NativeConstruct();
	HRESULT Load_Data();

private:
	SHPTR<CModelManager>		m_spModelManager;
	SHPTR<UStageManager>		m_spStageManager;
};

END