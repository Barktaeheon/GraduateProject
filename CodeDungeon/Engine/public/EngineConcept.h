#pragma once


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

	template<class T>
	concept CheckNumber = std::is_same_v<T, unsigned int> || std::is_same_v<T, float> || std::is_same_v<T, short> || std::is_same_v<T, long> ||
		std::is_same_v<T, unsigned short> || std::is_same_v<T, int> || std::is_same_v<T, unsigned long> || std::is_same_v<T, unsigned long long>
		|| std::is_same_v<T, long long> || std::is_same_v<T, unsigned char> || std::is_same_v<T, signed char>;

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

