#ifndef _SERVERFRAMEWORK_CORE_PUBLIC_ASERVERSERVICE_H
#define _SERVERFRAMEWORK_CORE_PUBLIC_ASERVERSERVICE_H

#include "AService.h"

BEGIN(Core)
class ANavigation;
/*
@ Date: 2024-01-05, Writer: ������
@ Explain
-  ServerService�� �������ؼ� ����Ѵ�.  Server Framework���� ������ �ؼ� ����ؾ��Ѵ�. 
*/
class CORE_DLL UServerService abstract  : public AService {
public: /* USING */
	using SESSIONCONTAINER = CONUNORMAP<SESSIONID, SHPTR<ASession>>;
public: 
	UServerService(OBJCON_CONSTRUCTOR, const _string& _strNavigationPath = "");
	DESTRUCTOR(UServerService)
public:
	virtual _bool NativeConstruct() PURE;
	virtual _bool Start() override;
public:
	/* AWS */
	virtual SHPTR<ASession> FindSession(const SESSIONID _SessionID) override;
	virtual void BroadCastMessage(_char* _pPacket, const PACKETHEAD& _PacketHead) override;
	// ���⼭�� Session ��ü�� ����
	virtual void LeaveService(const SESSIONID _SessionID) override;
	virtual void InsertSession(SESSIONID _SessionID, SHPTR<ASession> _spSession) override;
protected:
	// Thread�� ������� �Լ�
	static void ThreadFunc(void* _spService);
protected: /* get set */
	TCPACCEPTOR& GetTcpAccepctor(REF_RETURN) { return m_TcpAcceptor; }
private:
	virtual void Free() PURE;
private:
	// Tcps
	TCPACCEPTOR							m_TcpAcceptor;
	// Session Conatiner
	SESSIONCONTAINER				m_SessionContainer;
	// Navigation
	SHPTR<ANavigation>				m_spNavigation;
};

END

#endif // _SERVERFRAMEWORK_CORE_PUBLIC_ASERVERSERVICE_H