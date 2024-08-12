#ifndef _SERVERFRAMEWORK_CORE_PUBLIC_ASERVICE_H
#define _SERVERFRAMEWORK_CORE_PUBLIC_ASERVICE_H

#include "ACoreObject.h"

BEGIN(Core)
class AMonster;
class ASession;
class AStaticObject;
/*
@ Date: 2024-01-06, Writer: ������
@ Explain
- PlayerSession ����, ���� �� AWS ���� ����
*/
class CORE_DLL AService abstract : public ACoreObject {
public:
	using SESSIONCONTAINER = CONUNORMAP<SESSIONID, SHPTR<ASession>>;
	using MOBOBJCONTAINER = CONUNORMAP<SESSIONID, SHPTR<AMonster>>;
	using STATICOBJCONTAINER = CONUNORMAP<SESSIONID, SHPTR<AStaticObject>>;
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
	SHPTR<AMonster> FindMobObject(const SESSIONID _SessionID);
	// 
	SHPTR<AStaticObject> FindStaticObject(const SESSIONID _SessionID);
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
	void InsertMobObject(SESSIONID _SessionID, SHPTR<AMonster> _spMobObject);
	// StaticObject 
	void InsertStaticObj(SESSIONID _SessionID, SHPTR<AStaticObject> _spStaticObj);
public: /* get set */
	const SESSIONCONTAINER& GetSessionContainer() const { return m_SessionContainer; }
	const MOBOBJCONTAINER& GetMobObjContainer() const { return m_MobObjContainer; }
	const STATICOBJCONTAINER& GetStaticObjContainer() const { return m_StaticObjContainer; }
	IOContext& GetIOContext(REF_RETURN) { return m_IOContext; }

	void IncreaseCurrentSessionCount(_llong _Count);
protected:
	virtual void Connect() PURE;
	virtual SESSIONID GiveID();
protected: /* Get Set */
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
	MOBOBJCONTAINER			m_MobObjContainer;
	STATICOBJCONTAINER		m_StaticObjContainer;
};

END

#endif // _SERVERFRAMEWORK_CORE_PUBLIC_ASERVICE_H