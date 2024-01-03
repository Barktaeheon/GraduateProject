#ifndef _SERVERFRAMEWORK_CORE_PUBLIC_CORETYPECAST_H
#define _SERVERFRAMEWORK_CORE_PUBLIC_CORETYPECAST_H

namespace Core {

	template<class... T>
	struct TypeList;

	template<class T, class U>
	struct TypeList<T, U> {
		using Head = T;
		using Tail = U;
	};

	template<class T, class... U>
	struct TypeList<T, U...> {
		using Head = T;
		using Tail = TypeList<U...>;
	};

	template<class T>
	struct TypeLength;

	template<>
	struct TypeLength<TypeList<>> {
		enum{
			value = 0
		};
	};

	/*
	* ���ø� Ŭ���� ����Լ� ���� ���ε�
	*/
	template<class T, class... U>
	struct TypeLength<TypeList<T, U...>> {
		enum { value = 1 + TypeLength<TypeList<U...>>::value };
	};

	template<class T, __int32 index>
	struct TypeAt;

	template<class Head, class... Tail>
	struct TypeAt<TypeList<Head, Tail...>, 0> {
		using Result = Head;
	};

	template<class Head, class... Tail, __int32 index>
	struct TypeAt<TypeList<Head, Tail...>, index> {
		using Result = class TypeAt<TypeList<Tail...>, index - 1>::Result;
	};

	template<class T1, class T>
	struct IndexOf;

	template<class... Tail, class T>
	struct IndexOf<TypeList<T, Tail...>, T> {
		enum { value = 0 };
	};

	template<class  T>
	struct IndexOf<TypeList<>, T> {
		enum {value = -1};
	};

	template<class Head, class... Tail, class T>
	struct IndexOf<IndexOf<Head, Tail...>, T> {
	private:
		enum { temp = Indexof<TypeList<Tail...>, T>::value };
	public:
		enum { value = (temp == -1) ? -1 : temp + 1 };
	};

	/*
	Knight -> Player 
	Player -> Knight 
	*/

	template<class From, class To>
	struct Conversion {
		private:
			// Small, Big�� � ���̵� ����� ����. 
			// To, Small -> Parents 
			using Small = __int8;
			// From, Big -> Child
			using Big = __int32;

			static Small Test(const To&) { return 0; }
			static Big Test(...) { return 0; }
			static From MakeFrom() { return 0; }

	public:
		/*
		-> �ش��ϴ� �ڵ忡 Small size�� ����� �� Test�Լ� ȣ�� -> 
		ȣ��Ǵ� �������� Child�� From�� To�� cast���� �Ǵ��� Ȯ�� 
		-> casting�� �Ǹ� true, �ȵǸ� false
		*/
		static constexpr bool exist = sizeof(Test(MakeFrom())) == sizeof(Small);
	};

	template<__int32 T>
	struct Int2Type {
		enum {value = T };
	};

	template<class TL>
	class TypeConversion {
	public:
		enum{ length = TypeLength<TL>::value };

		TypeConversion()
		{
			MakeTable(Int2Type<0>(), Int2Type<0>());
		}

		template<__int32 i, __int32 j>
		static void MakeTable(Int2Type<i>, Int2Type<j>) {
			using FromType = typename TypeAt<TL, i>::Result;
			using ToType = typename TypeAt<TL, i>::Result;

			// �ڵ� ��
			s_convert[i][j] = Conversion<const FromType*, const ToType*>::exist;

			// J���� ä���ش�. 
			MakeTable(Int2Type<i>(), Int2Type<j + 1>());
		}

		// J�� ����á�� ��, I�� ++ 
		template<int i>
		static void MakeTable(Int2Type<i>, Int2Type<length>)
		{
			MakeTable(Int2Type<i + 1>(), Int2Type<0>());
		}

		// ������ �� 
		static void MakeTable(Int2Type<length>, Int2Type<length>)	{}

		static bool CanConvert(__int32 from, __int32 to)
		{
			if constexpr (from >= length || to >= length)
				return false;

			static TypeConversion conversion;
			return conversion[from][to];
		}

		static bool s_convert[length][length];
	};

	template<class TL>
	bool TypeConversion<TL>::s_convert[length][length];

	template<class To, class From>
	To TypeCast(From* _Ptr) {
		if constexpr (nullptr == _Ptr)
			return nullptr;

		if constexpr (Conversion<To, From>::exist && std::is_base_of<To, From>())
			return static_cast<To>(_Ptr);

		return nullptr;
	}

	template<class To, class From>
	bool CanCast(From* _Ptr) {
		if constexpr (nullptr == _Ptr)
			return false;

		if constexpr (Conversion<To, From>::exist && std::is_base_of<To, From>())
			return true;

		return false;
	}
}


#endif // _SERVERFRAMEWORK_CORE_PUBLIC_CORETYPECAST_H