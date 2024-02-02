#pragma once

namespace Engine {
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
	class UMemoryHeader {
	public:
		UMemoryHeader(_llong _Size) : m_AllocSize{ _Size } {}

		static void* AttachHeader(UMemoryHeader* _Header, _llong _Size) {
			new(_Header)UMemoryHeader(_Size);
			return reinterpret_cast<void*>(++_Header);
		}

		static UMemoryHeader* DetachHeader(void* _Ptr) {
			UMemoryHeader* header = reinterpret_cast<UMemoryHeader*>(_Ptr) - 1;
			return header;
		}

		_llong GetAllocSize() const { return m_AllocSize; }
		void SetAllocSize(const _llong _AllocSize) { this->m_AllocSize = _AllocSize; }
	private:
		_llong	m_AllocSize;
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
	class UMemoryPool {
	public:
		UMemoryPool(_int _AllocSize);
		~UMemoryPool();

		void			Push(UMemoryHeader* ptr);
		UMemoryHeader* Pop();

	private:
		CONQUEUE<UMemoryHeader*>		m_MemoryQueue;
		_int															m_AllocSize;
	};
	/*
	===========================================================
	MemoryPool
	===========================================================
	MemoryAdminster
	===========================================================
	*/
	/*
	@ Date: 2024-02-01, Writer: ������
	@ Explain
	-  �޸� Ǯ�� �����ϴ� Ŭ����
	*/
	class UMemoryAdminster {
		enum
		{
			POOL_COUNT = (512 / 16) + (512 / 32) + (1024 / 64) + (1024 / 128) + (2048 / 256),
			MAX_ALLOC_SIZE = 5120
		};
	public:
		UMemoryAdminster();
		~UMemoryAdminster();

		void* Allocate(_ullong _Size);
		void Release(void* _Ptr);

	private:
		void MakeMemoryPool(_uint _Size, _uint& _TableIndex,
			const _uint _Limited, const _uint _AddValue);

	private:
		// �޸𸮸� ������ ã�� ���� Ǯ ���̺��̴�. 
		ARRAY<UMemoryPool*, MAX_ALLOC_SIZE + 1>	m_PoolTable;
		CONVECTOR<UMemoryPool*>									m_Pools;
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
	};
	/*
	===========================================================
	UPoolAllocator
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
		static _int						s_AllocSize;
	};

	template<typename Type>
	_int UObjectPool<Type>::s_AllocSize = sizeof(Type) + sizeof(UMemoryHeader);

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

		template<typename Type, typename... Args>
		Type* xnew(Args&&... args)
		{
			Type* memory = static_cast<Type*>(UPoolAllocator::Alloc(sizeof(Type)));
			new(memory)Type(std::forward<Args>(args)...); // placement new
			return memory;
		}

		template<typename Type>
		void xdelete(Type* obj)
		{
			obj->~Type();
			UPoolAllocator::Release(obj);
		}

		template<typename Type, typename... Args>
		std::shared_ptr<Type> MakeShared(Args&&... args)
		{
			return std::shared_ptr<Type>{ xnew<Type>(std::forward<Args>(args)...), xdelete<Type> };
		}
	}
}