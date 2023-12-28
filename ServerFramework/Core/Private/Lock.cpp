#include "CoreDefines.h"
#include "Lock.h"

namespace Core
{
	namespace Lock
	{
		/*
		@ Date: 2023-12-27
		@ Writer: ������
		@	 Explain: �ƹ��� �������� ���� ��� Write Lock�� �ɾ �ڽ��� ���ϴ� ���� ����Ǳ⸦ �ٶ���. 
		*/
		RWLock::RWLock() : m_lockFlag{ 0 }, m_WriteCount{ 0 } {
		}
		RWLock::RWLock(const RWLock& _rhs) 
		{
			m_lockFlag.store(_rhs.m_lockFlag.load());
			m_WriteCount.store(_rhs.m_WriteCount.load());
		}
		RWLock::~RWLock()
		{
		}
		RWLock& RWLock::operator=(const RWLock& _lock)
		{
			m_lockFlag.store(_lock.m_lockFlag.load());
			m_WriteCount.store(_lock.m_WriteCount.load());
			return *this;
		}
		void RWLock::WriteLock()
		{
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
			const _llong Desired = ((TLS::g_ThreadID << 16) & WRITE_THREAD_MASK);
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
		void RWLock::WriteUnLock()
		{
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
		void RWLock::ReadLock()
		{
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
		void RWLock::ReadUnLock()
		{
			if ((m_lockFlag.fetch_sub(1) & READ_THREAD_MASK) == 0)
			{
				CRASH("MUTIPLE_UNLOCK");
			}
		}

	}
}