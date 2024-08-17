#ifndef _SERVERFRAMEWORK_CORE_PUBLIC_COREFUNCTION_H
#define _SERVERFRAMEWORK_CORE_PUBLIC_COREFUNCTION_H

namespace Core
{
#pragma region FUNCTION 

	/*
	@ Date: 2024-01-10,  Writer: ������
	@ Explain
	- ���� ���� ����ϴ� �Լ��̴�.
	*/
	template<class T>
	T* CreateAndRegisterApp()
	{
		T* App = new T;
		Core::g_RegisterApp = App;
		return App;
	}
	/*
	@ Date: 2024-01-09,  Writer: ������
	@ Explain
	- ���� �и������带 ���ϴ� �Լ��̴�. 
	*/
	static _llong CurrentMilliseconds()
	{
		auto Time = std::chrono::high_resolution_clock::now();
		return std::chrono::duration_cast<std::chrono::milliseconds>(Time.time_since_epoch()).count();
	}
	/*
	@ Date: 2024-01-13,  Writer: ������
	@ Explain
	- ���� �и������带 ���ϴ� �Լ��̴�.
	*/
	static _llong CurrentTime()
	{
		auto Time = std::chrono::system_clock::now();
		return std::chrono::duration_cast<std::chrono::milliseconds>(Time.time_since_epoch()).count();
	}

	/*
	@ Date: 2024-01-05,  Writer: ������
	@ Explain
	- _temp�� _Value�� �Ѿ����� Ȯ���ϴ� �Լ�
	*/
	template<class T, class Value >
	requires (CheckNumber<T> && CheckNumber<Value>)
	static T CheckOverValue(const T& _temp, const Value& _Value)
	{
		T convert = static_cast<T>(_Value);
		if (_temp >= convert)
		{
			return convert;
		}
		return _temp;
	}
	/*
	@ Date: 2024-01-05,  Writer: ������
	@ Explain
	- �޸𸮸� �ʱ�ȭ�ϱ� ���� �Լ�, Window�Լ��� ZeroMemory�� ���� �ʱ� �����̴�.
	*/
	template<class T>
	static void MemoryInitialization(T* _pData, const size_t _bufferSize)
	{
		constexpr static _int ZEROMEMORY{ 0 };
		std::memset(_pData, ZEROMEMORY, _bufferSize);
	}

	/*
	@ Date: 2024-01-05,  Writer: ������
	@ Explain
	- �޸𸮸� �ʱ�ȭ�ϱ� ���� �Լ�, Window�Լ��� ZeroMemory�� ���� �ʱ� �����̴�.
	*/
	template<class T>
	static void MemoryInitialization(T& _Data)
	{
		constexpr static _int ZEROMEMORY{ 0 };
		std::memset(&_Data, ZEROMEMORY, sizeof(T));
	}

	/*
	@ Date: 2024-01-05,  Writer: ������
	@ Explain
	- �ش� Thread�� nanoseconds��ŭ ���� �Լ��̴�.
	*/
	template<class T>
	requires std::is_integral_v<T>
	static void ThreadNanoRelax(const T& _data)
	{
		std::this_thread::sleep_for(std::chrono::nanoseconds(_data));
	}
	/*
	@ Date: 2024-01-05,  Writer: ������
	@ Explain
	- �ش� Thread�� MicroSeconds��ŭ ���� �Լ��̴�.
	*/
	template<class T>
		requires std::is_integral_v<T>
	static void ThreadMicroRelax(const T& _data)
	{
		std::this_thread::sleep_for(std::chrono::microseconds(_data));
	}

	/*
	@ Date: 2024-01-05,  Writer: ������
	@ Explain
	- �ش� Thread�� MiliSeconds��ŭ ���� �Լ��̴�.
	*/
	template<class T>
		requires std::is_integral_v<T>
	static void ThreadMiliRelax(const T& _data)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(_data));
	}
	/*
	@ Date: 2023-12-26
	@ Writer: ������
	@ Explain
	- _origin�� ���� _old�� ���� �� _weak�� �ְ� true ����
	*/
	template<class T>
	static bool CAS_VALUE(std::atomic<T>& _origin, T _old, T _weak)
	{
		return _origin.compare_exchange_strong(_old, _weak);
	}
	/*
	@ Date: 2024-01-02,  Writer: ������
	@ Explain
	- std::make_pair�� ���� �Ⱦ ����ϴ� �Լ�
	*/
	template<class T1, class T2>
	static std::pair<T1, T2> MakePair(const T1& _t1, const T2& _t2) { return std::move(std::pair<T1, T2>(_t1, _t2)); }

	/*
	@ Date: 2024-02-04,  Writer: ������
	@ Explain
	- sizeof�� ����, ���� ���� ȣ���ε� �� �Լ��� ���� ���� ���� ȣ���̶� ���ο� �ӽ� ������ ������ �ʴ´�.
	*/
	template<class Type>
	static int GetTypeSize() { constexpr static int SIZE{ sizeof(Type) }; return SIZE; }
	/*
	@ Date: 2024-02-04,  Writer: ������
	@ Explain
	- ���� ���� num�� �����ŭ�� ���� �����ϴ� �Լ�
	*/
	template<int Number, class T>
		requires CheckNumber<T>
	static T MakeMultipleNumber(T number) {
		// 16�� ����� ����� ���� 15�� ���ϰ� 16���� �����ϴ�.
		T remainder = number % Number;
		if (remainder != 0) {
			number += (Number - remainder);
		}
		return number;
	}
	/*
	@ Date: 2024-02-04,  Writer: ������
	@ Explain
	- ���� ���� Divine ���ִ� �Լ�
	*/
	template<int Number, class T>
		requires CheckNumber<T>
	static T GetDivineNumber(T number) {
		return static_cast<T>(number / Number);
	}

	/*
	@ Date: 2024-12-26, Writer: ������
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
	/*
	@ Date: 2024-04-06, Writer: ������
	@ Explain
	- char�� �����ϴ� �Լ��̴�.
	*/
	static _string CombineRawString(const _char* _p1, const _char* _p2)
	{
		_string str(_p1);
		str += _p2;
		return std::move(str);
	}
	/*
	@ Date: 2024-07-03, Writer: ������
	@ Explain
	- wstring�� �׳� string���� ��ȯ�ϴ� �Լ�
	*/
	static _string ConvertWToS(const _wstring& var)
	{
		_string str;
		size_t size;
		str.resize(var.size());
		wcstombs_s(&size, &str[0], str.size() + 1, var.c_str(), var.size());
		return std::move(str);
	}
	/*
	@ Date: 2024-07-03, Writer: ������
	@ Explain
	- wstring�� �׳� string���� ��ȯ�ϴ� �Լ�
	*/
	static _wstring ConvertSToW(const _string& var)
	{
		_wstring wstr;
		size_t size;
		wstr.resize(var.size());
		mbstowcs_s(&size, wstr.data(), wstr.size() + 1, var.c_str(), var.size());
		return std::move(wstr);
	}
	/*
	@ Date: 2024-07-03, Writer: ������
	@ Explain
	- string�� �д� �Լ� 
	*/
	static void ReadString(std::ifstream& _if, _wstring& _wstr) 
	{
		_int size = 0;
		_if.read((_char*)&size, sizeof(int));
		_char* pText = AllocBuffer<_char>(size);
		::memset(pText, 0, static_cast<size_t>(size));
		_if.read((_char*)pText, size);
		_wstr = ConvertSToW(pText);
		ReleaseBuffer(pText);
	}
	/*
	@ Date: 2024-07-03, Writer: ������
	@ Explain
	- string�� �д� �Լ�
	*/
	static void ReadString(std::ifstream& _if, _string& _str)
	{
		_int size = 0;
		_if.read((_char*)&size, sizeof(int));
		_char* pText = AllocBuffer<_char>(size);
		::memset(pText, 0, static_cast<size_t>(size));
		_if.read((_char*)pText, size);
		_str = pText;
		ReleaseBuffer(pText);
	}

	static void SaveString(std::ofstream& _os, const _string& _str)
	{
		int size = (int)_str.length() + 1;
		_os.write((char*)&size, sizeof(int));
		_os.write((char*)_str.c_str(), size);
	}
	static void SaveString(std::ofstream& _os, const _wstring& _wstr)
	{
		SaveString(_os, ConvertWToS(_wstr));
	}

	template<class T>
	static void ChangeAtomicValue(std::atomic<T>& _value, T _change)
	{
		while (true)
		{
			T value = _value.load();

			if (true == CAS_VALUE(_value, value, _change))
			{
				return;
			}
		}
	}

#pragma endregion FUNCTION
}

#endif // _SERVERFRAMEWORK_CORE_PUBLIC_COREFUNCTION_H