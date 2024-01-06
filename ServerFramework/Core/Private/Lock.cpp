#include "CoreDefines.h"
#include "Lock.h"
#include "UDeadLockProfiler.h"

namespace Core
{
	/*
	@ Date: 2023-12-27
	@ Writer: ������
	@	 Explain
	- �ƹ��� �������� ���� ��� Write Lock�� �ɾ �ڽ��� ���ϴ� ���� ����Ǳ⸦ �ٶ���.
	*/
	URWLock::URWLock() : m_lockFlag{ 0 }, m_WriteCount{ 0 } {
	}
	URWLock::URWLock(const URWLock& _rhs)
	{
		m_lockFlag.store(_rhs.m_lockFlag.load());
		m_WriteCount.store(_rhs.m_WriteCount.load());
	}
	URWLock::~URWLock()
	{
	}
	URWLock& URWLock::operator=(const URWLock& _lock)
	{
		m_lockFlag.store(_lock.m_lockFlag.load());
		m_WriteCount.store(_lock.m_WriteCount.load());
		return *this;
	}

#ifdef USE_DEBUG
	void URWLock::WriteLock(const char* _name)
#else
	void RWLock::WriteLock()
#endif
	{

#ifdef USE_DEBUG
		g_pDeadLockProfiler->PushLock(_name);
#endif

		// ���� 16��Ʈ�� �����Ѵ�. 
		const _llong LockThreadID = (m_lockFlag.load() & WRITE_THREAD_MASK) >> 16;

		// ���� ������ �����尡 Lock�� �����ϰ� ������ ������ ������Ų��. 
		if (LockThreadID == TLS::g_ThreadID)
		{
			++m_WriteCount;
			return;
		}
		// 10��
		auto BeginTime = std::chrono::high_resolution_clock::now();

		// �������� �����´�. 
		const _llong Desired = ((static_cast<_llong>(TLS::g_ThreadID) << 16) & WRITE_THREAD_MASK);
		while (true)
		{
			for (_uint SpinCount = 0; SpinCount < MAX_SPIN_COUNT; ++SpinCount)
			{
				_llong Expected = EMPTY_FLAG;
				// Lock Flag�� EMPTY FLAG�� ���ٸ� LockFlag�� �� Desired�� ����
				if (m_lockFlag.compare_exchange_strong(REF_OUT Expected, Desired))
				{
					// Write Count ����
					++m_WriteCount;
					return;
				}
			}
			// 10�� �̻��� ��� �ش� �ݺ����� �������´�. 
			auto EndTime = BeginTime - std::chrono::high_resolution_clock::now();
			if (duration_cast<std::chrono::milliseconds>(EndTime).count() >= ACOUIRE_TIMEOUT_TICK)
			{
				CRASH("LOCK_TIMEOUT");
			}

			// �ϴ� �ش� ������� ��� ���� �д�. 
			std::this_thread::yield();
		}
	}
	/*
	@ Date: 2023-12-27
	@ Writer: ������
	*/
#ifdef USE_DEBUG
	void URWLock::WriteUnLock(const char* _name)
#else
	void RWLock::WriteUnLock()
#endif
	{
#ifdef USE_DEBUG
		g_pDeadLockProfiler->PopLock(_name);
#endif

		// ReadLock�� �� Ǯ������ WriteLock�� �Ұ���
		if ((m_lockFlag.load() & READ_THREAD_MASK) != 0)
		{
			CRASH("INVALID_UNLOCK_ORDER");
		}

		const _uint WRITECOUNT = --m_WriteCount;
		if (0 == WRITECOUNT)
		{
			m_lockFlag.store(EMPTY_FLAG);
		}
	}
	/*
	@ Date: 2023-12-27
	@ Writer: ������
	*/
#ifdef USE_DEBUG
	void URWLock::ReadLock(const char* _name)
#else 
	void RWLock::ReadLock()
#endif
	{
#ifdef USE_DEBUG
		g_pDeadLockProfiler->PushLock(_name);
#endif

		// ���� 16��Ʈ�� �����Ѵ�. 
		const _llong LockThreadID = (m_lockFlag.load() & READ_THREAD_MASK) >> 16;
		// ���� ������ �����尡 Lock�� �����ϰ� ������ ������ ������Ų��. 
		if (LockThreadID == TLS::g_ThreadID)
		{
			m_lockFlag.fetch_add(1);
			return;
		}
		auto BeginTime = std::chrono::high_resolution_clock::now();
		// �������� �����´�. 
		while (true)
		{
			for (_uint SpinCount = 0; SpinCount < MAX_SPIN_COUNT; ++SpinCount)
			{
				_llong Expected = (m_lockFlag.load() & READ_THREAD_MASK);
				// Lock Flag�� EMPTY FLAG�� ���ٸ� LockFlag�� �� Desired�� ����
				if (m_lockFlag.compare_exchange_strong(REF_OUT Expected, Expected + 1))
				{
					return;
				}
			}
			// 10�� �̻��� ��� �ش� �ݺ����� �������´�. 
			auto EndTime = BeginTime - std::chrono::high_resolution_clock::now();
			if (duration_cast<std::chrono::milliseconds>(EndTime).count() >= ACOUIRE_TIMEOUT_TICK)
			{
				CRASH("LOCK_TIMEOUT");
			}

			// �ϴ� �ش� ������� ��� ���� �д�. 
			std::this_thread::yield();
		}
	}
	/*
	@ Date: 2023-12-27
	@ Writer: ������
	*/
#ifdef USE_DEBUG
	void URWLock::ReadUnLock(const char* _name)
#else
	void RWLock::ReadUnLock()
#endif
	{
#ifdef USE_DEBUG
		g_pDeadLockProfiler->PopLock(_name);
#endif

		if ((m_lockFlag.fetch_sub(1) & READ_THREAD_MASK) == 0)
		{
			CRASH("MUTIPLE_UNLOCK");
		}
	}

	/*
	============================================
	RWLock 
	============================================
	FastSpinLock
	============================================
	*/

	UFastSpinLock::UFastSpinLock() : m_LockFlag{ 0 } { }

	UFastSpinLock::UFastSpinLock(const UFastSpinLock& _rhs) :
		m_LockFlag{ _rhs.m_LockFlag.load() } {}

	UFastSpinLock& UFastSpinLock::operator=(const UFastSpinLock& _lock)
	{
		m_LockFlag.store(_lock.m_LockFlag, std::memory_order_seq_cst);
		return *this;
	}

	UFastSpinLock::~UFastSpinLock(){ }

	void UFastSpinLock::EnterWriteLock()
	{
		while (true)
		{
			while (m_LockFlag.load(std::memory_order_relaxed) & LF_WRITE_MASK)
				std::this_thread::yield();

			
			if ((m_LockFlag.fetch_add(LF_WRITE_FLAG, std::memory_order_acquire) & LF_WRITE_MASK) == LF_WRITE_FLAG)
			{
				while (m_LockFlag.load(std::memory_order_relaxed) & LF_READ_MASK)
					std::this_thread::yield();
				return;
			}
			m_LockFlag.fetch_sub(LF_WRITE_FLAG, std::memory_order_relaxed);
		}
	}

	void UFastSpinLock::LeaveWriteLock()
	{
		m_LockFlag.fetch_sub(LF_WRITE_FLAG, std::memory_order_relaxed);
	}

	void UFastSpinLock::EnterReadLock()
	{
		while (true)
		{
			while (m_LockFlag.load(std::memory_order_relaxed) & LF_WRITE_MASK)
				std::this_thread::yield();

			if ((m_LockFlag.fetch_add(1, std::memory_order_acquire) & LF_WRITE_MASK) == LF_WRITE_FLAG)
			{
				return;
			}
			m_LockFlag.fetch_sub(1, std::memory_order_relaxed);
		}
	}

	void UFastSpinLock::LeaveReadLock()
	{
		m_LockFlag.fetch_sub(1, std::memory_order_relaxed);
	}
}