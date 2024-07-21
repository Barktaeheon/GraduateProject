#include "CoreDefines.h"
#include "Memory.h"

namespace Core
{
	AMemoryPool::AMemoryPool(const _ullong& _AllocateSize) : 
		m_AllocateSize{ _AllocateSize }, m_AllocateCount{0}
	{

	}

	AMemoryPool::~AMemoryPool()
	{
	}
	/*
	* �ݳ��� �޸𸮸� Ǯ�� �����Ѵ�. 
	*/
	void AMemoryPool::Push(MEMORYHEADER* _ptr)
	{
		std::atomic_thread_fence(std::memory_order_seq_cst);
		MEMORYHEADER* Ptr = _ptr;
		{
			Ptr->AllocateSize = 0;
		}
		m_MemoryQueue.push(std::move(_ptr));
		m_AllocateCount.fetch_add(1);
	}

	MEMORYHEADER* AMemoryPool::Pop()
	{
		std::atomic_thread_fence(std::memory_order_seq_cst);
		MEMORYHEADER* Header = nullptr;
		if (false == m_MemoryQueue.empty())
		{
			m_MemoryQueue.try_pop(Header);
		}

		if (nullptr == Header)
		{
			Header = reinterpret_cast<MEMORYHEADER*>(std::malloc(m_AllocateSize));
		}
		else
		{
			ASSERT_CRASH(Header->AllocateSize == 0);
			m_AllocateCount.fetch_sub(1);
		}

		return Header;
	}

	void AMemoryPool::Release()
	{
		while (!m_MemoryQueue.empty())
		{
			MEMORYHEADER* header;
			m_MemoryQueue.try_pop(header);
			std::this_thread::sleep_for(std::chrono::microseconds(1));
			::free(header);
		}
	}

	/*
	-------------------------------
	MEMORYPOOL
	------------------------------
	MEMORYADMINSTER
	------------------------------
	*/

	// ������ �޸� �Ҵ�
	AMemoryAdiminstor::AMemoryAdiminstor()
	{
		unsigned int TableIndex = 0;
		unsigned int MemoryIndex = 0;
		unsigned int SIZE = 16;
		MakeMemoryPool(SIZE, MemoryIndex, TableIndex, 512, 16);
		MakeMemoryPool(SIZE, MemoryIndex, TableIndex, 1024 + 512, 32);
		MakeMemoryPool(SIZE, MemoryIndex, TableIndex, 1024 + 1024 + 512, 128);
		MakeMemoryPool(SIZE, MemoryIndex, TableIndex, MAX_ALLOC_SIZE + 1, 256);
	}

	AMemoryAdiminstor::~AMemoryAdiminstor()
	{
		for (auto& iter : m_PoolTable)
		{
			if (nullptr != iter)
				iter->Release();
			delete iter;
		}
	}

	// SIze�� ũ�Ⱑ �ش� �޸𸮿� ������ �׳� �Ҵ�, �ƴ϶�� �����Ѵ�. 
	void* AMemoryAdiminstor::Allocate(_ullong _Size)
	{
		MEMORYHEADER* Header = nullptr;
		_ullong AllocateSize = _Size + sizeof(MEMORYHEADER);
#ifdef USE_STOMP
		Header = reinterpret_cast<MEMORYHEADER*>(UStompAllocator::Alloc(_Size));
#else
		// �޸� �ִ� ũ�⸦ �������� �Ϲ� �Ҵ� 
		if (AllocateSize > MAX_ALLOC_SIZE)
		{
			Header = reinterpret_cast<MEMORYHEADER*>(::malloc(AllocateSize));
		}
		else
		{
			// �޸� Ǯ���� �����´�. 
			Header = m_PoolTable[AllocateSize]->Pop();
		}
#endif
		return MEMORYHEADER::AttachHeader(Header, AllocateSize);
	}
	// SIze�� ũ�Ⱑ �ش� �޸𸮿� ������ ����, �ƴϸ� �ݳ��Ѵ�. 
	void AMemoryAdiminstor::Release(void* _Ptr)
	{
		MEMORYHEADER* Header = MEMORYHEADER::DetachHeader(_Ptr);

		const _ullong AllocSize = Header->AllocateSize;
		ASSERT_CRASH(AllocSize > 0);

#ifdef USE_STOMP
		UStompAllocator::Release(_Ptr);
#else
		if (AllocSize > MAX_ALLOC_SIZE)
		{
			::free(Header);
		}
		else
		{
			::memset(Header, 0, AllocSize);
			m_PoolTable[AllocSize]->Push(Header);
		}
#endif
	}
	// �޸� �Ҵ��� ���ִ� �Լ��̴�. 
	void AMemoryAdiminstor::MakeMemoryPool(unsigned int& _Size, unsigned int& _MemoryIndex,
		unsigned int& _TableIndex, const unsigned int  _Limited, const 	unsigned int  _AddValue)
	{
		for (; _Size < _Limited;)
		{
			AMemoryPool* pool = new AMemoryPool(_Size);
			m_MemoryTable[_MemoryIndex++] = pool;
			while (_TableIndex <= _Size)
			{
				m_PoolTable[_TableIndex++] = pool;
			}
			_Size += _AddValue;
		}
	}
}