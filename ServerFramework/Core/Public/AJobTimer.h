#ifndef _SERVERFRAMEWORK_CORE_PUBLIC_AJOBTIMER_H
#define _SERVERFRAMEWORK_CORE_PUBLIC_AJOBTIMER_H

#include "ACoreObject.h"

BEGIN(Core)

/*
@ Date: 2024-01-23, Writer: ������
@ Explain
- ���� �����ϴ� Ŭ������ �ش� Ŭ������ ���� ������ AI, �ǽð����� DB�� ������ �ʿ��� ������ 
������ �� �ִ�. 
*/
class AJobTimer abstract : public ACoreObject {
public:
	AJobTimer(OBJCON_CONSTRUCTOR, Asio::io_service& _service, _int _millisecondsTimer);
	DESTRUCTOR(AJobTimer)
public:
	void RegisterAsio();
	 void TimerThread(const boost::system::error_code& _error);
protected:
	virtual void TickTimer() PURE;
private:
	virtual void Free() override;
private:
	Asio::deadline_timer	m_DeadLineTimer;
};

END

#endif // _SERVERFRAMEWORK_CORE_PUBLIC_AJOBTIMER_H