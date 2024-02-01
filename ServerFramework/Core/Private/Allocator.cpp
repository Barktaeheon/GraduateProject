#include "CoreDefines.h"
#include "Allocator.h"
#include "Memory.h"

namespace Core
{
	void* 	UBaseAllocator::Alloc(size_t  _size)
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


	void* UStompAllocator::Alloc(size_t _size)
	{
		// �ݿø��� ���� �ڵ�
		const size_t PageCount = (_size + PAGE_SIZE - 1) / PAGE_SIZE;
		// PageCount�� �ڿ� �κп� �Ҵ��� �� �ְ� DataOffset �����ؼ� �����÷ο� ���� �ذ�
		const size_t DataOffset = PageCount * PAGE_SIZE - _size;

		
#ifdef _WIN32
		void* BaseAddress = ::VirtualAlloc(NULL, PageCount * PAGE_SIZE, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
#else
		void* BaseAddress = mmap(NULL, PageCount * PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
#endif
		return static_cast<void*>(static_cast<_byte*>(BaseAddress) + DataOffset);
	}
#ifdef _WIN32
	void UStompAllocator::Release(void* _ptr)
	{
		// DataOffset�� �����ؼ� �Ҵ��� �޸𸮸� ���� �ڸ��� ���ư��� �����. 
		const size_t address = reinterpret_cast<size_t>(_ptr);
		const size_t baseAddress = address - (address % PAGE_SIZE);
		::VirtualFree(reinterpret_cast<void*>(baseAddress), 0, MEM_RELEASE);
	}
#else
	void StompAllocator::Release(void* _ptr, size_t _size)
	{	
		const _ullong PageCount = (_size + PAGE_SIZE - 1) / PAGE_SIZE;
		const size_t address = reinterpret_cast<size_t>(_ptr);
		const size_t BaseAddress = address - (address % PAGE_SIZE);
		munmap(reinterpret_cast<_byte*>(BaseAddress), PAGE_SIZE);
	}
#endif

	void* UPoolAllocator::Alloc(size_t _size)
	{
		return g_pMemoryAdminster->Allocate(_size);
	}

	void UPoolAllocator::Release(void* _ptr)
	{
		g_pMemoryAdminster->Release(_ptr);
	}

	/*
	----------------------------------------
	STOMP ALLOCATOR
	----------------------------------------
	STOMP ALLOCATOR
	----------------------------------------
	*/
}