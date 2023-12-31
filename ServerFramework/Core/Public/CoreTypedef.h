#ifndef _SERVERFRAMEWORK_CORE_PUBLIC_CORETYPEDEF_H
#define _SERVERFRAMEWORK_CORE_PUBLIC_CORETYPEDEF_H
/*
@ Date: 2023-12-26
@ Writer: ������
@ Explain
- ���� �����ӿ�ũ ���࿡�� ����� ���� Ÿ�Ե� ����
*/
namespace Core {

	using _ubyte = unsigned char;
	using _byte = signed char;
	using  _char = char;

	using _ushort = unsigned short;
	using _short = signed short;

	using _uint = unsigned int;
	using _int = signed int;

	using _ulong = unsigned long;
	using _long = signed long;
	using _ullong = unsigned long long;
	using _llong = signed long long;

	using	ACTORID = _llong;
	using CAMID = unsigned short;

	using ID = __int64;

	using _float = float;
	using _double = double;

	using _bool = bool;
	using _tchar = wchar_t;

	/*
	GLM (OpenGl�� ���� ���̺귯��) 
	*/
	using _float2 = PTH::Vector2;
	using _float3 = PTH::Vector3;
	using _float4 = PTH::Vector4;

	using _int2 = glm::int2;
	using _int3 = glm::int3;
	using _int4 = glm::int4;

	using _float4x4 = PTH::Matrix;
	using _quaternion = PTH::Quaternion;

	using _wstring = std::wstring;
	using _string = std::string;

	using _wstringview = std::wstring_view;
	using _stringview = std::string_view;

	template<class T>
	using _atomic = std::atomic<T>;

	using _mutex = std::mutex;
	using SMART_LOCK = std::lock_guard<_mutex>;
	using UNQUE_LOCK = std::unique_lock<_mutex>;

	namespace PATH {
		using PATHS = std::vector<std::wstring>;
	}

	using PATHS = PATH::PATHS;

	namespace VOIDS {
		using VOIDDATAS = std::vector<void*>;
	}

	using VOIDDATAS = VOIDS::VOIDDATAS;

	template<class Type, _uint Num>
	using ARRAY = std::array<Type, Num>;

	template<class Type, class Other>
	using CONUNORMAP = tbb::concurrent_unordered_map<Type, Other>;

	template<class Type, class Other, class Compare = std::less<Type>>
	using CONMULTIMAP = tbb::concurrent_multimap<Type, Other, Compare>;

	template<class Type, class Other>
	using CONMAP = tbb::concurrent_map<Type, Other>;

	template<class Type, class Other, class Compare = tbb::tbb_hash_compare<Type>>
	using CONHASHMAP = tbb::concurrent_hash_map<Type, Other, Compare>;

	template<class Type>
	using CONUNORSET = tbb::concurrent_unordered_set<Type>;

	template<class Type>
	using CONVECTOR = tbb::concurrent_vector<Type>;

	template<class Type>
	using CONQUEUE = tbb::concurrent_queue<Type>;

	template<class Type>
	using CONPRIORITYQUEUE = tbb::concurrent_priority_queue<Type>;

	template<class Type>
	using CONSET = tbb::concurrent_set<Type>;

	template<class Type>
	using QUEUE = std::queue<Type>;

	template<class Type, class Compare = std::less<Type>>
	using CONMULTISET = tbb::concurrent_multiset<Type, Compare>;

	using IOContext = boost::asio::io_context;

	using SendReceiveFunc = std::function<void(const boost::system::error_code& _error, std::size_t _Size)>;
	using AcceptFunc = std::function<void(const boost::system::error_code& _error)>;

	template<class T>
	using ATOMIC = std::atomic<T>;

	template<class T>
	using LOCKGUARD = std::lock_guard<T>;

	// TBB Mutex
	using MUTEX = tbb::mutex;

	using THREADFUNC = std::function<void(void*)>;
}



#endif // _SERVERFRAMEWORK_CORE_PUBLIC_CORETYPEDEF_H