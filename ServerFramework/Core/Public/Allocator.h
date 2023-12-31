#ifndef _SERVERFRAMEWORK_CORE_PUBLIC_ALLOCATOR_H
#define _SERVERFRAMEWORK_CORE_PUBLIC_ALLOCATOR_H

namespace Core {

	/*
	----------------------------------------
	BASE ALLOCATOR
	----------------------------------------
	*/
	/*
	@ Date: 2023-12-30
	@ Writer: ������
	@ Explain
	- �⺻���� �޸� �Ҵ�� Delete�� �ð� �ִ� Ŭ�����̴�. 
	*/
	class CORE_DLL UBaseAllocator {
	public:
		/*
		@ Date: 2023-12-30
		@ Writer: ������
		*/
		static void* Alloc(_ullong _size);
		/*
		@ Date: 2023-12-30
		@ Writer: ������
		*/
		static void Release(void* _ptr);
	};


	/*
	----------------------------------------
	BASE ALLOCATOR
	----------------------------------------
	POOL ALLOCATOR
	----------------------------------------
	*/
		/*
		@ Date: 2023-12-31
		@ Writer: ������
		@ Explain
		- �޸� Ǯ ��ü �Ҵ��� ���� Ŭ����
		*/
		class CORE_DLL UPoolAllocator {
		public:
			/*
			@ Date: 2023-12-30
			@ Writer: ������
			*/
			static void* Alloc(_ullong _size);
			/*
			@ Date: 2023-12-30
			@ Writer: ������
			*/
			static void Release(void* _ptr);
	};



	/*
	----------------------------------------
	POOL ALLOCATOR
	----------------------------------------
	STOMP ALLOCATOR
	----------------------------------------
	*/
	/*
	@ Date: 2023-12-30
	@ Writer: ������
	@ Explain
	- �ش��ϴ� OS�� �°� ���� �޸𸮸� �Ҵ����ִ� Ŭ�����̴�.
	*/
	class CORE_DLL UStompAllocator{
		/*
		@ Date: 2023-12-30
		@ Writer: ������
		@ Explain
		- �޸𸮸� �Ҵ��� �� PAGE_SIZE ��ŭ ����ִ� ���̴�. 
		�׷��� �޸𸮸� �Ҵ��� �� Page_Size��ŭ �Ҵ��ϱ� ���ؼ� ���
		*/
		enum { PAGE_SIZE = 0x1000 };

	public:
		/*
		@ Date: 2023-12-30
		@ Writer: ������
		*/
		static void* Alloc(_ullong _size);

#ifdef WINDOW_OS
		/*
		@ Date: 2023-12-30
		@ Writer: ������
		*/
		static void		Release(void* _ptr);
#else
		static void		Release(void* _ptr, _ullong _size);
#endif
	};

	/*
	----------------------------------------
	STOMP ALLOCATOR
	----------------------------------------
	STOMP ALLOCATOR
	----------------------------------------
	*/

	template<class Type, class... Args>
	Type* MemoryAlloc(Args&&... args)
	{
		Type* memory = static_cast<Type*>(UPoolAllocator::Alloc(sizeof(Type)));
		new(memory)Type(std::forward<Args>(args)...); // placement new
		return memory;
	}

	template<class Type>
	void MemoryRelease(Type* obj)
	{
		obj->~Type();
		UPoolAllocator::Release(obj);
	}
}


#endif // _SERVERFRAMEWORK_CORE_PUBLIC_ALLOCATOR_H