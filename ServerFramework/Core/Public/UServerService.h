#ifndef _SERVERFRAMEWORK_CORE_PUBLIC_USERVERSERVICE_H
#define _SERVERFRAMEWORK_CORE_PUBLIC_USERVERSERVICE_H

#include "UService.h"

BEGIN(Core)
/* Aws Connector */
class UAwsConnector;

/*
@ Date: 2024-01-05, Writer: ������
@ Explain
-  ServerService�� �������ؼ� ����Ѵ�.  Server Framework���� ������ �ؼ� ����ؾ��Ѵ�. 
*/
class CACHE_ALGIN_CORE_DLL UServerService abstract  : public UService {
public: /* USING */
	using SESSIONCONTAINER = CONUNORMAP<SESSIONID, SHPTR<USession>>;
public: 
	UServerService(OBJCON_CONSTRUCTOR);
	DESTRUCTOR(UServerService)
public:
	virtual _bool NativeConstruct() PURE;
	virtual _bool Start() override;
public:
	/* AWS */
	virtual SHPTR<USession> FindSession(const SESSIONID _SessionID) override;
	virtual void BroadCastMessage(_char* _pPacket, const PACKETHEAD& _PacketHead) override;
	// ���⼭�� Session ��ü�� ����
	virtual void LeaveService(const SESSIONID _SessionID) override;
	virtual void InsertSession(SESSIONID _SessionID, SHPTR<USession> _spSession) override;
protected:
	// Thread�� ������� �Լ�
	static void ThreadFunc(void* _spService);
protected: /* get set */
	TCPACCEPTOR& GetTcpAccepctor(REF_RETURN) { return m_TcpAcceptor; }
private:
	virtual void Free() PURE;
private:
	// Session Conatiner
	SESSIONCONTAINER				m_SessionContainer;
	// Tcps
	TCPACCEPTOR							m_TcpAcceptor;
};

END

#endif // _SERVERFRAMEWORK_CORE_PUBLIC_USERVERSERVICE_H