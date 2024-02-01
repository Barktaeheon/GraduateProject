#ifndef _SERVERFRAMEWORK_CORE_PUBLIC_ACLIENTSERVICE_H
#define _SERVERFRAMEWORK_CORE_PUBLIC_ACLIENTSERVICE_H

#include "AService.h"

BEGIN(Core)
/*
@ Date: 2024-01-05, Writer: ������
@ Explain
-  ClientService�� �������ؼ� ����Ѵ�.  Client Framework���� ������ 
*/
class CORE_DLL AClientService abstract : public AService {
public: /* USING */
	using SESSIONCONTAINER = CONHASHMAP<SESSIONID, SHPTR<ASession>>;
public:
	AClientService(OBJCON_CONSTRUCTOR);
	DESTRUCTOR(AClientService)
public:
	virtual _bool NativeConstruct() PURE;
	virtual _bool Start() override;
public:
	virtual SHPTR<ASession> FindSession(const SESSIONID _SessionID) override;
	virtual void BroadCastMessage(_char* _pPacket, const PACKETHEAD& _PacketHead) override;
	virtual void LeaveService(const SESSIONID _SessionID) override;
	virtual void InsertSession(SESSIONID _SessionID, SHPTR<ASession> _spSession) override;

	static void RunningThread(void* _pService);

	_bool IsRunningThread() const { return m_isRunningThread; }
	void StopThreadRunning() { m_isRunningThread.store(false); }
protected:
	virtual void Tick() PURE;
	virtual void Connect() PURE;
	virtual SESSIONID GiveID() override;
	// ������ ������ ���ϵ��� �����Ͽ� �޸� Ȯ��
	void RemoveDisconnectSockets();
protected: /* get set */
	SESSIONCONTAINER& GetSessionContainer() { return m_SessionContainer; }
private:
	virtual void Free() PURE;
private:
	ATOMIC<_bool>							m_isRunningThread;
	// Session Conatiner
	SESSIONCONTAINER					m_SessionContainer;
	CONQUEUE<SESSIONID>			m_RemainIDQueue;
};

END

#endif // _SERVERFRAMEWORK_CORE_PUBLIC_ACLIENTSERVICE_H