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
	class BaseAllocator {
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
	STOMP ALLOCATOR
	----------------------------------------
	*/
	/*
	@ Date: 2023-12-30
	@ Writer: ������
	@ Explain
	- �ش��ϴ� OS�� �°� ���� �޸𸮸� �Ҵ����ִ� Ŭ�����̴�.
	*/
	class StompAllocator{
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
}


#endif // _SERVERFRAMEWORK_CORE_PUBLIC_ALLOCATOR_H