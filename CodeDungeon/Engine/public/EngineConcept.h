#pragma once
#include <concepts>
#include <iostream>

namespace Engine
{
	template <class T, class U>
	concept SameType = std::is_same_v<T, U>();
	template <class T, class U>
	concept ParentsChildType = std::is_base_of_v<T*, U*>() || std::is_base_of_v<U*, T*>();
	template<class T, class U>
	concept Convertible = std::is_convertible_v<T*, U*>();

	template <typename T>
	concept SharedPtrConvertible = requires(T t) {
		{ std::shared_ptr<T>(std::move(t)) };
	};
	/*
	@ Date: 2024-02-04, Writer: ������
	@ Explain: ProtoType Ŭ�������� üũ�ϴ� concept
	*/
	template<typename T>
	concept CheckProtoType = requires(T * _t) {
		{_t->ByteSizeLong() }->std::same_as<size_t>;
	};
	template <typename T, typename U>
	concept ConvertibleFrom = (SameType<T, U> || ParentsChildType<T, U> || Convertible<T, U>);

	// �� ConCept�� ������ ���ڵ��� ����� �ִ��� Ȯ���ϴ� ConCept�̴�. 
	template<typename T, class ...Args>
	concept ConstructWidthArgsCheck = std::is_constructible_v<T, Args...>;

	// �����ڰ� �ִ��� üũ
	template <typename T, typename DevicePtr, typename GpuCommandPtr>
	concept ConstructArgsMatch = std::is_constructible_v<T, DevicePtr, GpuCommandPtr>;

	// �� Conecept�� T::NativeConstruct�� ���ڰ� �´��� Ȯ���ϴ� Concept�̴�. 
	template <typename T, typename... Args>

	// �ش� �Լ��� �ִ��� Ȯ���ؼ� ���ڵ��� ���Ѵ�. �׸��� HRESULT ��� ��ȯ ���� �������� üũ
	concept CheckToSameMethodArgs = requires(T * t, Args&&... args) {
		{ t->NativeConstruct(args...) } -> std::same_as<HRESULT>;
	};

	/*
	@ Date: 2024-02-25, Writer: ������
	@ Explain: Pointer���� Ȯ���ϴ� Concept
	*/
	template<class T>
	concept IsPointer = true == std::is_pointer_v<T>;
	/*
	@ Date: 2024-02-25, Writer: ������
	@ Explain: Pointer�� �ƴ��� Ȯ���ϴ� Concept 
	*/
	template<class T>
	concept IsNotPointer = false == IsPointer<T>;

	/*
	@ Date: 2024-02-05, Writer: ������
	@ Explain: filesystem���� ��ȯ �������� Ȯ���ϴ� concept
	*/
	template <class TChar>
	concept ConvertibleTCharToPath= requires(const TChar * str) {
		std::filesystem::path{ str };
	};
	/*
	@ Date: 2024-02-05, Writer: ������
	@ Explain: filesystem���� ��ȯ �������� Ȯ���ϴ� concept
	*/
	template <class TString>
	concept ConvertibleTStringToPath = requires(const TString& str) {
		std::filesystem::path{ str };
	};

	/*
	@ Date: 2024-02-05, Writer: ������
	@ Explain: String���� Ȯ���ϴ� concept
	*/
	template<class T>
	concept CheckStdStrings =  std::is_same_v<T, std::string> || std::is_same_v<T, std::wstring>;

	/*
	@ Date: 2024-02-05, Writer: ������
	@ Explain: char, wchar_t ���� Ȯ���ϴ� concept 
	*/
	template<class T>
	concept CheckChar = std::is_same_v<T, char> || std::is_same_v<T, wchar_t>;
	/*
	@ Date: 2024-02-05, Writer: ������
	@ Explain: CustomString �� Concept���� filesystem���� ��ȯ ��������, TString�� ���� std::string, std::wstring �´��� Ȯ��
	*/
	template<class TChar, class TString>
	concept CheckCustomString = CheckStdStrings<TString> || CheckChar<TChar> || ConvertibleTCharToPath<TChar> || ConvertibleTStringToPath<TString>;

	/*
	@ Date: 2024-02-03, Writer: ������
	@ Explain: �ش��ϴ� Template�� �������� Ȯ��
	*/
	template<class T>
	concept CheckNumber = std::is_integral_v<T> || std::is_floating_point_v<T>;

	template<class T>
	concept CheckAtomicNumber = std::is_same_v<T, std::atomic<unsigned int>> || std::is_same_v<T, std::atomic<float>>
		|| std::is_same_v<T, std::atomic<short>> || std::is_same_v<T, std::atomic<long>> || std::is_same_v<T, std::atomic<unsigned short>>
		|| std::is_same_v<T, std::atomic<int>> || std::is_same_v<T, std::atomic<unsigned long>> || std::is_same_v<T, std::atomic<unsigned long long>>
		|| std::is_same_v<T, std::atomic<long long>> || std::is_same_v<T, std::atomic<unsigned char>> || std::is_same_v<T, std::atomic<signed char>>;

	template <typename T, typename U>
	concept SharedPtrConvertibleFrom = SharedPtrConvertible<U> && (SameType<T, U> || ParentsChildType<T, U>);
	template<typename T, typename U>
	concept AtomicPtrConvertibleFrom = (SameType<T, U> || ParentsChildType<T, U>);

}

