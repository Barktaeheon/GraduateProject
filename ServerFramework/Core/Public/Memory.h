#ifndef _SERVERFRAMEWORK_CORE_PUBLIC_MEMORY_H
#define _SERVERFRAMEWORK_CORE_PUBLIC_MEMORY_H

#include "Allocator.h"

namespace Core {
	/*
	@ Date: 2024-02-04, Writer: ������
	@ Explain
	-  �⺻ �޸� ������ (Memory Pool ����ȭ�� ����)
	*/
	enum
	{
		BASE_ALLOC_SIZE = 16
	};
	/*
	@ Date: 2023-12-31
	@ Writer: ������
	@ Explain
	- �޸��� �� �κ��� ��Ÿ���� Header�̴�.
	*/
	struct  MEMORYHEADER {

		MEMORYHEADER(_llong _Size) : AllocateSize{ _Size } {}

		static void* AttachHeader(MEMORYHEADER* _Header, _llong _Size)
		{
			new(_Header)MEMORYHEADER(_Size);
			// �޸� ��� �κ��� �ǳ� �ڴ�. 
			return reinterpret_cast<void*>(++_Header);
		}

		static MEMORYHEADER* DetachHeader(void* _Ptr)
		{
			// �޸� ��� �κ��� �����Ѵ�. 
			MEMORYHEADER* Header = reinterpret_cast<MEMORYHEADER*>(_Ptr) - 1;
			return Header;
		}

		volatile _llong		AllocateSize;
	};

	/*
	-------------------------------
	MEMORYHEADER
	------------------------------
	MEMORYPOOL
	------------------------------
	*/

	/*
	@ Date: 2023-12-31, Writer: ������
	@ Explain
	- �޸𸮵��� �����ϴ� Ŭ����
	*/
	class CORE_DLL AMemoryPool {
		using MEMORYQUEUE = CONPRIORITYQUEUE<MEMORYHEADER*>;
	public:
		AMemoryPool(const _ullong& _AllocateSize);
		~AMemoryPool();

		void Push(MEMORYHEADER* _ptr);
		MEMORYHEADER* Pop();
	private:
		_ullong														m_AllocateSize;
		ATOMIC<_uint>										m_AllocateCount;
		// Memory Queue
		MEMORYQUEUE										m_MemoryQueue;
	};


	/*
	-------------------------------
	MEMORYPOOL
	------------------------------
	MEMORYADMINSTER
	------------------------------
	*/
	/*
	@ Date: 2023-12-31
	@ Writer: ������
	@ Explain
	- �޸𸮸� �����ϴ� ������ Ŭ����
	*/
	class CORE_DLL AMemoryAdiminstor {
		enum
		{
			POOL_COUNT = 350,
			MAX_ALLOC_SIZE = BASE_ALLOC_SIZE * POOL_COUNT
		};
	public:
		AMemoryAdiminstor();
		~AMemoryAdiminstor();

		void* Allocate(_ullong _Size);
		void Release(void* _Ptr);

	private:
		void MakeMemoryPool(_uint _Size, const _uint _Limited, const _uint _AddValue);

	private:
		// �޸𸮸� ������ ã�� ���� Ǯ ���̺��̴�. 
		ARRAY<AMemoryPool*, POOL_COUNT>		m_PoolTable;
		CONUNORMAP<_ullong, int>							m_KeyTable;
	};

}


#endif // _SERVERFRAMEWORK_CORE_PUBLIC_MEMORY_H