
#ifndef _PTH_FRAMEWORK_ENGIEN_PUBLIC_UEVENT_H
#define _PTH_FRAMEWORK_ENGIEN_PUBLIC_UEVENT_H

BEGIN(Engine)

/*

Event ��ü�� ������ Ŭ���� 

�� Ŭ������ Ȱ���ϸ� �ش� �̺�Ʈ�� �߻��� �� ���� ����� �� �ֵ��� ���� �����̴�. 
�̺�Ʈ�� ������ ���Ѵ�. 

*/

// �̺�Ʈ�� ������ enum class 
enum EVENT : _ushort 
{
	EVENT_NON = 0,
	EVENT_ACTIVE = 1,

};

class ENGINE_DLL UEvent {
public:
	UEvent();
	UEvent(const _ushort  _sEvent);
	UEvent(const UEvent& _rhs);
	~UEvent();

public:
	static void AllEventClear();

	// �ش� �̺�Ʈ�� ��ٴ�. 
	void WaitEvent();
	void CountDown();
private:
	static VECTOR<UEvent*>				s_m_vecEventContainer;

	//  ���� �б� 
	 std::condition_variable				m_ConditionVariable;
	atomic_short								m_sEventCountSaves;
	atomic_short								m_sEventCountIndex;
	// �̺�Ʈ ���
	std::mutex										m_EventMutex;
};


END

#endif //_PTH_FRAMEWORK_ENGIEN_PUBLIC_UEVENT_H