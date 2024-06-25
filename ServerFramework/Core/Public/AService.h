#ifndef _SERVERFRAMEWORK_CORE_PUBLIC_ASERVICE_H
#define _SERVERFRAMEWORK_CORE_PUBLIC_ASERVICE_H

#include "ACoreObject.h"

BEGIN(Core)
class AGameObject;
class ASession;
/*
@ Date: 2024-01-06, Writer: ������
@ Explain
- PlayerSession ����, ���� �� AWS ���� ����
*/
class CORE_DLL AService abstract : public ACoreObject {
public:
	using SESSIONCONTAINER = CONUNORMAP<SESSIONID, SHPTR<ASession>>;
	using GAMEOBJECTCONTAINER = CONUNORMAP<SESSIONID, SHPTR<AGameObject>>;
public:
	AService(OBJCON_CONSTRUCTOR, SERVICETYPE _Type);
	DESTRUCTOR(AService)
public:
	virtual _bool NativeConstruct() PURE;
	// �ش� �Լ��� 
	virtual _bool Start() PURE;
public:
	// ID�� ���ؼ� SessionID
	SHPTR<ASession> FindSession(const SESSIONID _SessionID) ;
	// ID�� ���ؼ� GameObject�� ã�ƿ´�. 
	SHPTR<AGameObject> FindGameObject(const SESSIONID _SessionID);
	// ��ü ���� �����ڿ��� �޽����� ������ �Լ��̴�. 
	void BroadCastMessage(_char* _pPacket, const PACKETHEAD& _PacketHead) ;
	// �ش� Session ID�� ������ ��ü ���� �����ڿ��� �޽����� ������ �Լ�
	void BroadCastMessageExcludingSession(const SESSIONID _SessionID, _char* _pPacket, const PACKETHEAD& _PacketHead) ;
	// Direct�� �޽��� ������ �Լ�
	void DirectSendMessage(const SESSIONID _SessionID, _char* _pPacket, const PACKETHEAD& _PacketHead) ;
	// Session�� TCP Socket ���Ḹ ���� Insert�� ��Ÿ �ٸ� �Լ����� �����ؼ� Socket�� ����
	void LeaveService(const SESSIONID _SessionID) ;
	// Session�� Container�� �����ϴ� �Լ��̴�. 
	void InsertSession(SESSIONID _SessionID, SHPTR<ASession> _spSession) ;
	// GameObject�� ����ִ� �Լ�
	void InsertGameObject(SESSIONID _SessionID, SHPTR<AGameObject> _spGameObject);
public: /* get set */
	const SESSIONCONTAINER& GetSessionContainer() const { return m_SessionContainer; }
	const GAMEOBJECTCONTAINER& GetGameObjectContainer() const { return m_GameObjectContainer; }
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
	SERVICETYPE						m_ServiceType;
	ATOMIC<SESSIONID>			m_IDIssuance;
	ATOMIC<_llong>					m_CurrentSessionCount;
	// Boost/Asio Context
	IOContext								m_IOContext;
	TCPSOCKET							m_TcpSocket;
	// Lock
	MUTEX									m_Lock;
	// Session Conatiner
	SESSIONCONTAINER			m_SessionContainer;
	GAMEOBJECTCONTAINER	m_GameObjectContainer;
};

END

#endif // _SERVERFRAMEWORK_CORE_PUBLIC_ASERVICE_H