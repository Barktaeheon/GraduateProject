#ifndef _SERVERFRAMEWORK_CORE_PUBLIC_SMARTPOINTER_H
#define _SERVERFRAMEWORK_CORE_PUBLIC_SMARTPOINTER_H

namespace Core
{
	/*
	@ Date: 2023-12-26
	@ Writer: ������
	@ Explain: Cash������ 16����Ʈ ������ ���߱� ���� �ʿ��� ����
	*/
	constexpr static unsigned short CASH_ALGIN_VALUE{ 16 };

#ifdef _WIN64
	using llong = long long;
	using p_size = long long;
#else
	using llong = long;
	using p_size = long;
#endif

	template<class T>
	class URefCounter;

	template<class T>
	class USharedPtr;

	template<class T>
	class UWeakPtr;

	template<class T>
	class UEnableSharedFromThis;

	template<typename Tp, typename = void>
	struct _Can_enable_shared : std::false_type
	{};

	template<typename Tp>
	struct _Can_enable_shared<Tp, std::void_t<typename Tp::_Esft_type>>
		: std::is_convertible<std::remove_cv_t<Tp>*, typename Tp::_Esft_type*>::type
	{};


	/*
	@ Date: 2023-12-26
	@ Writer: ������
	@ Explain: ReferenceCounter -> ���� �Ű��ִ� ī����, CAS�� ���� ����
	*/
	template<class T>
	class __declspec(align(CASH_ALGIN_VALUE)) URefCounter {

		template<class T, class U>
		friend USharedPtr<T> static_shared_cast(const USharedPtr<U>& _rhs);

		template<class T, class U>
		friend USharedPtr<T> dynamic_shared_cast(const USharedPtr<U>& _rhs);

		template<class T, class U>
		friend USharedPtr<T> reinterpret_shared_cast(const USharedPtr<U>& _rhs);

		template<class T, class U>
		friend USharedPtr<T> const_shared_cast(const USharedPtr<U>& _rhs);
	public:
		URefCounter(const std::shared_ptr<T>& _ptr) :
			ptr(_ptr), ref_strong(1), ref_weak(1) {
		}
		URefCounter(const std::shared_ptr<T>& _ptr, llong _strong, llong _weak) :
			ptr(_ptr), ref_strong{ _strong }, ref_weak{ _weak } {
		}

		~URefCounter() { }
	public:
		/*
		@ Date: 2023-12-26
		@ Writer: ������
		@ Explain: Strong ���۷��� ī��Ʈ ����
		*/
		URefCounter<T>* ComparePointerToStrong()
		{
			while (true)
			{
				llong value = ref_strong.load();

				if (value > 0)
				{
					if (true == CAS_VALUE(ref_strong, value, value + 1))
					{
						return this;
					}
					else
					{
						continue;
					}
				}

				return nullptr;
			}
			return nullptr;
		}
		/*
		@ Date: 2023-12-26
		@ Writer: ������
		@ Explain: Strong ���۷��� ī��Ʈ ����, 0�� �� �� �ش� �޸� ����
		*/
		llong ReleasePointerToStrong()
		{
			llong value{ 0 };
			// Strong�� ���� 0
			while (true)
			{
				value = ref_strong.load();
				if (true == CAS_VALUE(ref_strong, value, value - 1))
				{
					if (value <= 1)
					{
						delete this;
					}
					return --value;
				}
			}
			return value;
		}
		/*
		@ Date: 2023-12-26
		@ Writer: ������
		@ Explain: Weak ���۷��� ī��Ʈ ����
		*/
		URefCounter<T>* ComparePointerToWeak()
		{
			while (true)
			{
				llong value = ref_weak.load();
				if (true == CAS_VALUE(ref_weak, value, value + 1))
				{
					return this;
				}
			}
			return nullptr;
		}
		/*
		@ Date: 2023-12-26
		@ Writer: ������
		@ Explain: Weak ���۷��� ī��Ʈ ����, 0�� �Ǿ �޸� ���� X
		*/
		llong ReleasePointerToWeak()
		{
			while (true)
			{
				llong value = ref_weak.load();
				if (true == CAS_VALUE(ref_weak, value, value - 1))
				{
					value -= 1;
					return value;
				}
			}
			return ref_weak.load();
		}

		llong GetUseCount() { return ref_strong.load() + ref_weak.load(); }

		const std::atomic<llong>& GetRefStrongCount() { return ref_strong; }
		const std::atomic<llong>& GetRefWeakCount() { return ref_weak; }

		T* get() const { return ptr.get(); }

		std::shared_ptr<T> GetShared() const { return ptr; }

		void SetShared(const std::shared_ptr<T>& _shared) { ptr = _shared; }
	private:
		/*
		@ Date: 2023-12-26
		@ Writer: ������
		@ Explain: _origin�� ���� _old�� ���� �� _weak�� �ְ� true ����
		*/
		bool CAS_VALUE(std::atomic<llong>& _origin, llong _old, llong _weak)
		{
			return _origin.compare_exchange_strong(_old, _weak);
		}
		/*
		@ Date: 2023-12-26
		@ Writer: ������
		@ Explain: �޸𸮸� �����Ľ�Ų �� ptr�� set�Ѵ�.
		*/
		void SetSharedPtr(std::shared_ptr<T> _ptr)
		{
			std::atomic_thread_fence(std::memory_order_seq_cst);
			ptr = _ptr;
		}
	private:
		std::atomic<llong>						ref_strong;
		std::atomic<llong>						ref_weak;
		std::shared_ptr<T>						ptr;
	};

	/*
	@ Date: 2023-12-26
	@ Writer: ������
	@ Explain: std::shared_ptr�� ��ġ��ũ �� ����Ʈ �����ͷ� ��Ƽ�����忡�� ������ ����Ʈ �������̴�.
	*/
	template<class T>
	class   USharedPtr {

		friend class URefCounter<T>;
		friend class UWeakPtr<T>;
		friend  class UEnableSharedFromThis<T>;

		template<class T, class U>
		friend USharedPtr<T> static_shared_cast(const USharedPtr<U>& _rhs);
		template<class T, class U>
		friend USharedPtr<T> dynamic_shared_cast(const USharedPtr<U>& _rhs);
		template<class T, class U>
		friend USharedPtr<T> reinterpret_shared_cast(const USharedPtr<U>& _rhs);
		template<class T, class U>
		friend USharedPtr<T> const_shared_cast(const USharedPtr<U>& _rhs);

		using cas_ptr = std::atomic<llong>;
	public:
		USharedPtr() : _RefCounter{ nullptr } { }
		USharedPtr(URefCounter<T>* _Ref) : _RefCounter{ _Ref } {	}
		USharedPtr(nullptr_t) : _RefCounter{ nullptr } {}
		template<class U>
			requires std::is_base_of_v<T, U>
		USharedPtr(const USharedPtr<U>& _rhs) : _RefCounter{ nullptr }
		{
			_RefCounter = reinterpret_cast<URefCounter<T>*>(_rhs.AddShared());
		}
		USharedPtr(const USharedPtr& _rhs) : _RefCounter{ nullptr } { Move(_rhs); }

		USharedPtr(USharedPtr&& _rhs) noexcept : _RefCounter{ _rhs._RefCounter } { _rhs._RefCounter = nullptr; }

		USharedPtr(const UWeakPtr<T>& _rhs) : _RefCounter{ _rhs.AddShared() } {  }

		~USharedPtr()
		{
			Release(_RefCounter);
		}
	private:
		/*
		@ Date: 2023-12-26
		@ Writer: ������
		@ Explain: EnableSharedFromThis Ŭ������ ��� �޴� Ŭ������ �ҷ��� SharedPtr( �ڱ� �ڽ��� Weak �����͸�
							 ������ �ϱ� �����̴�.
		*/
		void _Enable_shared_from_this(const std::shared_ptr<T>& _Ptr, std::true_type) {
			_Ptr->SetSharedFromThis(*this);
		}
		void _Enable_shared_from_this(const std::shared_ptr<T>&, std::false_type) {}
	public:
		void SetEnableShared(const std::shared_ptr<T>& new_ptr, URefCounter<T>* new_ctr)
		{
			_RefCounter = new_ctr;
			_Enable_shared_from_this(new_ptr, std::bool_constant<std::conjunction_v<_Can_enable_shared<T>>>{});
		}

	public:
		/*
		@ Date: 2023-12-26
		@ Writer: ������
		@ Explain: operator�� ���� nullptr_t �϶�, �ش� ������ �ʱ�ȭ
		*/
		USharedPtr& operator =(nullptr_t)
		{
			URefCounter* prevptr{ nullptr };
			while (true)
			{
				prevptr = _RefCounter;
				if (prevptr == nullptr)
					return *this;

				if (true == CAS_POINTER(&_RefCounter, prevptr, nullptr))
				{
					Release(prevptr);
					return *this;
				}
			}
			return *this;
		}

		USharedPtr& operator =(const USharedPtr& _rhs)
		{
			return Move(_rhs);
		}
		/*
		@ Date: 2023-12-26
		@ Writer: ������
		@ Explain: �̵��Լ� �϶� �ش� ������ ���� �� �ٸ� ��ü�� �� null��
		*/
		USharedPtr& operator =(USharedPtr&& _rhs)
		{
			URefCounter<T>* prevptr{ nullptr };
			URefCounter<T>* otherptr{ nullptr };

			while (true)
			{
				prevptr = _RefCounter;
				otherptr = _rhs._RefCounter;
				// ������ null�� ��
				if (otherptr == nullptr)
				{
					if (true == CAS_POINTER(&_RefCounter, prevptr, nullptr))
					{
						return *this;
					}
					continue;
				}
				// �ڱ� �ڽŰ� ���� ��
				if (otherptr == prevptr)
				{
					_rhs._RefCounter = nullptr;
					return *this;
				}
				// ����� �ٸ� ��
				if (true == CAS_POINTER(&_RefCounter, prevptr, otherptr))
				{
					_rhs._RefCounter = nullptr;
					return *this;
				}
			}
			return *this;
		}
		/*
		@ Date: 2023-12-26
		@ Writer: ������
		@ Explain: �ٸ� SharedPtr�� operator = �� ��, �ش� sharedPtr�� ȣȯ�� �Ǵ��� �ش� Ŭ������ Ȯ���ϰ� ���� �ִ´�.
		*/
		template<class U, typename std::enable_if_t<std::is_convertible_v<U*, T*>, int> = 0>
			requires std::is_base_of_v<T, U>
		USharedPtr& operator =(const USharedPtr<U>& _rhs)
		{
			URefCounter<T>* prevptr{ nullptr };
			URefCounter<T>* otherptr{ nullptr };

			while (true)
			{
				prevptr = _RefCounter;
				otherptr = reinterpret_cast<URefCounter<T>*>(_rhs.AddShared());
				// �ٸ� sharedPtr�� NULL �϶�, ���� ���� Release �Ѵ�. 
				if (otherptr == nullptr)
				{
					if (true == CAS_POINTER(&_RefCounter, prevptr, nullptr))
					{
						if (nullptr != prevptr)
						{
							Release(prevptr);
						}
						return *this;
					}
					continue;
				}
				// ���� ���� ��, ���� rRelease 
				if (otherptr == prevptr)
				{
					Release(otherptr);
					return *this;
				}
				// �ٸ� ���� ��, �ش� ���� ���� ���� ��ȯ �� �� ���� ���� Release �Ѵ�. 
				if (true == CAS_POINTER(&_RefCounter, prevptr, otherptr))
				{
					if (nullptr != prevptr)
					{
						Release(prevptr);
					}
					return *this;
				}
				Release(otherptr);
			}

			return *this;
		}
		/*
		@ Date: 2023-12-26
		@ Writer: ������
		@ Explain:  ���� RefCounter�� ������ �ִ� WeakPtr�� �޾ƿ� ��
		*/
		USharedPtr& operator =(const UWeakPtr<T>& _rhs)
		{
			URefCounter<T>* prevptr{ nullptr };
			URefCounter<T>* otherptr{ nullptr };

			while (true)
			{
				prevptr = _RefCounter;
				otherptr = _rhs.AddShared();
				// �ٸ� �༮�� null �϶� 
				if (otherptr == nullptr)
				{
					if (true == CAS_POINTER(&_RefCounter, prevptr, nullptr))
					{
						if (nullptr != prevptr)
						{
							Release(prevptr);
						}
						return *this;
					}
					continue;
				}
				// ���� ���� ��
				if (otherptr == prevptr)
				{
					Release(otherptr);
					return *this;
				}
				// ���� �ٸ� ��
				if (true == CAS_POINTER(&_RefCounter, prevptr, otherptr))
				{
					if (nullptr != prevptr)
					{
						Release(prevptr);
					}
					return *this;
				}
				// ��� �����ϸ� Add �� ���� Release�ϰ� �ٽ� 
				Release(otherptr);
			}

			return *this;
		}

		T* operator->() const { return get(); }
		T& operator*() { return *get(); }

		bool operator ==(nullptr_t _ptr) const { return _RefCounter == _ptr; }
		bool operator !=(nullptr_t _ptr) const { return _RefCounter != _ptr; }

		template<class T2>
		bool operator ==(const T2*& _rhs) const { return _RefCounter == _rhs; }

		template<class T2>
		bool operator !=(const T2*& _rhs) const { return _RefCounter != _rhs; }

		template<class T2>
		bool operator ==(const USharedPtr<T2>& _rhs) const { return _RefCounter == _rhs._RefCounter; }

		template<class T2>
		bool operator !=(const USharedPtr<T2>& _rhs) const { return _RefCounter != _rhs._RefCounter; }

		T** operator&() const { &_RefCounter->get(); }

		T* get() const { if (nullptr == _RefCounter) return nullptr; return _RefCounter->get(); }
		/*
		@ Date: 2023-12-26
		@ Writer: ������
		@ Explain:  Reset �ϴ� �Լ�
		*/
		void reset()
		{
			URefCounter<T>* prevref{ nullptr };
			while (true)
			{
				prevref = _RefCounter;
				if (prevref == nullptr)
					return;

				if (true == CAS_POINTER(&_RefCounter, prevref, nullptr))
				{
					Release(prevref);
					return;
				}
			}
		}
		/*
		@ Date: 2023-12-26
		@ Writer: ������
		*/
		llong use_count()
		{
			return _RefCounter->GetUseCount();
		}
		/*
		@ Date: 2023-12-26
		@ Writer: ������
		*/
		operator bool()
		{
			URefCounter<T>* currptr = _RefCounter;

			if (nullptr != currptr)
				if (currptr->GetUseCount() > 0)
					return true;

			return false;
		}
		/*
		@ Date: 2023-12-26
		@ Writer: ������
		@ Explain:  ���۷��� ī��Ʈ ����
		*/
		URefCounter<T>* AddShared() const
		{
			URefCounter<T>* prev{ nullptr };
			URefCounter<T>* curr{ nullptr };
			URefCounter<T>* getptr{ nullptr };

			while (true)
			{
				prev = _RefCounter;
				if (nullptr == prev)
					return nullptr;

				getptr = prev->ComparePointerToStrong();
				curr = _RefCounter;
				// ���� ������ ptr�� curr ������ ������ ����
				if (getptr == curr)
					return curr;
				// �����ϸ� Release 
				if (getptr != nullptr)
					prev->ReleasePointerToStrong();
			}
			return nullptr;
		}
	private:
		/*
		@ Date: 2023-12-26
		@ Writer: ������
		@ Explain: Refcounter�� ���� NULL�� �ʱ�ȭ
		*/
		void RefValueNull()
		{
			URefCounter<T>* prev{ nullptr };
			URefCounter<T>* curr{ nullptr };

			while (true)
			{
				prev = _RefCounter;
				if (nullptr == prev)
					return;

				if (true == CAS_POINTER(&_RefCounter, prev, nullptr))
					return;
			}
		}
		/*
		@ Date: 2023-12-26
		@ Writer: ������
		@ Explain: Weak�� ����
		*/
		URefCounter<T>* AddWeak() const
		{
			URefCounter<T>* prev{ nullptr };
			URefCounter<T>* curr{ nullptr };
			URefCounter<T>* getptr{ nullptr };

			while (true)
			{
				prev = _RefCounter;
				if (nullptr == prev)
					return nullptr;

				getptr = prev->ComparePointerToWeak();
				curr = _RefCounter;

				if (getptr == curr)
					return curr;

				if (getptr != nullptr)
					prev->ReleasePointerToWeak();
			}
			return nullptr;
		}
		/*
		@ Date: 2023-12-26
		@ Writer: ������
		*/
		bool CAS_POINTER(URefCounter<T>** _Counter, URefCounter<T>* _old, URefCounter<T>* _new) const
		{
			llong value1 = reinterpret_cast<llong>(_old);
			llong value2 = reinterpret_cast<llong>(_new);
			return std::atomic_compare_exchange_strong(reinterpret_cast<std::atomic<llong>*>(_Counter), &value1, value2);
		}
		/*
		@ Date: 2023-12-26
		@ Writer: ������
		*/
		void Release(URefCounter<T>*& _ref)
		{
			if (nullptr == _ref)
				return;

			llong value = _ref->ReleasePointerToStrong();
			if (value <= 0)
			{
				_ref = nullptr;
			}
		}
		/*
		@ Date: 2023-12-26
		@ Writer: ������
		@ Explain: ��Ƽ�����忡���� �̵� �Լ� ����
		*/
		USharedPtr<T>& Move(const USharedPtr<T>& _rhs)
		{
			URefCounter<T>* prevptr{ nullptr };
			URefCounter<T>* otherptr{ nullptr };

			while (true)
			{
				prevptr = _RefCounter;
				otherptr = _rhs.AddShared();

				if (otherptr == nullptr)
				{
					if (true == CAS_POINTER(&_RefCounter, prevptr, nullptr))
					{
						if (nullptr != prevptr)
						{
							Release(prevptr);
						}
						return *this;
					}
					continue;
				}

				if (otherptr == prevptr)
				{
					Release(otherptr);
					return *this;
				}

				if (true == CAS_POINTER(&_RefCounter, prevptr, otherptr))
				{
					if (nullptr != prevptr)
					{
						Release(prevptr);
					}
					return *this;
				}
				Release(otherptr);
			}
		}

	private:
		URefCounter<T>* _RefCounter;
	};

	template<class T>
	bool operator==(nullptr_t _lhs, const USharedPtr<T>& _rhs) { return _lhs == _rhs.get(); }
	template<class T>
	bool operator!=(nullptr_t _lhs, const USharedPtr<T>& _rhs) { return _lhs != _rhs.get(); }
	template<class T, class U>
	bool operator==(U* _lhs, const USharedPtr<T>& _rhs) { return _lhs == _rhs.get(); }
	template<class T, class U>
	bool operator!=(U* _lhs, const USharedPtr<T>& _rhs) { return _lhs != _rhs.get(); }

	/*
	@ Date: 2023-12-26
	@ Writer: ������
	@ Explain: weak_ptr ��� ��Ƽ�����忡�� ����� �� �ִ� WeakPtr ����
	*/
	template<class T>
	class   UWeakPtr {
		friend class URefCounter<T>;
		friend class USharedPtr<T>;
		friend class UEnableSharedFromThis<T>;
	public:
		UWeakPtr() : _RefCounter{ nullptr } { }
		UWeakPtr(URefCounter<T>* _Ref) : _RefCounter{ _Ref->ComparePointerToWeak() } {	}
		UWeakPtr(nullptr_t) : _RefCounter{ nullptr } {}
		template<class U>
		UWeakPtr(const UWeakPtr<U>& _rhs) : _RefCounter{ _rhs.AddWeak() } {}
		UWeakPtr(const UWeakPtr& _rhs) : _RefCounter{ _rhs.AddWeak() } {	}
		UWeakPtr(UWeakPtr&& _rhs) : _RefCounter{ _rhs._RefCounter } { _rhs._RefCounter = nullptr; }
		UWeakPtr(USharedPtr<T> _sharedPtr) : _RefCounter{ _sharedPtr.AddWeak() } { }
		~UWeakPtr() { Release(_RefCounter); }
	public:
		/*
		@ Date: 2023-12-26
		@ Writer: ������
		*/
		UWeakPtr& operator =(nullptr_t)
		{
			URefCounter<T>* prevptr{ nullptr };
			while (true)
			{
				prevptr = _RefCounter;
				if (prevptr == nullptr)
					return *this;

				if (true == CAS_POINTER(&_RefCounter, prevptr, nullptr))
				{
					Release(prevptr);
					return *this;
				}
			}
			return *this;
		}
		/*
		@ Date: 2023-12-26
		@ Writer: ������
		*/
		UWeakPtr& operator =(const USharedPtr<T>& _rhs)
		{
			URefCounter<T>* prevptr{ nullptr };
			URefCounter<T>* otherptr{ nullptr };

			while (true)
			{
				prevptr = _RefCounter;
				otherptr = _rhs.AddWeak();

				if (otherptr == nullptr)
				{
					if (true == CAS_POINTER(&_RefCounter, prevptr, nullptr))
					{
						if (nullptr != prevptr)
						{
							Release(prevptr);
						}
						return *this;
					}
					continue;
				}

				if (otherptr == prevptr)
				{
					Release(otherptr);
					return *this;
				}

				if (true == CAS_POINTER(&_RefCounter, prevptr, otherptr))
				{
					if (nullptr != prevptr)
					{
						Release(prevptr);
					}
					return *this;
				}
				Release(otherptr);
			}

			return *this;
		}
		/*
		@ Date: 2023-12-26
		@ Writer: ������
		*/
		UWeakPtr& operator =(const UWeakPtr<T>& _rhs)
		{
			URefCounter<T>* prevptr{ nullptr };
			URefCounter<T>* otherptr{ nullptr };

			while (true)
			{
				prevptr = _RefCounter;
				otherptr = _rhs.AddWeak();

				if (otherptr == nullptr)
				{
					if (true == CAS_POINTER(&_RefCounter, prevptr, nullptr))
					{
						if (nullptr != prevptr)
						{
							Release(prevptr);
						}
						return *this;
					}
					continue;
				}

				if (otherptr == prevptr)
				{
					Release(otherptr);
					return *this;
				}

				if (true == CAS_POINTER(&_RefCounter, prevptr, otherptr))
				{
					if (nullptr != prevptr)
					{
						Release(prevptr);
					}
					return *this;
				}
				Release(otherptr);
			}

			return *this;
		}

		bool operator ==(nullptr_t _ptr) const { return _RefCounter == _ptr; }
		bool operator !=(nullptr_t _ptr) const { return _RefCounter != _ptr; }

		template<class T2>
		bool operator ==(const T2*& _rhs) const { return _RefCounter == _rhs; }

		template<class T2>
		bool operator !=(const T2*& _rhs) const { return _RefCounter != _rhs; }

		template<class T2>
		bool operator ==(const USharedPtr<T2>& _rhs) const { return _RefCounter == _rhs._RefCounter; }

		template<class T2>
		bool operator !=(const USharedPtr<T2>& _rhs) const { return _RefCounter != _rhs._RefCounter; }
		/*
		@ Date: 2023-12-26
		@ Writer: ������
		*/
		USharedPtr<T> lock() const { return USharedPtr<T>(*this); }
		/*
		@ Date: 2023-12-26
		@ Writer: ������
		*/
		void reset()
		{
			URefCounter<T>* prevref{ nullptr };
			while (true)
			{
				prevref = _RefCounter;
				if (prevref == nullptr)
					return;

				if (true == CAS_POINTER(&_RefCounter, prevref, nullptr))
				{
					Release(prevref);
					return;
				}
			}
		}
		/*
		@ Date: 2023-12-26
		@ Writer: ������
		*/
		llong use_count() const
		{
			return _RefCounter->GetUseCount();
		}
		/*
		@ Date: 2023-12-26
		@ Writer: ������
		*/
		operator bool() const
		{
			URefCounter<T>* currptr = _RefCounter;

			if (nullptr != currptr)
				if (currptr->GetUseCount() > 0)
					return true;

			return false;
		}
		/*
		@ Date: 2023-12-26
		@ Writer: ������
		*/
		void SetEnableShared(const USharedPtr<T>& _rhs)
		{
			_RefCounter = _rhs._RefCounter->ComparePointerToWeak();
		}
	private:
		/*
		@ Date: 2023-12-26
		@ Writer: ������
		*/
		URefCounter<T>* AddShared() const
		{
			URefCounter<T>* prev{ nullptr };
			URefCounter<T>* curr{ nullptr };
			URefCounter<T>* getptr{ nullptr };

			while (true)
			{
				prev = _RefCounter;
				if (nullptr == prev)
					return nullptr;

				getptr = prev->ComparePointerToStrong();
				curr = _RefCounter;

				if (getptr == curr)
					return curr;

				if (getptr != nullptr)
					prev->ReleasePointerToStrong();
			}
			return nullptr;
		}
		/*
		@ Date: 2023-12-26
		@ Writer: ������
		*/
		URefCounter<T>* AddWeak() const
		{
			URefCounter<T>* prev{ nullptr };
			URefCounter<T>* curr{ nullptr };
			URefCounter<T>* getptr{ nullptr };

			while (true)
			{
				prev = _RefCounter;
				if (nullptr == prev)
					return nullptr;

				getptr = prev->ComparePointerToWeak();
				curr = _RefCounter;

				if (getptr == curr)
					return curr;

				if (getptr != nullptr)
					prev->ReleasePointerToWeak();
			}
			return nullptr;
		}
		/*
		@ Date: 2023-12-26
		@ Writer: ������
		*/
		bool CAS_POINTER(URefCounter<T>** _Counter, URefCounter<T>* _old, URefCounter<T>* _new) const
		{
			llong value1 = reinterpret_cast<llong>(_old);
			llong value2 = reinterpret_cast<llong>(_new);
			return std::atomic_compare_exchange_strong(reinterpret_cast<std::atomic<llong>*>(_Counter), &value1, value2);
		}
		/*
		@ Date: 2023-12-26
		@ Writer: ������
		*/
		void Release(URefCounter<T>*& _ptr)
		{
			if (nullptr == _ptr)
				return;

			llong value = _ptr->ReleasePointerToWeak();
			if (value <= 0)
			{
				_RefCounter = nullptr;
			}
		}
	private:
		mutable URefCounter<T>* _RefCounter;
	};
	/*
	@ Date: 2023-12-26
	@ Writer: ������
	@ Explain: �ڱ��ڽ��� ����Ʈ �����Ͱ� �ʿ��� ��, �ش� Ŭ������ ��ӽ��Ѽ� �ڱ� �ڽ��� ����Ʈ �����͸� ����� �� �ְ� �Ѵ�.
	*/
	template<typename T>
	class  UEnableSharedFromThis : public  std::enable_shared_from_this<UEnableSharedFromThis<T>> {
		friend  class USharedPtr<T>;
		friend  class URefCounter<T>;
	protected:
		UEnableSharedFromThis()
			: Wptr()
		{}

		UEnableSharedFromThis(const UEnableSharedFromThis& other)
			: Wptr()
		{}

		UEnableSharedFromThis& operator=(const UEnableSharedFromThis&)
		{
			return (*this);
		}

		~UEnableSharedFromThis() = default;
	public:
		using _Esft_type = UEnableSharedFromThis;

		USharedPtr<T> shared_from_this()
		{
			return (USharedPtr<T>(Wptr));
		}

		UWeakPtr<T> weak_from_this()
		{
			return Wptr;
		}

		void SetSharedFromThis(const USharedPtr<T>& _ptr) { Wptr.SetEnableShared(_ptr); }
	private:
		UWeakPtr<T> Wptr;
	};
	/*
	@ Date: 2023-12-26
	@ Writer: ������
	@ Explain: std::shared_ptr�� �޾Ƽ� RefCounter�� ���� �� SetEnableShared����
						UEnableSharedFromThis Ŭ������ ��� �޾Ҵ��� Ȯ���Ѵ�.
	*/
	template<class T>
	USharedPtr<T> MakeShared(const std::shared_ptr<T>& _ptr)
	{
		USharedPtr<T> SharedPtr;
		URefCounter<T>* pRefCounter = new URefCounter<T>(_ptr);
		SharedPtr.SetEnableShared(_ptr, pRefCounter);
		return std::move(SharedPtr);
	}
	/*
	@ Date: 2023-12-26
	@ Writer: ������
	*/
	template<class T, class U>
	USharedPtr<T> static_shared_cast(const USharedPtr<U>& _rhs)
	{
		URefCounter<T>* RefCounter = reinterpret_cast<URefCounter<T>*>(_rhs.AddShared());
		RefCounter->SetShared(std::static_pointer_cast<T>(_rhs._RefCounter->ptr));
		return USharedPtr<T> {RefCounter};
	}
	/*
	@ Date: 2023-12-26
	@ Writer: ������
	*/
	template<class T, class U>
		requires std::is_base_of_v<T, U> || std::is_base_of_v<U, T>
	USharedPtr<T> dynamic_shared_cast(const USharedPtr<U>& _rhs)
	{
		URefCounter<T>* RefCounter = reinterpret_cast<URefCounter<T>*>(_rhs.AddShared());
		RefCounter->SetShared(std::dynamic_pointer_cast<T>(_rhs._RefCounter->ptr));
		return USharedPtr<T> {RefCounter};
	}
	/*
	@ Date: 2023-12-26
	@ Writer: ������
	*/
	template<class T, class U>
	USharedPtr<T> reinterpret_shared_cast(const USharedPtr<U>& _rhs)
	{
		URefCounter<T>* RefCounter = reinterpret_cast<URefCounter<T>*>(_rhs.AddShared());
		RefCounter->SetShared(std::reinterpret_pointer_cast<T>(_rhs._RefCounter->ptr));
		return USharedPtr<T> {RefCounter};
	}
	/*
	@ Date: 2023-12-26
	@ Writer: ������
	*/
	template<class T, class U>
	USharedPtr<T> const_shared_cast(const USharedPtr<U>& _rhs)
	{
		URefCounter<T>* RefCounter = reinterpret_cast<URefCounter<T>*>(_rhs.AddShared());
		RefCounter->SetShared(std::const_pointer_cast<T>(_rhs._RefCounter->ptr));
		return USharedPtr<T> {RefCounter};
	}
}
/*
@ Date: 2023-12-26
@ Writer: ������
@ Explain:  type ������
*/
namespace Core {

	template<class T>
	using SHPTR = __declspec(align(Core::CASH_ALGIN_VALUE)) USharedPtr<T>;

	template<class T>
	using CSHPTR = const SHPTR<T>;

	template<class T>
	using CSHPTRREF = const SHPTR<T>&;

	template<class T>
	using WKPTR = __declspec(align(Core::CASH_ALGIN_VALUE))   UWeakPtr<T>;

	template<class T>
	using CWKPTR = const WKPTR<T>;

	template<class T>
	using CWKPTRREF = const WKPTR<T>&;
}

#define CACHE_ALGIN  __declspec(align(Core::CASH_ALGIN_VALUE)) 


namespace std
{
	/*
	@ Date: 2023-12-26
	@ Writer: ������
	@ Explain:  HASH opeartor() ���꿡 ���� SharedPtr ������, concurrent_ �����̳ʿ� ����ϱ� ����
	*/
	template <typename T>
	struct hash<Core::USharedPtr<T>> {
		size_t operator()(const Core::USharedPtr<T>& ptr) const {
			return std::hash<T*>()(ptr.get());
		}
	};
	/*
	@ Date: 2023-12-26
	@ Writer: ������
	@ Explain:  Equal ���꿡 ���� SharedPtr ������, concurrent_ �����̳ʿ� ����ϱ� ����
	*/
	template <typename T>
	struct equal_to<Core::USharedPtr<T>> {
		bool operator()(const Core::USharedPtr<T>& lhs, const Core::USharedPtr<T>& rhs) const {
			return lhs.get() == rhs.get();
		}
	};
	/*
	@ Date: 2023-12-26
	@ Writer: ������
	@ Explain:  less ���꿡 ���� SharedPtr ������, concurrent_ �����̳ʿ� ����ϱ� ����
	*/
	template <typename T>
	struct less<Core::USharedPtr<T>> {
		bool operator()(const Core::USharedPtr<T>& lhs, Core::USharedPtr<T>& rhs) const {
			return lhs.get() < rhs.get();
		}
	};
}

#endif // _SERVERFRAMEWORK_CORE_PUBLIC_SMARTPOINTER_H