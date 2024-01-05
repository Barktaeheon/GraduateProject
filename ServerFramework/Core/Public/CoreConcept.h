#ifndef _SERVERFRAMEWORK_CORE_PUBLIC_CORECONCEPT_H
#define _SERVERFRAMEWORK_CORE_PUBLIC_CORECONCEPT_H

#include <iostream>
#include <concepts>

namespace Core
{
	/*
	@ Date: 2024-01-05, Writer: ������
	@ Explain: �ش��ϴ� Template�� �������� Ȯ��
	*/
	template <typename T>
	concept Number = std::integral<T> || std::floating_point<T>;

	/*
@ Date: 2023-12-26, Writer: ������
@ Explain: ���� ������, �θ� Ÿ������, operator = ���� ���� �������� 
*/
	template <class T, class U>
	concept SameType = std::is_same_v<T, U>();
	template <class T, class U>
	concept ParentsChildType = std::is_base_of_v<T*, U*>() || std::is_base_of_v<U*, T*>();
	template<class T, class U>
	concept Convertible = std::is_convertible_v<T*, U*>();


	template <typename Base, typename Derived>
	concept BASE_CHECK = std::is_base_of_v<Base, Derived>;

	/*
	@ Date: 2023-12-26, Writer: ������
	@ Explain: Args...���� ��ģ ������� �������� �������� �´��� Ȯ��
	*/
	template<typename T, class ...Args>
	concept ConstructArgsCheck = std::is_constructible_v<T, Args...>;

	/*
	@ Date: 2023-12-26, Writer: ������
	@ Explain: Ŭ������ NativeConstruct �Լ��� ���ǵǾ��ִ��� Ȯ��
	*/
	template <typename T, typename... Args>
	concept CheckToSameMethodArgs = requires(T * t, Args&&... args) {
		{ t->NativeConstruct(args...) } -> std::same_as<bool>;
	};


	/*
	@ Date: 2023-12-26
	@ Writer: ������
	@ Explain: SmartPointer���� Ȯ���ϴ� Concept 
	*/
	template <typename T>
	concept SmartPointer = requires(T t) {
		{ t.get() } -> std::same_as<typename T::element_type*>;
		{ t.reset() } -> std::same_as<void>;
	};

	/*
	@ Date: 2023-12-26
	@ Writer: ������
	@ Explain: Pointer�� �迭���� Ȯ���ϴ� conecept
	*/
	template <typename T>
	concept PointerArray = requires(T t) {
		{ *t } -> std::same_as<std::remove_extent_t<T>>;  
	};

	template <typename T, typename U>
	concept ConvertibleFrom = (SameType<T, U> || ParentsChildType<T, U> || Convertible<T, U>);
}


#endif // _SERVERFRAMEWORK_CORE_PUBLIC_CORECONCEPT_H