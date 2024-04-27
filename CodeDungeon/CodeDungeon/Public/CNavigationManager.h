#pragma once
#include "UBase.h"

BEGIN(Engine)
class UGameInstance;
class UDevice;
class UCommand;
END
/*
@ Date: 2024-04-23, Writer: �̼���
@ Explain
- �׺���̼��� �о �ҷ����� �Ŵ��� ��ü
*/

BEGIN(Client)

class CNavigationManager : public UBase
{
public:
	CNavigationManager();
	NO_COPY(CNavigationManager)
	DESTRUCTOR(CNavigationManager)

	// UBase��(��) ���� ��ӵ�
	void Free() override;
public:
	HRESULT LoadNavigation();

};

END