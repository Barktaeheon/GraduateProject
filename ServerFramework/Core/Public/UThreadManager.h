#ifndef _SERVERFRAMEWORK_CORE_PUBLIC_THREADMANAGER_H
#define _SERVERFRAMEWORK_CORE_PUBLIC_THREADMANAGER_H

#include "UBase.h"


BEGIN(Core)
template<class T>
using VECTOR = std::vector<T>;
using THREAD = std::thread;
using THRAEDVECTOR = VECTOR<THREAD>;

/*
@ Date: 2023-12-26
@ Writer: ������
@ Explain:  Thread�� �����ϴ� �Ŵ��� Ŭ���� 
*/
class CACHE_ALGIN_CORE_DLL UThreadManager final : public UBase {
public:
	UThreadManager();
	NO_COPY(UThreadManager)
	DESTRUCTOR(UThreadManager)

public:
	/*
	@ Date: 2023-12-26
	@ Writer: ������
	@ Explain:  ���� �����带 ����ϴ� �Լ�
	*/
	void RegisterFunc(const THREADFUNC& _CallBack, void* _Data);
	/*
	@ Date: 2023-12-26
	@ Writer: ������
	@ Explain:  ���� �����带 �����ϴ� �Լ�
	*/
	void Join();
private:
	/*
	@ Date: 2023-12-26
	@ Writer: ������
	@ Explain:  �����忡 ����ϴ� �Լ�
	*/
	static void ThreadJoin(const THREADFUNC& _CallBack, void* _Data, _uint _iCurrentThreadID);
	/*
	@ Date: 2023-12-26
	@ Writer: ������
	*/
	static void InitTLS(_uint _iCurrentThreadID);
	/*
	@ Date: 2023-12-26
	@ Writer: ������
	*/
	static void DestroyTLS();

	// �ʱ�ȭ �Լ� -> UBase�� ��ϵ� 
	virtual void Free() override;
private:
	MUTEX							m_Mutex;
	THRAEDVECTOR			m_ThreadContainer;
	// Thread Num
	std::atomic< _uint>		m_iCurThreadNum;
};

END

#endif // _SERVERFRAMEWORK_CORE_PUBLIC_THREADMANAGER_H