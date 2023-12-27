#ifndef _SERVERFRAMEWORK_CORE_PUBLIC_LOCK_H
#define _SERVERFRAMEWORK_CORE_PUBLIC_LOCK_H

BEGIN(Core)


BEGIN(Lock)

enum : _llong
{
	ACOUIRE_TIMEOUT_TICK = 10000,
	MAX_SPIN_COUNT = 5000,
	WRITE_THREAD_MASK = 0xFFFF'0000,
	READ_THREAD_MASK = 0x0000'FFFF,
	EMPTY_FLAG = 0x0000'0000
};

/*
=============================
			RW Spin Lock
=============================
*/
/*
[\\\\\\\\][\\\\\\\\][RRRRRRRR][RRRRRRRR]
W: WriteFlag (Exclusive Lock Owner Thread)
R: ReadFlag(Shared Lock Count)
*/
/*
@ Date: 2023-12-27
@ Writer: ������
@ Explain:  RW Lock ����

W -> R (O)
R -> W (X)
*/
class  RWLock {
public:
	RWLock();
	RWLock(const RWLock& _rhs);
	~RWLock();

	RWLock& operator =(const RWLock& _lock);
	/*
	@ Explain:  WriteLock �ش��ϴ� Lock�� �ɸ� ����, ��� Lock�� �������� �ʴ´�. 
	*/
	void WriteLock();
	/*
	@ Explain:  WriteLock ����
	*/
	void WriteUnLock();
	/*
	@ Explain:  ReadLock ���� Read Lock�� �ɷ��־ �׳� ��������� WriteLock�� �ɸ� ��� ���
	*/
	void ReadLock();
	/*
	@ Explain:  ReadLock ���� 
	*/
	void ReadUnLock();
private:
	ATOMIC<_llong>	m_lockFlag;
	ATOMIC<_uint>	m_WriteCount;
};

class ReadLockGuard {
public:
	ReadLockGuard(const RWLock& _Lock) : m_Lock{ _Lock } { m_Lock.ReadLock(); }
	~ReadLockGuard() { m_Lock.ReadUnLock(); }

private:
	RWLock		m_Lock;
};

class WriteLockGuard {
public:
	WriteLockGuard(const RWLock& _Lock) : m_Lock{ _Lock } { m_Lock.WriteLock(); }
	~WriteLockGuard() { m_Lock.WriteUnLock(); }

private:
	RWLock		m_Lock;
};

END

END

#endif 
