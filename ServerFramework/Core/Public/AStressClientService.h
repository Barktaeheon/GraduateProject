#ifndef _SERVERFRAMEWORK_CORE_PUBLIC_ASTRESSCLIENTSERVICE_H
#define _SERVERFRAMEWORK_CORE_PUBLIC_ASTRESSCLIENTSERVICE_H

#include "AService.h"

BEGIN(Core)
/*
@ Date: 2024-01-31, Writer: ������
@ Explain
-  StressClient���� ����� ���񽺸� ������ 
*/
class CORE_DLL AStressClientService abstract : public AService {
public: /* USING */
	using SESSIONCONTAINER = CONHASHMAP<SESSIONID, SHPTR<ASession>>;
public:
	AStressClientService(OBJCON_CONSTRUCTOR);
	DESTRUCTOR(AStressClientService)
public:
	virtual _bool NativeConstruct() PURE;
	virtual _bool Start() override;
public:
	static void RunningThread(void* _pService);

	_bool IsRunningThread() const { return m_isRunningThread; }
	void StopThreadRunning() { m_isRunningThread.store(false); }
protected:
	virtual void Tick() PURE;
	virtual void Connect() PURE;
	virtual SESSIONID GiveID() override;
	// ������ ������ ���ϵ��� �����Ͽ� �޸� Ȯ��
	void RemoveDisconnectSockets();
private:
	virtual void Free() PURE;
private:
	ATOMIC<_bool>							m_isRunningThread;
	CONQUEUE<SESSIONID>			m_RemainIDQueue;
};

END

#endif // _SERVERFRAMEWORK_CORE_PUBLIC_ASTRESSCLIENTSERVICE_H