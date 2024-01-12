#ifndef _SERVERFRAMEWORK_CORE_PUBLIC_USERVICE_H
#define _SERVERFRAMEWORK_CORE_PUBLIC_USERVICE_H

#include "UObject.h"

BEGIN(Core)
class USession;
/*
@ Date: 2024-01-06, Writer: ������
@ Explain
- PlayerSession ����, ���� �� AWS ���� ����
*/
class CACHE_ALGIN_CORE_DLL UService abstract : public UObject {
public:
	UService(OBJCON_CONSTRUCTOR, SERVICETYPE _Type);
	DESTRUCTOR(UService)
public:
	virtual _bool NativeConstruct() PURE;
	// �ش� �Լ��� 
	virtual _bool Start() PURE;
public:
	// ID�� ���ؼ� SessionID
	virtual SHPTR<USession> FindSession(const SESSIONID _SessionID) PURE;
	// ��ü ���� �����ڿ��� �޽����� ������ �Լ��̴�. 
	virtual void BroadCastMessage(_char* _pPacket, const PACKETHEAD& _PacketHead) PURE;
	// Session�� TCP Socket ���Ḹ ���� Insert�� ��Ÿ �ٸ� �Լ����� �����ؼ� Socket�� ����
	virtual void LeaveService(const SESSIONID _SessionID) PURE;
	// Session�� Container�� �����ϴ� �Լ��̴�. 
	virtual void InsertSession(SESSIONID _SessionID, SHPTR<USession> _spSession) PURE;
protected:
	virtual void Connect() PURE;
	virtual SESSIONID GiveID();
protected: /* Get Set */
	IOContext& GetIOContext(REF_RETURN) { return m_IOContext; }
	TCPSOCKET& GetTcpSocket(REF_RETURN) { return m_TcpSocket; }
	MUTEX& GetLock(REF_RETURN) { return m_Lock; }
	IOContext* GetIOConectPointer() { return &m_IOContext; }
private:
	virtual void Free() override;
private:
	SERVICETYPE					m_ServiceType;
	ATOMIC<SESSIONID>		m_IDIssuance;
	ATOMIC<_llong>				m_CurrentSessionCount;
	// Boost/Asio Context
	IOContext							m_IOContext;
	TCPSOCKET						m_TcpSocket;
	// Lock
	MUTEX								m_Lock;
};

END

#endif // _SERVERFRAMEWORK_CORE_PUBLIC_USERVICE_H