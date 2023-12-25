#ifndef _SERVERFRAMEWORK_CORE_PUBLIC_COREFUNCTION_H
#define _SERVERFRAMEWORK_CORE_PUBLIC_COREFUNCTION_H

namespace Core
{
#pragma region FUNCTION 
#ifdef USE_DEBUG
	namespace DEBUG
	{
		/*
		@ Date: 2023-12-26
		@ Writer: ������
		@ Explain: �Ϻη� ũ������ ������ ������ ���� �Լ�, USE_DEBUG������ ���
		*/
		template<class T>
		void Crash(T&& _value)
		{
			_uint* Crash{ nullptr };
			__analysis_assume(nullptr != Crash);
			*Crash = 0xDEADBEEF;
		}
		/*
		@ Date: 2023-12-26
		@ Writer: ������
		@ Explain: ������ ����Ǹ� Crash�� ���� �Լ�, USE_DEBUG������ ���
		*/
		template<class T>
		void AssertCrash(T&& _value, const bool _isCondition)
		{
			if (true == _isCondition)
			{
				Crash(std::forward<T>(_value));
			}
		}
		/*
		@ Date: 2023-12-26
		@ Writer: ������
		@ Explain: ����� �޽����� ����ϴ� �Լ�, USE_DEBUG������ ���
		*/
		static void Debugging_Message(const char* FILE,
			const unsigned int& LINE, const char* FUNC, const _string& _pMessage)
		{
#ifdef USE_DEBUG
			char	szMessage[MAX_PATH] = "";
			_string ClassName = _pMessage;
			_string str = "";
			str += "\nFILE: ";
			str += FILE;
			str += "\n LINE: ";
			str += std::to_string(LINE);
			str += "\n";
			str += FUNC;
			ClassName.append(str);
			MessageBoxA(0, ClassName.c_str(), nullptr, MB_OK);
#endif
		}
		/*
		@ Date: 2023-12-26
		@ Writer: ������
		@ Explain: ��ü�� ������ ��, ������ �ִ� Ŭ���������� �ش� Ŭ������ type�� Ȯ���Ͽ� �޽����� ���� �Լ�, USE_DEBUG������ ���
		*/
		template<typename T>
		static void ErrorToCreateClass(T& _rhs) {
			MessageBoxA(0, typeid(T).name(), nullptr, MB_OK);
		}
	}
#endif
	/*
	@ Date: 2023-12-26
	@ Writer: ������
	@ Explain: �ܼ��� ��ü�� ����� �Լ��̴�, �������� �������� ����� ���Դ��� Ȯ���ϰ� �ȵ������� ���� ���, USE_DEBUG������ ���
	*/
	template<class T, class...Args>
		requires ConstructWidthArgsCheck<T, Args...>
	static SHPTR<T> Create(Args&&... args) {
		SHPTR<T> pInstance = Core::PTH::MakeShared<T>(std::make_shared<T>(std::forward<Args>(args)...));
		return pInstance;
	}
	/*
	@ Date: 2023-12-26
	@ Writer: ������
	@ Explain: NatvieConstruct �Լ��� ȣ�� �� �� , �����ϸ� nullptr ����, �Լ����� �ʱ�ȭ
	*/
	template<class T, class ...Args>
		requires	CheckToSameMethodArgs<T, Args...>
	static SHPTR<T> CreateInitNative(Args&&... args) {
		static_assert(CheckToSameMethodArgs<T, Args...>, "NativeConstruct�� ���ڰ� �߸��Ǿ����ϴ�.");
		SHPTR<T> pInstance{ Core::PTH::MakeShared<T>(std::make_shared<T>()) };
		if (false ==  (pInstance->NativeConstruct(args...))) {
#ifdef USE_DEBUG
			DEBUG::ErrorToCreateClass(pInstance);
#endif
			pInstance.reset();
		}
		return pInstance;
	}


	/*
	@ Date: 2023-12-26
	@ Writer: ������
	@ Explain: CreateInitNative ����, �ٸ� �޽����� ������� ����
	*/
	template<class T, class ...Args>
		requires CheckToSameMethodArgs<T, Args...>
	static SHPTR<T> CreateInitNativeNotMsg(Args&&... args) {
		static_assert(CheckToSameMethodArgs<T, Args...>, "NativeConstruct�� ���ڰ� �߸��Ǿ����ϴ�.");
		SHPTR<T> pInstance{ Core::PTH::MakeShared<T>(std::make_shared<T>()) };
		if (false == (pInstance->NativeConstruct(args...))) {
			pInstance.reset();
		}
		return pInstance;
	}
	/*
	@ Date: 2023-12-26
	@ Writer: ������
	@ Explain: NatvieConstruct �Լ��� ȣ�� �� �� , �����ϸ� nullptr ����, �����ڿ��� �ʱ�ȭ, 
						Natvie�Լ��� �ִ��� Ȯ�� ��, �������� ������ Args...�� ������ �������� �Ǵ�.
	*/
	template<class T, class ...Args>
		requires ConstructWidthArgsCheck<T, Args...>
	&& CheckToSameMethodArgs<T>
		static SHPTR<T> CreateInitConstructor(Args&&... args) {
		;
		static_assert(ConstructWidthArgsCheck<T, Args...>, "�������� ���ڰ� �߸��Ǿ����ϴ�.");
		SHPTR<T> pInstance{ Core::PTH::MakeShared<T>(std::make_shared<T>(std::forward<Args>(args)...)) };
		if (false == (pInstance->NativeConstruct())) {
			ErrorToCreateClass(pInstance);
			pInstance.reset();
		}
		return pInstance;
	}
	/*
	@ Date: 2023-12-26
	@ Writer: ������
	@ Explain: CreateInitConstructor ����, �ٸ� �޽����� ������� ����
	*/
	template<class T, class ...Args>
		requires ConstructWidthArgsCheck<T, Args...>
	&& CheckToSameMethodArgs<T>
		static SHPTR<T>  CreateInitConstructorNotMsg(Args&&... args) {
		static_assert(ConstructWidthArgsCheck<T, Args...>, "�������� ���ڰ� �߸��Ǿ����ϴ�.");
		SHPTR<T> pInstance{ Core::PTH::MakeShared<T>(std::make_shared<T>(std::forward<Args>(args)...)) };
		if (false ==  (pInstance->NativeConstruct())) {
			pInstance.reset();
		}
		return pInstance;
	}
	/*
	@ Date: 2023-12-26
	@ Writer: ������
	@ Explain: �ڱ� �ڽ��� Ŭ���ϴ� �Լ��̴�. 
	*/
	template<class T>
	static SHPTR<T> CloneThis(const T& _rhs)
	{
		SHPTR<T> pInstance{ Core::PTH::MakeShared<T>(std::make_shared<T>(_rhs)) };
		return pInstance;
	}

#pragma endregion FUNCTION
}

#endif // _SERVERFRAMEWORK_CORE_PUBLIC_COREFUNCTION_H