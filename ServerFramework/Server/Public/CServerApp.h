#ifndef _SERVERFRAMEWORK_SERVER_PUBLIC_CSERVERAPP_H
#define _SERVERFRAMEWORK_SERVER_PUBLIC_CSERVERAPP_H

#include "ACoreApp.h"

BEGIN(Server)
class CServerAdiminstor;
/*
@ Date: 2023-12-26, Writer: ������
@ Explain
- ServerApp�� �����ϴ� Ŭ����
*/
class CServerApp : public Core::ACoreApp {
public:
	CServerApp();
	NO_COPY(CServerApp)
public:
	virtual _bool Start() override;
private:
};

END

#endif // _SERVERFRAMEWORK_SERVER_PUBLIC_CSERVERAPP_H