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
- ���� �о �ҷ����� �Ŵ��� ��ü
*/

BEGIN(Client)

class CModelManager : public UBase
{
public:
	CModelManager();
	NO_COPY(CModelManager)
	DESTRUCTOR(CModelManager)

	// UBase��(��) ���� ��ӵ�
	void Free() override;
public:
	HRESULT CreateModelProtos(CSHPTRREF<UGameInstance> _spGameInstance, CSHPTRREF<UDevice> _spDevice);
	HRESULT AddModelProtosFromFile(CSHPTRREF<UGameInstance> _spGameInstance, CSHPTRREF<UDevice> _spDevice, CSHPTRREF<FILEGROUP> _spFolder);


};

END