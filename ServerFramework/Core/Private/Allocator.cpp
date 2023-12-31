#include "CoreDefines.h"
#include "Allocator.h"
#include "Memory.h"

namespace Core
{
	void* UBaseAllocator::Alloc(_ullong  _size)
	{
		return ::malloc(_size);
	}

	void UBaseAllocator::Release(void* _ptr)
	{
		::free(_ptr);
	}


	/*
	----------------------------------------
	BASE ALLOCATOR
	----------------------------------------
	STOMP
	----------------------------------------
	*/


	void* UStompAllocator::Alloc(_ullong _size)
	{
		// �ݿø��� ���� �ڵ�
		const _ullong PageCount = (_size + PAGE_SIZE - 1) / PAGE_SIZE;
		// PageCount�� �ڿ� �κп� �Ҵ��� �� �ְ� DataOffset �����ؼ� �����÷ο� ���� �ذ�
		const _ullong DataOffset = PageCount * PAGE_SIZE - _size;

		
#ifdef WINDOW_OS
		void* BaseAddress = ::VirtualAlloc(NULL, PageCount * PAGE_SIZE, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
#else
		void* BaseAddress = mmap(NULL, PageCount * PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
#endif
		return static_cast<void*>(static_cast<_byte*>(BaseAddress) + DataOffset);
	}
#ifdef WINDOW_OS
	void UStompAllocator::Release(void* _ptr)
	{
		// DataOffset�� �����ؼ� �Ҵ��� �޸𸮸� ���� �ڸ��� ���ư��� �����. 
		const _llong address = reinterpret_cast<_llong>(_ptr);
		const _llong baseAddress = address - (address % PAGE_SIZE);
		::VirtualFree(reinterpret_cast<void*>(baseAddress), 0, MEM_RELEASE);
	}
#else
	void StompAllocator::Release(void* _ptr, _ullong _size)
	{	
		const _ullong PageCount = (_size + PAGE_SIZE - 1) / PAGE_SIZE;
		const _llong address = reinterpret_cast<_llong>(_ptr);
		const _llong BaseAddress = address - (address % PAGE_SIZE);
		munmap(reinterpret_cast<_byte*>(BaseAddress), PAGE_SIZE);
	}
#endif

	void* UPoolAllocator::Alloc(_ullong _size)
	{
		return g_MemoryAdminster->Allocate(_size);
	}

	void UPoolAllocator::Release(void* _ptr)
	{
		g_MemoryAdminster->Release(_ptr);
	}

	/*
	----------------------------------------
	STOMP ALLOCATOR
	----------------------------------------
	STOMP ALLOCATOR
	----------------------------------------
	*/
}