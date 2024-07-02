#ifndef _SERVERFRAMEWORK_CORE_PUBLIC_AJOBTIMER_H
#define _SERVERFRAMEWORK_CORE_PUBLIC_AJOBTIMER_H

#include "ACoreObject.h"

BEGIN(Core)

using TIMEREVENTQUEUE = CONPRIORITYQUEUE<TIMEREVENT>;

/*
@ Date: 2024-01-23, Writer: ������
@ Explain
- ���� �����ϴ� Ŭ������ �ش� Ŭ������ ���� ������ AI, �ǽð����� DB�� ������ �ʿ��� ������ 
������ �� �ִ�. 
*/
class AJobTimer abstract : public ACoreObject {
public:
	AJobTimer(OBJCON_CONSTRUCTOR, Asio::io_service& _service);
	DESTRUCTOR(AJobTimer)
public:
	void RegisterTimer(_int _RegisterTimer);
	 void TimerThread(const boost::system::error_code& _error);
protected:
	virtual void TickTimer(const TIMEREVENT& _TimerEvent) PURE;
private:
	virtual void Free() override;
private:
	Asio::steady_timer 			m_SteadyEvent;
	TIMEREVENTQUEUE		m_TimerEventQueue;
};

END

#endif // _SERVERFRAMEWORK_CORE_PUBLIC_AJOBTIMER_H