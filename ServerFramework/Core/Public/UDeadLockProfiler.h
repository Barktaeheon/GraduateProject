#ifndef _SERVERFRAMEWORK_CORE_PUBLIC_UDEADLOCKPROFILER
#define _SERVERFRAMEWORK_CORE_PUBLIC_UDEADLOCKPROFILER

#include "UBase.h"
#include <stack>

BEGIN(Core)

/*
@ Date: 2023-12-29
@ Writer: ������
@ Explain: DeadLock�� �����ϱ� ���� �������Ϸ�
*/
class UDeadLockProfiler final : public UBase {
public:
	UDeadLockProfiler();
	NO_COPY(UDeadLockProfiler)
	DESTRUCTOR(UDeadLockProfiler)

public:
	/*
	@ Date: 2023-12-29
	@ Writer: ������
	*/
	void PushLock(const char* _DeadLockLog);
	/*
	@ Date: 2023-12-29
	@ Writer: ������
	*/
	void PopLock(const char* _DeadLockLog);
	/*
	@ Date: 2023-12-29
	@ Writer: ������
	*/
	void CheckCycle();
private:
	/*
	@ Date: 2023-12-29
	@ Writer: ������
	*/
	void Dfs(const _long _Here);

private:
	virtual void Free() override;

private:
	CONUNORMAP<const char*, _long>			m_NameTold;
	CONUNORMAP<_long, const char*>			m_IdToName;
	static thread_local	std::stack<_long>		s_LockStack;
	CONMAP<_long, CONSET<_long>>				m_LockHistory;

	MUTEX																m_Mutex;

	CONVECTOR<_long>										m_DiscoveredOrder; // ��尡 �߰ߵ� ������ ����ϴ� �迭
	std::atomic<_long>											m_DiscoveredCount; // ��尡 �߰ߵ� ����
	CONVECTOR<_bool>										m_Finished; // Dfs�� ����Ǿ����� ����
	CONVECTOR<_long>										m_Parent; 
};

END

#endif // _SERVERFRAMEWORK_CORE_PUBLIC_UDEADLOCKPROFILER