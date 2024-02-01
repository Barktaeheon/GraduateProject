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
	AJobTimer(OBJCON_CONSTRUCTOR);
	DESTRUCTOR(AJobTimer)
public:
	static void TimerThread(void* _pJobTimer);
	 void RunTimer();
	void TurnOffRunningThread() { m_isRunningThread = false; }
protected:
	virtual void TickTimer() PURE;
private:
	virtual void Free() override;
private:
	_bool	m_isRunningThread;
};

END

#endif // _SERVERFRAMEWORK_CORE_PUBLIC_AJOBTIMER_H