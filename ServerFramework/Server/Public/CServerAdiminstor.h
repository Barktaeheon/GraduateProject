#ifndef _SERVERFRAMEWORK_SERVER_PUBLIC_CSERVERADIMINSTOR_H
#define _SERVERFRAMEWORK_SERVER_PUBLIC_CSERVERADIMINSTOR_H

#include "UServerService.h"

BEGIN(Server)

/*
@ Date: 2023-01-02, Writer: ������
@ Explain
- Server�� �����ϴ� ������ Ŭ�����̴�. �÷��̾� ���ǵ��� �Ѱ�
*/
class CACHE_ALGIN CServerAdiminstor final : public Core::UServerService{
public:
	CServerAdiminstor(OBJCON_CONSTRUCTOR);
	NO_COPY(CServerAdiminstor)
	DESTRUCTOR(CServerAdiminstor)
protected:
	// UServerService��(��) ���� ��ӵ�
	void AsyncAccept() override;
private:
	void Free() override;
};

END

#endif // _SERVERFRAMEWORK_SERVER_PUBLIC_CSERVERADIMINSTOR_H
