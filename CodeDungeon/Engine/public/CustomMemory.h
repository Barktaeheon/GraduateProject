#pragma once

namespace Engine {

	/*
	@ Date: 2024-02-04, Writer: ������
	@ Explain
	-  Window���� �����ϴ� LockFreeList�� ����ϱ� ���� SLIST ENUM 
	*/
	enum
	{
		SLIST_ALIGNMENT = 16
	};

	/*
	===========================================================
	MemoryHeader
	===========================================================
	*/
	/*
	@ Date: 2024-02-01, Writer: ������
	@ Explain
	-  �޸� �տ� �ٴ� ���
	*/
	DECLSPEC_ALIGN(SLIST_ALIGNMENT)
	class UMemoryHeader : public SLIST_ENTRY {
	public:
		UMemoryHeader(long long _Size) : m_AllocSize{ _Size } {}

		static void* AttachHeader(UMemoryHeader* _Header, long long _Size) {
			new(_Header)UMemoryHeader(_Size);
			return reinterpret_cast<void*>(++_Header);
		}

		static UMemoryHeader* DetachHeader(void* _Ptr) {
			UMemoryHeader* header = reinterpret_cast<UMemoryHeader*>(_Ptr) - 1;
			return header;
		}

		long long GetAllocSize() const { return m_AllocSize; }
		void SetAllocSize(const long long _AllocSize) { this->m_AllocSize = _AllocSize; }
	private:
		long long	m_AllocSize;
	};
	/*
	===========================================================
	MemoryHeader
	===========================================================
	MemoryPool
	===========================================================
	*/
	/*
	@ Date: 2024-02-01, Writer: ������
	@ Explain
	-  �޸𸮸��� ������ Memory Pool�̴�.
	*/
	DECLSPEC_ALIGN(SLIST_ALIGNMENT)
	class UMemoryPool {
	public:
		UMemoryPool(long long _AllocSize);
		~UMemoryPool();

		void			Push(UMemoryHeader* ptr);
		UMemoryHeader* Pop();

	private:
		SLIST_HEADER			m_Header;
		long long					m_AllocSize;
	};
	/*
	===========================================================
	MemoryPool
	===========================================================
	MemoryAdminster
	===========================================================
	*/
	/*
	@ Date: 2024-02-04, Writer: ������
	@ Explain
	-  �޸� Ǯ�� �����ϴ� Ŭ����
	*/
	class UMemoryAdminstor {
		enum
		{
			POOL_COUNT = (512 / 16) + (1024 / 32) + (1024 / 128) + (2048 / 256),
			MAX_ALLOC_SIZE = 4608
		};
		using POOLTABLE = std::array<UMemoryPool*, MAX_ALLOC_SIZE + 1>;
		using MEMORYTABLE = std::array<UMemoryPool*, POOL_COUNT>;
	public:
		UMemoryAdminstor();
		~UMemoryAdminstor();

		void* Allocate(unsigned long long _Size);
		void Release(void* _Ptr);

	private:
		void MakeMemoryPool(unsigned int&  _Size, unsigned int& _MemoryIndex,
			unsigned int& _TableIndex, const unsigned int  _Limited, const 	unsigned int  _AddValue);

	private:
		// �޸𸮸� ������ ã�� ���� Ǯ ���̺��̴�. 
		POOLTABLE			m_PoolTable;
		MEMORYTABLE	m_MemoryTable;
	};

	/*
	===========================================================
	MemoryAdminster
	===========================================================
	BaseAllocator
	===========================================================
	*/
	/*
	@ Date: 2024-02-01,  Writer: ������
	@ Explain
	- �⺻���� �Ҵ��� ���� Ŭ����
	*/
	class UBaseAllocator
	{
	public:
		static void* Alloc(size_t size);
		static void	Release(void* ptr);
	};
		/*
	===========================================================
	BaseAllocator
	===========================================================
	PoolAllocator
	===========================================================
	*/
	/*
	@ Date: 2024-02-01,  Writer: ������
	@ Explain
	- Memory Adiminster���� �޸� ����
	*/
	class UPoolAllocator {
	public:
		static void* Alloc(size_t _size);
		static void Release(void* _ptr);
		static void Release(const void* _ptr);
	};
	/*
	===========================================================
	UPoolAllocator
	===========================================================
	UStlAllocator
	===========================================================
	*/

	/*-------------------
	STL Allocator
-------------------*/

	/*
	@ Date: 2024-02-01,  Writer: ������
	@ Explain
	- STL�� ����ϱ� ���� Allocator
	*/
	template<typename T>
	class UStlAllocator
	{
	public:
		using value_type = T;

		UStlAllocator() { }

		template<typename Other>
		UStlAllocator(const UStlAllocator<Other>&) { }

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
		bool operator==(const UStlAllocator<U>&) { return true; }

		template<typename U>
		bool operator!=(const UStlAllocator<U>&) { return false; }
	};
	/*
	===========================================================
	UStlAllocator
	===========================================================
	UObjectPool
	===========================================================
	*/
	/*
	@ Date: 2024-02-02,  Writer: ������
	@ Explain
	- ObjectPool Ŭ������ Memory Pool
	*/
	template<class Type>
	class UObjectPool {
	public:
		template<class... Args>
		static Type* Pop(Args&&... _args) {
			Type* memory = static_cast<Type*>(UMemoryHeader::AttachHeader(s_MemoryPool.Pop(), s_AllocSize));
			new(memory)Type(std::forward<Args>(_args)...); // placement new
			return memory;
		}
		static void Push(Type* obj)
		{
			obj->~Type();
			s_MemoryPool.Push(UMemoryHeader::DetachHeader(obj));
		}

		template<class... Args>
		static std::shared_ptr<Type> MakeShared(Args&&... _args) {
			std::shared_ptr<Type> ptr = { Pop(std::forward<Args>(_args)...), Push };
			return std::move(ptr);
		}
	private:
		static UMemoryPool	s_MemoryPool;
		static int							s_AllocSize;
	};

	template<typename Type>
	int UObjectPool<Type>::s_AllocSize = sizeof(Type) + sizeof(UMemoryHeader);

	template<typename Type>
	UMemoryPool UObjectPool<Type>::s_MemoryPool{ s_AllocSize };

	/*
	===========================================================
	UObjectPool
	===========================================================
	Make
	===========================================================
	*/

	namespace Make {
		/*
		@ Date: 2024-02-03,  Writer: ������
		@ Explain
		- �Ҵ��� ���� custom �ϱ� ���� �Լ�
		*/
		template<typename Type, typename... Args>
		static Type* xnew(Args&&... args)
		{
			Type* memory = static_cast<Type*>(UPoolAllocator::Alloc(sizeof(Type)));
			new(memory)Type(std::forward<Args>(args)...); // placement new
			return memory;
		}
		/*
		@ Date: 2024-02-03,  Writer: ������
		@ Explain
		- �������� �Ҵ��� ���� �����ϴ� �Լ�
		*/
		template<typename Type>
		static void xdelete(Type* obj)
		{
			obj->~Type();
			UPoolAllocator::Release(obj);
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
		/*
		@ Date: 2024-02-03,  Writer: ������
		@ Explain
		- make_shared�� custom
		*/
		template<typename Type, typename... Args>
		requires ConstructWidthArgsCheck<Type, Args...>
		static std::shared_ptr<Type> MakeShared(Args&&... args)
		{
		//	return std::shared_ptr<Type>{ xnew<Type>(std::forward<Args>(args)...), xdelete<Type> };
			return std::make_shared<Type>(std::forward<Args>(args)...);
		}
	}
}