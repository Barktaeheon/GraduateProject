#ifndef _SERVERFRAMEWORK_SERVER_PUBLIC_CSERVERAPP
#define _SERVERFRAMEWORK_SERVER_PUBLIC_CSERVERAPP

#include "UBase.h"

BEGIN(Server)
/*
@ Date: 2023-12-26
@ Writer: ������
@ Explain:  ServerApp�� �����ϴ� Ŭ����
*/
class CServerApp : public Core::UBase {
public:
	CServerApp();
	
private:
	virtual void Free() override;
};

END

#endif // _SERVERFRAMEWORK_SERVER_PUBLIC_CSERVERAPP