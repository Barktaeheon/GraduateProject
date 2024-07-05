#ifndef _SERVERFRAMEWORK_CORE_PUBLIC_ATHREADMANAGER_H
#define _SERVERFRAMEWORK_CORE_PUBLIC_ATHREADMANAGER_H

#include "ACoreBase.h"


BEGIN(Core)
class AJobTimer;
class AMainLoop;
class ACoreInstance;

using THREAD = std::thread;
using THRAEDVECTOR = VECTOR<THREAD>;
using JOBTIMERCONTAINER = CONUNORMAP<_int, SHPTR<AJobTimer>>;

/*
@ Date: 2023-12-26, Writer: ������
@ Explain
- Thread�� �����ϴ� �Ŵ��� Ŭ���� 
*/
class  AThreadManager final : public ACoreBase {
public:
	AThreadManager();
	NO_COPY(AThreadManager)
	DESTRUCTOR(AThreadManager)

public:
	void CreateMainLoop(SHPTR<ACoreInstance> _spCoreinstance, Asio::io_service& _service);
	void RegisterFunc(const THREADFUNC& _CallBack, void* _Data);
	void RegisterJob(_int _jobType, CSHPTRREF<AJobTimer> _spJobTimer);
	SHPTR<AJobTimer> FindJobTimer(_int _JobTimer);
	void Join();
	void Detach();
public: 
	SHPTR<AMainLoop> GetMainLoop() const { return m_spMainLoop; }
private:
	static void ThreadJoin(const THREADFUNC& _CallBack, void* _Data, _uint _CurrentThreadID);
	static void InitTLS(_uint _CurrentThreadID);
	static void DestroyTLS();

	// �ʱ�ȭ �Լ� -> UCoreBase�� ��ϵ� 
	virtual void Free() override;
private:
	MUTEX								m_Mutex;
	THRAEDVECTOR				m_ThreadContainer;
	JOBTIMERCONTAINER		m_JobThreadContainer;

	SHPTR< AMainLoop>		m_spMainLoop;
	// Thread Num
	std::atomic< _uint>			m_CurThreadNum;
};

END

#endif // _SERVERFRAMEWORK_CORE_PUBLIC_ATHREADMANAGER_H