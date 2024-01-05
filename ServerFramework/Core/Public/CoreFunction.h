#ifndef _SERVERFRAMEWORK_CORE_PUBLIC_COREFUNCTION_H
#define _SERVERFRAMEWORK_CORE_PUBLIC_COREFUNCTION_H

namespace Core
{
#pragma region FUNCTION 

	template<class T, class Value >
	requires (Number<T> && Number<Value>)
	T CheckOverValue(const T& _temp, const Value& _Value)
	{
		if (_temp >= _Value)
		{
			return static_cast<T>(_Value);
		}
		return _temp;
	}

	template<class T>
	void MemoryInitialization(T* _pData, const _int _bufferSize)
	{
		std::memset(_pData, 0, _bufferSize);
	}

	template<class T>
	requires std::is_integral_v<T>
	void ThreadRelax(const T& _data)
	{
		std::this_thread::sleep_for(std::chrono::nanoseconds(_data));
	}

	/*
	@ Date: 2023-01-02,  Writer: ������
	@ Explain
	- ��Ƽ�����忡�� �����ϰ� ���� �ٲٱ� ���� �Լ�
	*/
	template<class T, class Value>
	bool CAS(T* _pData, Value _expected, const Value& _value) {
		return std::atomic_compare_exchange_strong(
			reinterpret_cast<volatile ATOMIC<T>*>(_pData), &_expected, _value);
	}
	
	/*
	@ Date: 2023-01-02,  Writer: ������
	@ Explain
	- ��Ƽ�����忡�� �����ϰ� ���� �ٲٱ� ���� �Լ�
	*/
	template<class T, class Value>
	bool CAS(REF_IN std::atomic<T>& _Data, Value _expected, const Value& _value) {
		return _Data.compare_exchange_strong(_expected, _value);
	}

	template<class T1, class T2>
	std::pair<T1, T2> MakePair(T1& _t1, T2& _t2) { return std::move(std::pair<T1, T2>(_t1, _t2)); }

	/*
	@ Date: 2023-12-26, Writer: ������
	@ Explain
	- �ܼ��� ��ü�� ����� �Լ��̴�, 
	*/
	template<class T, class... Args>
		requires ConstructArgsCheck<T, Args...>
	static SHPTR<T> Create(Args&&... args) {
		SHPTR<T> pInstance = Core::MakeShared<T>(std::forward<Args>(args)...);
		return std::move(pInstance);
	}
	/*
	@ Date: 2023-12-26, Writer: ������
	@ Explain
	- NatvieConstruct �Լ��� ȣ�� �� �� , �����ϸ� nullptr ����, �Լ����� �ʱ�ȭ
	*/
	template<class T, class ...Args>
		requires	CheckToSameMethodArgs<T, Args...>
	static SHPTR<T> CreateInitNative(Args&&... args) {
		static_assert(CheckToSameMethodArgs<T, Args...>, "NativeConstruct�� ���ڰ� �߸��Ǿ����ϴ�.");
		SHPTR<T> pInstance{ Core::MakeShared<T>() };
		if (false ==  (pInstance->NativeConstruct(args...))) {
#ifdef USE_DEBUG
			CRASH("SHPTR<T>::CreateInitNative");
#endif
			pInstance.reset();
		}
		return std::move(pInstance);
	}
	/*
	@ Date: 2023-12-26, Writer: ������
	@ Explain
	- CreateInitNative ����, �ٸ� �޽����� ������� ����
	*/
	template<class T, class ...Args>
		requires CheckToSameMethodArgs<T, Args...>
	static SHPTR<T> CreateInitNativeNotMsg(Args&&... args) {
		static_assert(CheckToSameMethodArgs<T, Args...>, "NativeConstruct�� ���ڰ� �߸��Ǿ����ϴ�.");
		SHPTR<T> pInstance{ Core::MakeShared<T>() };
		if (false == (pInstance->NativeConstruct(args...))) {
			pInstance.reset();
		}
		return std::move(pInstance);
	}
	/*
	@ Date: 2023-12-26, Writer: ������
	@ Explain
	- NatvieConstruct �Լ��� ȣ�� �� �� , �����ϸ� nullptr ����, �����ڿ��� �ʱ�ȭ, 
	Natvie�Լ��� �ִ��� Ȯ�� ��, �������� ������ Args...�� ������ �������� �Ǵ�.
	*/
	template<class T, class ...Args>
		requires ConstructArgsCheck<T, Args...>
	&& CheckToSameMethodArgs<T>
		static SHPTR<T> CreateInitConstructor(Args&&... args) {
		static_assert(ConstructArgsCheck<T, Args...>, "�������� ���ڰ� �߸��Ǿ����ϴ�.");
		SHPTR<T> pInstance{ Core::MakeShared<T>(std::forward<Args>(args)...) };
		if (false == (pInstance->NativeConstruct())) {
#ifdef USE_DEBUG
			CRASH("SHPTR<T>::CreateInitConstructor");
#endif
			pInstance.reset();
		}
		return std::move(pInstance);
	}
	/*
	@ Date: 2023-12-26, Writer: ������
	@ Explain
	- CreateInitConstructor ����, �ٸ� �޽����� ������� ����
	*/
	template<class T, class ...Args>
		requires ConstructArgsCheck<T, Args...>
	&& CheckToSameMethodArgs<T>
		static SHPTR<T>  CreateInitConstructorNotMsg(Args&&... args) {
		static_assert(ConstructArgsCheck<T, Args...>, "�������� ���ڰ� �߸��Ǿ����ϴ�.");
		SHPTR<T> pInstance{ Core::MakeShared<T>(std::forward<Args>(args)...) };
		if (false ==  (pInstance->NativeConstruct())) {
			pInstance.reset();
		}
		return std::move(pInstance);
	}
	/*
	@ Date: 2023-12-26, Writer: ������
	@ Explain
	- �ڱ� �ڽ��� Ŭ���ϴ� �Լ��̴�. 
	*/
	template<class T>
	static SHPTR<T> CloneThis(const T& _rhs)
	{
		SHPTR<T> pInstance{ Core::MakeShared<T>(_rhs) };
		return std::move(pInstance);
	}

#pragma endregion FUNCTION
}

#endif // _SERVERFRAMEWORK_CORE_PUBLIC_COREFUNCTION_H