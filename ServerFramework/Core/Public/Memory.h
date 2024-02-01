#ifndef _SERVERFRAMEWORK_CORE_PUBLIC_MEMORY_H
#define _SERVERFRAMEWORK_CORE_PUBLIC_MEMORY_H

#include "Allocator.h"

namespace Core {

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
		using MEMORYQUEUE = CONQUEUE<MEMORYHEADER*>;
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
	class CORE_DLL AMemoryAdminster {
		enum
		{
			POOL_COUNT = (512 / 16) + (512 / 32) + (1024 / 64) + (1024 / 128) + (2048 / 256),
			MAX_ALLOC_SIZE = 5120
		};
	public: 
		AMemoryAdminster();
		~AMemoryAdminster();

		void* Allocate(_ullong _Size);
		void Release(void* _Ptr);

	private:
		void MakeMemoryPool(_uint _Size, REF_IN _uint& _TableIndex,
			const _uint _Limited, const _uint _AddValue);

	private:
		// �޸𸮸� ������ ã�� ���� Ǯ ���̺��̴�. 
		ARRAY<AMemoryPool*, MAX_ALLOC_SIZE + 1>		m_PoolTable;

		CONVECTOR<AMemoryPool*>										m_Pools;
	};

}


#endif // _SERVERFRAMEWORK_CORE_PUBLIC_MEMORY_H