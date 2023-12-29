#include "CoreDefines.h"
#include "UDeadLockProfiler.h"

BEGIN(Core)

 thread_local	std::stack<_long>		UDeadLockProfiler::s_LockStack;

UDeadLockProfiler::UDeadLockProfiler()
{
}

void UDeadLockProfiler::PushLock(const char* _DeadLockLog)
{
	// ID�� ã�ų� �߱��Ѵ�. 
	_long	LockID = 0;
	auto findIt = m_NameTold.find(_DeadLockLog);
	if (findIt == m_NameTold.end())
	{
		LockID = static_cast<_long>(m_NameTold.size());
		m_NameTold[_DeadLockLog] = LockID;
		m_IdToName[LockID] = _DeadLockLog;
	}
	else
	{
		LockID = findIt->second;
	}

	// ��� �ִ� ���� �־��ٸ�
	if (false == s_LockStack.empty())
	{
		// ���ؿ� �߰ߵ��� ���� ���̽���� ����� ���θ� Ȯ���Ѵ�. 
		const _long PrevID = s_LockStack.top();
		if (LockID != PrevID)
		{
			CONSET<_long>& History = m_LockHistory[PrevID];
			if (History.find(LockID) == History.end())
			{
				History.insert(LockID);
				CheckCycle();
			}
		}
	}
	s_LockStack.push(LockID);
}

void UDeadLockProfiler::PopLock(const char* _DeadLockLog)
{
	if (s_LockStack.empty())
		CRASH("MULTIPLE_UNLOCK");

	_long LockID = m_NameTold[_DeadLockLog];
	if (s_LockStack.top() != LockID)
		CRASH("INVALID_UNLOCK");

	s_LockStack.pop();
}

void UDeadLockProfiler::CheckCycle()
{
	const _long LockCount = static_cast<_long>(m_NameTold.size());
	m_DiscoveredCount = 0;
	// �Ҵ� 
	{
		LOCKGUARD<MUTEX> Lock(m_Mutex);
		m_DiscoveredOrder = CONVECTOR<_long>(LockCount, -1);
		m_Finished = CONVECTOR<bool>(LockCount, false);
		m_Parent = CONVECTOR<_long>(LockCount, -1);
	}

	for (_long LockID = 0; LockID < LockCount; ++LockID)
	{
		Dfs(LockID);
	}

	// �������ش�. 
	{
		LOCKGUARD<MUTEX> Lock(m_Mutex);
		m_DiscoveredOrder.clear();
		m_Finished.clear();
		m_Parent.clear();
	}
}

void UDeadLockProfiler::Dfs(const _long _Here)
{
	if (m_DiscoveredOrder[_Here] != -1)
	{
		return;
	}

	m_DiscoveredOrder[_Here] = m_DiscoveredCount++;
	auto FindIt = m_LockHistory.find(_Here);
	if (FindIt == m_LockHistory.end())
	{
		m_Finished[_Here] = true;
		return;
	}

	CONSET<_long>& NextSet = FindIt->second;
	for (_long There : NextSet)
	{
		// ���� �湮������ ������ �湮 
		if (m_DiscoveredOrder[There] == -1)
		{
			m_Parent[There] = _Here;
			Dfs(There);
			continue;
		}

		// Here�� There���� ���� �߰ߵǾ��ٸ�, There�� Here�� �ļ��̴� (������ ����)
		if (m_DiscoveredOrder[_Here] < m_DiscoveredOrder[There])
			continue;

		// �������� �ƴϰ� DFS(There)�� ���� ������� �ʾҴٸ�, There�� Here�� �����̴� (������ ����)
		if (m_Finished[There] == false)
		{
			std::printf("%s -> %s", m_IdToName[_Here], m_IdToName[There]);

			_long Now = _Here;
			while (true)
			{
				std::printf("%s -> %s\n", m_IdToName[m_Parent[Now]], m_IdToName[There]);
				Now = m_Parent[Now];
				if (Now == There)
					break;
			}

			CRASH("DeadLock_Detected");
		}
	}

	m_Finished[_Here] = true;
}

void UDeadLockProfiler::Free()
{
}


END