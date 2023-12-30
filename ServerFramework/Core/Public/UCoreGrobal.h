#ifndef _SERVERFRAMEWORK_CORE_PUBLIC_UCOREGROBAL_H
#define _SERVERFRAMEWORK_CORE_PUBLIC_UCOREGROBAL_H

BEGIN(Core)
class UThreadManager;
class UDeadLockProfiler;
/*
@ Date: 2023-12-26
@ Writer: ������
@ Explain
- ����, �Ҹ��ڸ� �̿��ؼ� ������ ����� ��, �޸𸮰� ����� �����ǰ� �ϱ� ���� �۷ι� Ŭ�����̴�. 
*/
class CORE_DLL UCoreGrobal {
public:
	UCoreGrobal();
	NO_COPY(UCoreGrobal)
	~UCoreGrobal();
public: /* CoreGrobal */
	HRESULT ReadyCoreGrobal();
public: /* ThreadManager */
	void RegisterFunc(const THREADFUNC& _CallBack, void* _Data);
	void Join();

public: /* DeadLockProfiler*/
	void PushLock(const char* _DeadLockLog);
	void PopLock(const char* _DeadLockLog);
	void CheckCycle();

private:
	MUTEX											m_Mutex;
	IOContext*										m_pContext;
	SHPTR<UThreadManager>		m_spThreadManager;
	SHPTR< UDeadLockProfiler>		m_spDeadLockProfiler;
};

CORE_DLL extern		UCoreGrobal e_CoreGrobal;
END

#define GGROBAL Core::e_CoreGrobal

#endif 

