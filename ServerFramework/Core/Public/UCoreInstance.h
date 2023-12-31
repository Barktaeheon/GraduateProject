#ifndef _SERVERFRAMEWORK_CORE_PUBLIC_UCOREINSTANCE_H
#define _SERVERFRAMEWORK_CORE_PUBLIC_UCOREINSTANCE_H

BEGIN(Core)
class UThreadManager;
/*
@ Date: 2023-12-26
@ Writer: ������
@ Explain
- ����, �Ҹ��ڸ� �̿��ؼ� ������ ����� ��, �޸𸮰� ����� �����ǰ� �ϱ� ���� �۷ι� Ŭ�����̴�. 
*/
class CACHE_ALGIN_CORE_DLL UCoreInstance {
public:
	UCoreInstance();
	NO_COPY(UCoreInstance)
	~UCoreInstance();
public: /* CoreGrobal */
	HRESULT ReadyCoreGrobal();
public: /* ThreadManager */
	void RegisterFunc(const THREADFUNC& _CallBack, void* _Data);
	void Join();

private:
	MUTEX											m_Mutex;
	IOContext*										m_Context;
	SHPTR<UThreadManager>		m_ThreadManager;
};

END


#endif 

