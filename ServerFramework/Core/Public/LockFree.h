#ifndef _SERVERFRAMEWORK_CORE_PUBLIC_LOCKFREE_H
#define _SERVERFRAMEWORK_CORE_PUBLIC_LOCKFREE_H

namespace Core
{
	/*
	@ Date: 2023-12-29
	@ Writer: ������
	@ Explain: ��Ƽ������ ȯ�濡�� �ϳ��� �����Ϳ� CAS�� ������ ���� �����ϱ� ���� �����带 ��� ����Ű�� Ŭ����
	*/
	class CACHE_ALGIN Backoff {
	public:
		Backoff(int _minDelay, int _maxDelay) : m_UniformBackOff{ _minDelay, _maxDelay }, m_Limit{ 0 }, m_MaxDelay{ _maxDelay } {}

		void Relax() {
			int delay = 0;
			m_Limit = m_UniformBackOff(m_Dre);
			m_Limit *= 2;
			if (m_Limit > m_MaxDelay)
				m_Limit = m_MaxDelay;
			std::this_thread::sleep_for(std::chrono::microseconds(delay));
		}
	private:
		std::default_random_engine					m_Dre;
		std::uniform_int_distribution<int>		m_UniformBackOff;
		int																m_MaxDelay;
		int																m_Limit;
	};

	/*
	@ Date: 2023-12-31
	@ Writer: ������
	@ Explain: EBR Controller�� ������
	*/
	template<class NODE>
	class EBRController {
	public:
		EBRController() : m_Epoch{ 0 }, m_RemainCount{ 0 }, m_Retired{},
			m_Reservation{}, m_CurrentNumThreads{ } {}
		~EBRController()
		{
			clear_ebr();
		}

		void Initialize(int _currThread, int _remainCount)
		{
			m_CurrentNumThreads = _currThread;
			m_RemainCount = _remainCount;

			for (int i = 0; i < m_CurrentNumThreads; ++i)
			{
				m_Reservation[i] = 0;
			}
		}

		// ������� �ð��� �� ���� ���� ȣ��� �ð��� �����´�. 
		unsigned int get_min_reservation() {
			unsigned int min_re = 0xffffffff;
			for (int i = 0; i < m_CurrentNumThreads; ++i) {
				min_re = std::min(min_re, m_Reservation[i].load(std::memory_order_relaxed));
			}
			return min_re;
		}

		// ȣ��� �ð��� ���� ���� ���� �������� �޸� ����
		void empty() {
			unsigned int max_safe_epoch = get_min_reservation();

			while (false == m_Retired[TLS::g_ThreadID].empty()) {
				auto f = m_Retired[TLS::g_ThreadID].front();
				if (f->retireEpoch >= max_safe_epoch)
					break;
				m_Retired[TLS::g_ThreadID].pop();
				Core::MemoryRelease(f);
			}
		}

		// RemainCount���� ũ�� Empty ȣ�� 
		void retire(NODE* ptr) {
			m_Retired[TLS::g_ThreadID].push(ptr);
			ptr->retireEpoch = m_Epoch.load(std::memory_order_relaxed);
			if (m_Retired[TLS::g_ThreadID].size() >= m_RemainCount) {
				empty();
			}
		}

		// Reservation�� ����
		void start_op() {
			m_Reservation[TLS::g_ThreadID].store(m_Epoch.fetch_add(1, std::memory_order_relaxed), std::memory_order_relaxed);
		}

		// Reservation�� �����Ѵ�. 
		void end_op() {
			m_Reservation[TLS::g_ThreadID].store(ENDOP_VALUE, std::memory_order_relaxed);
		}

	private:
		// ���α׷� ����� ��� �޸𸮸� �����Ѵ�. 
		void clear_ebr()
		{
			for (int i = 0; i < m_CurrentNumThreads; ++i)
			{
				while (!m_Retired[i].empty())
				{
					NODE* node = m_Retired[i].front();
					Core::MemoryRelease(node);
					m_Retired[i].pop();
				}
			}
			m_Epoch = 0;
		}

	private:
		static constexpr	unsigned int												ENDOP_VALUE{ 0xffffffff };
		std::atomic_uint																		m_Epoch;
		unsigned int																				m_RemainCount;
		std::array<std::queue<NODE*>, TLS::MAX_THREAD>	m_Retired;
		std::array<std::atomic_uint, TLS::MAX_THREAD>			m_Reservation;
		int																								m_CurrentNumThreads;
	};

	namespace PTHStack {

		/*
		@ Date: 2023-12-29
		@ Writer: ������
		@ Explain: Stack�� ���
		*/
		template<class T>
		struct CACHE_ALGIN NODE {

			NODE<T>() : next{ nullptr }, retireEpoch{ 0 }, isRemove{ false }
			{
				if constexpr ((true == std::is_pointer<T>()) || (true == SmartPointer<T>))
					data = nullptr;
				else
					data = T();
			}
			NODE<T>(T _data, bool _isRemove = false) : next{ nullptr }, retireEpoch{ 0 }, data{ _data }, isRemove{ _isRemove }
			{}

			~NODE<T>()
			{
				if (true == isRemove)
				{
					if constexpr ((std::is_pointer<T>()))
					{
						Core::MemoryAlloc(data);
					}
				}
			}
		public:
			NODE<T>* volatile	next;
			T									data;
			std::atomic_uint		retireEpoch;
			bool								isRemove;
		};
		/*
		@ Date: 2023-12-29
		@ Writer: ������
		@ Explain: LockFreeStack + Backoff
		*/
		template<class T>
		class CACHE_ALGIN LockFreeStack {
		public:
			LockFreeStack() : m_BackOff{ 1, 50 }, m_Top{ nullptr } { }
			~LockFreeStack() {
				clear();
			}

			void Initialize(int _currentThread, int _remainCount)
			{
				m_EBRController.Initialize(_currentThread, _remainCount);
			}

			// �� ���� �ִ� ���� �����´�. 
			T Top()
			{
				NODE<T>* pNode = m_Top;
				if (nullptr == pNode)
				{
					return T();
				}
				return pNode->data;
			}

			// Top�� ���� �о� �ִ´�. 
			void Push(T _data, bool _isRemove = false)
			{
				m_EBRController.start_op();
				NODE<T>* newNode = Core::MemoryAlloc<NODE<T>>( _data, _isRemove );
				while (true) {
					NODE<T>* p = m_Top;
					newNode->next = p;
					// ī�� ����
					if (true == CAS(p, newNode)) {
						m_EBRController.end_op();
						return;
					}
					// ���н� ��� ���
					m_BackOff.Relax();
				}
			}
			// Top���κ��� ���� ������. 
			T Pop() {
				m_EBRController.start_op();
				while (true)
				{
					NODE<T>* ptr = m_Top;
					if (ptr == nullptr)
					{
						m_EBRController.end_op();
						return T();
					}
					NODE<T>* next = ptr->next;
					T data = ptr->data;

					if (false == CAS(ptr, next))
					{
						m_BackOff.Relax();
						continue;
					}
					m_EBRController.retire(ptr);
					m_EBRController.end_op();
					return data;
				}
			}

			bool IsEmpty()
			{
				NODE<T>* pNode = m_Top;
				return pNode == nullptr;
			}

			void clear()
			{
				NODE<T>* p = m_Top;
				while (p != nullptr) {
					NODE<T>* t = p;
					p = p->next;
					Core::MemoryRelease<NODE<T>>(t);
				}
				m_Top = nullptr;
			}
		private:
			bool CAS(NODE<T>* _old, NODE<T>* _new)
			{
				return std::atomic_compare_exchange_strong(reinterpret_cast<volatile std::atomic_llong*>(&m_Top),
					reinterpret_cast<long long*>(&_old),
					reinterpret_cast<long long>(_new));
			}
		private:
			EBRController<NODE<T>>		m_EBRController;
			Core::Backoff							m_BackOff;
			NODE<T>* volatile					m_Top;
		};
	}

	template<class T>
	using CONSTACK = Core::PTHStack::LockFreeStack<T>;
}


#endif // _SERVERFRAMEWORK_CORE_PUBLIC_LOCKFREE_H