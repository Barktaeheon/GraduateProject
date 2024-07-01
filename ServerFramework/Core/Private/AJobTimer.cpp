#include "CoreDefines.h"
#include "AJobTimer.h"
#include <functional>

namespace Core {
	AJobTimer::AJobTimer(OBJCON_CONSTRUCTOR, Asio::io_service& _service, _int _millisecondsTimer) :
		ACoreObject(OBJCON_CONDATA),
		m_DeadLineTimer{_service, boost::posix_time::milliseconds(_millisecondsTimer)}
	{
	}

	void AJobTimer::RegisterAsio()
	{
		// Ÿ�̸��� �񵿱� ��� ����
		m_DeadLineTimer.async_wait(std::bind(&AJobTimer::TimerThread, this, std::placeholders::_1));
	}

	void AJobTimer::TimerThread(const boost::system::error_code& _error)
	{
		TickTimer();
	}

	void AJobTimer::TickTimer()
	{
	}

	void AJobTimer::Free()
	{
	}
}