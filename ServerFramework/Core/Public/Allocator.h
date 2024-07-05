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
		static void* Alloc(size_t _size);
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
		@ Date: 2023-12-31, Writer: ������
		@ Explain
		- �޸� Ǯ ��ü �Ҵ��� ���� Ŭ����
		*/
		class CORE_DLL UPoolAllocator {
		public:
			/*
			@ Date: 2023-12-30
			@ Writer: ������
			*/
			static void* Alloc(size_t _size);
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
		static void* Alloc(size_t _size);

#ifdef _WIN32
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
	STL ALLOCATOR
	----------------------------------------
	*/

	template<class T>
	class USTLAllocator {
	public:
		using value_type = T;
		
		USTLAllocator() {}

		template<class Other>
		USTLAllocator(const USTLAllocator<Other>&) {}

		T* allocate(size_t count)
		{
			const size_t size = count * sizeof(T);
			return static_cast<T*>(UPoolAllocator::Alloc(size));
		}
		void deallocate(T* ptr, size_t count)
		{
			UPoolAllocator::Release(ptr);
		}

		template<typename U>
		bool operator==(const USTLAllocator<U>&) { return true; }

		template<typename U>
		bool operator!=(const USTLAllocator<U>&) { return false; }
	};

	/*
	----------------------------------------
	STL ALLOCATOR
	----------------------------------------
	MEMORY ALLOC FUNC
	----------------------------------------
	*/

	template<class Type, class... Args>
	Type* MemoryAlloc(Args&&... _args)
	{
		Type* memory = static_cast<Type*>(UPoolAllocator::Alloc(sizeof(Type)));
		new(memory)Type(std::forward<Args>(_args)...); // placement new
		return memory;
	}

	template<class Type>
	Type* MemoryAllocBuffer(size_t _bufferSize)
	{
		Type* memory = static_cast<Type*>(UPoolAllocator::Alloc(_bufferSize));
		::memset(memory, 0, _bufferSize);
		return memory;
	}

	template<class Type>
	void MemoryRelease(Type* _obj)
	{
		_obj->~Type();
		UPoolAllocator::Release(_obj);
	}

	template <class Type>
	void MemoryBufferRelease(Type* _obj)
	{
		UPoolAllocator::Release(_obj);
	}

	/*
	@ Date: 2024-02-04,  Writer: ������
	@ Explain
	- �迭�� �Ҵ��ϱ� ���� �Լ�
	*/
	template<class T>
	static T* AllocBuffer(size_t _number) {
		T* p = static_cast<T*>(UPoolAllocator::Alloc(_number * sizeof(T)));
		return p;
	}
	/*
	@ Date: 2024-02-03,  Writer: ������
	@ Explain
	- �迭�� �����ϱ� ���� �Լ�
	*/
	template<class T>
	static void ReleaseBuffer(T* obj)
	{
		if (nullptr == obj)
			return;

		UPoolAllocator::Release(obj);
	}
	/*
	@ Date: 2024-02-03,  Writer: ������
	@ Explain
	- const �� �迭�� �����ϱ� ���� �Լ�
	*/
	template<class T>
	static void ReleaseBuffer(const T* obj)
	{
		if (nullptr == obj)
			return;
		UPoolAllocator::Release(obj);
	}
}


#endif // _SERVERFRAMEWORK_CORE_PUBLIC_ALLOCATOR_H