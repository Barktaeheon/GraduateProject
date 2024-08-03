#include "CoreDefines.h"
#include "AJobTimer.h"
#include <functional>

namespace Core {
	AJobTimer::AJobTimer(OBJCON_CONSTRUCTOR, Asio::io_context& _context) :
		ACoreObject(OBJCON_CONDATA),
		m_SteadyEvent{ _context }
	{
	}

	void AJobTimer::Free()
	{
	}

	void AJobTimer::RegisterTimer(_int _RegisterTimer)
	{
		m_SteadyEvent.expires_from_now(std::chrono::microseconds(_RegisterTimer));
		// Ÿ�̸��� �񵿱� ��� ����
		m_SteadyEvent.async_wait(std::bind(&AJobTimer::TimerThread, this, std::placeholders::_1));
	}

	void AJobTimer::TimerThread(const boost::system::error_code& _error)
	{
		std::atomic_thread_fence(std::memory_order_seq_cst);

		TIMEREVENT TimerEvent;
		auto CurrentTime = std::chrono::system_clock::now();

		if (true == m_TimerEventQueue.try_pop(TimerEvent))
		{
			if (TimerEvent.WakeUpTime > CurrentTime) {
				auto MTime = TimerEvent.WakeUpTime - CurrentTime;
				std::this_thread::sleep_for(MTime);
			}
			TickTimer(TimerEvent);
		}
		else
		{
			RegisterTimer(10);
			return;
		}

		RegisterTimer(1);
	}

	void AJobTimer::InsertTimerEvent(TIMEREVENT _TimerEvent)
	{
		m_TimerEventQueue.push(_TimerEvent);
	}
}