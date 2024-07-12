#ifndef _SERVERFRAMEWORK_SERVER_PUBLIC_CSERVERADIMINSTOR_H
#define _SERVERFRAMEWORK_SERVER_PUBLIC_CSERVERADIMINSTOR_H

#include "AServerService.h"

BEGIN(Core)
class AJobTimer;

END

BEGIN(Server)
/*
@ Date: 2023-01-02, Writer: ������
@ Explain
- Server�� �����ϴ� ������ Ŭ�����̴�. �÷��̾� ���ǵ��� �Ѱ�
*/
class  CServerAdiminstor final : public Core::AServerService{
public:
	CServerAdiminstor(OBJCON_CONSTRUCTOR, const _string& _strNavigationPath = "");
	NO_COPY(CServerAdiminstor)
	DESTRUCTOR(CServerAdiminstor)
public:
	virtual bool NativeConstruct() override;
	virtual bool Start() override;
protected:
	// UServerService��(��) ���� ��ӵ�
	virtual void Connect() override;
private:
	void CreateMummyAndSarphagousMob(void* _pData, SARCOPHAGUSTYPE _SarcophagusType, SHPTR< AJobTimer> _spMonsterJobTimer);
	void CreateChest(void* _pData, SHPTR<AJobTimer> _spMonsterJobTimer);
private:
	void Free() override;
};

END

#endif // _SERVERFRAMEWORK_SERVER_PUBLIC_CSERVERADIMINSTOR_H
