#pragma once
namespace Engine {

	template <typename... Args>
	constexpr char* GetArgsTypeName()
	{
		return typeid(Args...).name();
	}


#pragma region FUNCTION 
	template<typename T>
	void Safe_Delete(T& _t) {
		delete _t;
		_t = nullptr;
	}

	template<typename T>
	void Safe_Delete_Array(T& _t) {
		delete[] _t;
		_t = nullptr;
	}

	template<typename T>
	bool Is_Nullptr_Debug(T& _data, const char* FILE,
		const unsigned int& LINE, const char* FUNC)
	{
		if (nullptr == _data)
		{
			char	szMessage[MAX_PATH] = "";
			strcpy_s(szMessage, typeid(T).name());
			_string ClassName = szMessage;
			_string str = " Is Nullptr";
			str += "\nFILE: ";
			str += FILE;
			str += "\n LINE: ";
			str += std::to_string(LINE);
			str += "\n";
			str += FUNC;
			ClassName.append(str);
#ifdef _DEBUG
			MessageBoxA(0, ClassName.c_str(), nullptr, MB_OK);
#endif
			return true;
		}
		return false;
	}


	static void Debugging_Message(const char* FILE,
		const unsigned int& LINE, const char* FUNC, const _string& _pMessage)
	{
#ifdef _DEBUG
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

	static void Check_Error(const HRESULT& _eResult, const char* FILE,
		const unsigned int& LINE, const char* FUNC) {
		_string str = std::to_string(_eResult);
		str += "\nFILE: ";
		str += FILE;
		str += "\n LINE: ";
		str += std::to_string(LINE);
		str += "\n";
		str += FUNC;
		MessageBoxA(0, str.c_str(), nullptr, MB_OK);
	}

	template<typename T>
	static void ErrorToCreateClass(T& _rhs) {
		MessageBoxA(0, typeid(T).name(), nullptr, MB_OK);
	}

	// �����ڰ� �����ϴ��� üũ�ϰ� �����ϸ� �����ϴ� �Լ� NativeConstruct �Լ��� ��� �ȴ�. 
	template<class T, class ...Args>
		requires ConstructWidthArgsCheck<T, Args...>
	static SHPTR<T> Create(Args&&... args) {
		SHPTR<T> pInstance{ Make::MakeShared<T>(args...) };
		return pInstance;
	}

	// �Լ����� ���� �޾ƿͼ� ��ü�� ����� ���� ��
	template<class T, class ...Args>
		requires	CheckToSameMethodArgs<T, Args...>
	static SHPTR<T> CreateNative(Args&&... args) {
		static_assert(CheckToSameMethodArgs<T, Args...>, "NativeConstruct�� ���ڰ� �߸��Ǿ����ϴ�.");
		SHPTR<T> pInstance{ Make::MakeShared<T>() };
		if (FAILED(pInstance->NativeConstruct(args...))) {
			ErrorToCreateClass(pInstance);
			pInstance.reset();
		}
		return pInstance;
	}

	// �Լ����� ���� �޾ƿͼ� ��ü�� ����� ���� ��
	template<class T, class ...Args>
		requires CheckToSameMethodArgs<T, Args...>
	static SHPTR<T> CreateNativeNotMsg(Args&&... args) {
		static_assert(CheckToSameMethodArgs<T, Args...>, "NativeConstruct�� ���ڰ� �߸��Ǿ����ϴ�.");
		SHPTR<T> pInstance{ Make::MakeShared<T>() };
		if (FAILED(pInstance->NativeConstruct(args...))) {
			pInstance.reset();
		}
		return pInstance;
	}

	// �����ڷ� Device, GpuCommand�� �ް� ������ ������ NativeCosntruct �Լ����� �޴� ��
	template<class T, class ...Args>
		requires  CheckToSameMethodArgs<T, Args...>
	static SHPTR<T> CreateConstructorNative(CSHPTRREF<class UDevice> _pDevice, Args&&... args) {
		SHPTR<T> pInstance{ Make::MakeShared<T>(_pDevice) };
		if (FAILED(pInstance->NativeConstruct(args...))) {
			ErrorToCreateClass(pInstance);
			pInstance.reset();
		}
		return pInstance;
	}

	// �����ڷ� Device, GpuCommand�� �ް� ������ ������ NativeCosntruct �Լ����� �޴� ��
	template<class T, class ...Args>
		requires CheckToSameMethodArgs<T, Args...>
	static SHPTR<T> CreateConstructorNativeNotMsg(CSHPTRREF<class UDevice> _pDevice, Args&&... args) {
		SHPTR<T> pInstance{ Make::MakeShared<T>(_pDevice) };
		if (FAILED(pInstance->NativeConstruct(args...))) {
			pInstance.reset();
		}
		return pInstance;
	}

	// �����ڿ����� ���� �޾ƿ��� ��
	template<class T, class ...Args>
		requires ConstructWidthArgsCheck<T, Args...>
	&& CheckToSameMethodArgs<T>
		static SHPTR<T> CreateConstructorToNative(Args&&... args) {
		;
		static_assert(ConstructWidthArgsCheck<T, Args...>, "�������� ���ڰ� �߸��Ǿ����ϴ�.");
		SHPTR<T> pInstance{ Make::MakeShared<T>(args...) };
		if (FAILED(pInstance->NativeConstruct())) {
			ErrorToCreateClass(pInstance);
			pInstance.reset();
		}
		return pInstance;
	}

	// �����ڿ����� ���� �޾ƿ��� ��
	template<class T, class ...Args>
		requires ConstructWidthArgsCheck<T, Args...>
	&& CheckToSameMethodArgs<T>
		static SHPTR<T> CreateConstructorToNativeNotMsg(Args&&... args) {
		static_assert(ConstructWidthArgsCheck<T, Args...>, "�������� ���ڰ� �߸��Ǿ����ϴ�.");
		SHPTR<T> pInstance{ Make::MakeShared<T>(args...) };
		if (FAILED(pInstance->NativeConstruct())) {
			pInstance.reset();
		}
		return pInstance;
	}

	// �ڱ� �ڽ��� Ŭ���ϴ� �Լ� 
	template<class T>
	static SHPTR<T> CloneThis(const T& _rhs)
	{
		SHPTR<T> pInstance{ Make::MakeShared<T>(_rhs) };
		return pInstance;
	}

	/*
	@ Date: 2023-01-09,  Writer: ������
	@ Explain
	- ���� �и������带 ���ϴ� �Լ��̴�.
	*/
	static _llong CurrentMilliseconds()
	{
		auto Time = std::chrono::high_resolution_clock::now();
		return std::chrono::duration_cast<std::chrono::milliseconds>(Time.time_since_epoch()).count();
	}

	/*
	@ Date: 2023-01-05,  Writer: ������
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
	@ Date: 2023-01-05,  Writer: ������
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
`@ Date: 2023-01-05,  Writer: ������
`@ Explain
`- �ش� Thread�� MicroSeconds��ŭ ���� �Լ��̴�.
`*/
	template<class T>
		requires std::is_integral_v<T>
	static void ThreadMiliRelax(const T& _data)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(_data));
	}

#ifdef _USE_ATOMIC
	template<class T, class U>
	SHPTR<T> ATOM_STATIC_CAST(const SHPTR<U>& _ptr)
	{
		return std::static_pointer_cast<T>(_ptr.getShared());
	}

	template<class T, class U>
	SHPTR<T> ATOM_DYNAMIC_CAST(const SHPTR<U>& _ptr)
	{
		return std::dynamic_pointer_cast<T>(_ptr.getShared());
	}
#else
	template<class T, class U>
	SHPTR<T>CUS_STATIC_CAST(const SHPTR<U>& _ptr)
	{
		return SHPTR<T>(_ptr, static_cast<T*>(_ptr.get()));
	}

	template<class T, class U>
	SHPTR<T> CUS_DYNAMIC_CAST(const SHPTR<U>& _ptr)
	{
		if (auto* ptr = dynamic_cast<typename SHPTR<T>::element_type*>(_ptr.get())) {
			return SHPTR<T>(_ptr, ptr);
		}
		else {
			return SHPTR<T>();
		}
	}
#endif
#pragma endregion FUNCTION
}