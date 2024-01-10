#ifndef _SERVERFRAMEWORK_SERVER_PUBLIC_CSERVERADIMINSTOR_H
#define _SERVERFRAMEWORK_SERVER_PUBLIC_CSERVERADIMINSTOR_H

#include "UServerService.h"

BEGIN(Server)
class  CAwsServerConnector;

/*
@ Date: 2023-01-02, Writer: ������
@ Explain
- Server�� �����ϴ� ������ Ŭ�����̴�. �÷��̾� ���ǵ��� �Ѱ�
*/
class CACHE_ALGIN CServerAdiminstor final : public Core::UServerService{
public:
	CServerAdiminstor(OBJCON_CONSTRUCTOR, _bool _CreateAwsConnector = false);
	NO_COPY(CServerAdiminstor)
	DESTRUCTOR(CServerAdiminstor)
public:
	virtual bool NativeConstruct() override;
public: /*  Get Set*/
	SHPTR<CAwsServerConnector> GetAwsConnector() const { return m_spAwsConnector; }
protected:
	// UServerService��(��) ���� ��ӵ�
	virtual void Connect() override;
private:
	void Free() override;

private:
	/* Connector */
	SHPTR<CAwsServerConnector>		m_spAwsConnector;
};

END

#endif // _SERVERFRAMEWORK_SERVER_PUBLIC_CSERVERADIMINSTOR_H
