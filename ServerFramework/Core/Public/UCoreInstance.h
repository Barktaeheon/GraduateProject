#ifndef _SERVERFRAMEWORK_CORE_PUBLIC_UCOREINSTANCE_H
#define _SERVERFRAMEWORK_CORE_PUBLIC_UCOREINSTANCE_H

#include "UBase.h"

BEGIN(Core)
class UThreadManager;
/*
@ Date: 2023-12-26
@ Writer: ������
@ Explain
- ����, �Ҹ��ڸ� �̿��ؼ� ������ ����� ��, �޸𸮰� ����� �����ǰ� �ϱ� ���� Ŭ�����̴�. 
������ �� ���� �����Ѵ�. 
*/
class CACHE_ALGIN_CORE_DLL UCoreInstance : public UBase {
	DECLARE_ONCEDATA
public:
	UCoreInstance();
	NO_COPY(UCoreInstance)
	DESTRUCTOR(UCoreInstance)

public: /* CoreGrobal */
	bool NativeConstruct();
public: /* ThreadManager */
	void RegisterFunc(const THREADFUNC& _CallBack, void* _Data);
	void Join();

public:

private:
	virtual void Free() override;
private:
	MUTEX											m_Mutex;
	SHPTR<UThreadManager>		m_ThreadManager;
};

END


#endif 

