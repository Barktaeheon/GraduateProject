#ifndef _SERVERFRAMEWORK_CORE_PUBLIC_CORECONCEPT_H
#define _SERVERFRAMEWORK_CORE_PUBLIC_CORECONCEPT_H

#include <iostream>
#include <concepts>

namespace Core
{
	/*
@ Date: 2023-12-26
@ Writer: ������
@ Explain: ���� ������, �θ� Ÿ������, operator = ���� ���� �������� 
*/
	template <class T, class U>
	concept SameType = std::is_same_v<T, U>();
	template <class T, class U>
	concept ParentsChildType = std::is_base_of_v<T*, U*>() || std::is_base_of_v<U*, T*>();
	template<class T, class U>
	concept Convertible = std::is_convertible_v<T*, U*>();

	/*
@ Date: 2023-12-26
@ Writer: ������
@ Explain: shared_ptr ����, �ش� ������ �̵��� �� �ִ��� üũ�Ѵ�. 
*/
	template <typename T>
	concept SharedPtrConvertible = requires(T t) {
		{ std::shared_ptr<T>(std::move(t)) };
	};

	/*
	@ Date: 2023-12-26
	@ Writer: ������
	@ Explain: Args...���� ��ģ ������� �������� �������� �´��� Ȯ��
	*/
	template<typename T, class ...Args>
	concept ConstructWidthArgsCheck = std::is_constructible_v<T, Args...>;

	/*
	@ Date: 2023-12-26
	@ Writer: ������
	@ Explain: �����ڰ� �ִ��� Ȯ��
	*/
	template <typename T, typename DevicePtr, typename GpuCommandPtr>
	concept ConstructArgsMatch = std::is_constructible_v<T, DevicePtr, GpuCommandPtr>;

	/*
	@ Date: 2023-12-26
	@ Writer: ������
	@ Explain: Ŭ������ NativeConstruct �Լ��� ���ǵǾ��ִ��� Ȯ��
	*/
	template <typename T, typename... Args>
	concept CheckToSameMethodArgs = requires(T * t, Args&&... args) {
		{ t->NativeConstruct(args...) } -> std::same_as<bool>;
	};

	template <typename T, typename U>
	concept ConvertibleFrom = (SameType<T, U> || ParentsChildType<T, U> || Convertible<T, U>);
	template <typename T, typename U>
	concept SharedPtrConvertibleFrom = SharedPtrConvertible<U> && (SameType<T, U> || ParentsChildType<T, U>);
}


#endif // _SERVERFRAMEWORK_CORE_PUBLIC_CORECONCEPT_H