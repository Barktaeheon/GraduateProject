#ifndef _SERVERFRAMEWORK_CORE_PUBLIC_UCOREINSTANCE_H
#define _SERVERFRAMEWORK_CORE_PUBLIC_UCOREINSTANCE_H

#include "UBase.h"

BEGIN(Core)
class UThreadManager;
class URandomManager;
class ULogManager;
/*
@ Date: 2023-12-26
@ Writer: ������
@ Explain
- ����, �Ҹ��ڸ� �̿��ؼ� ������ ����� ��, �޸𸮰� ����� �����ǰ� �ϱ� ���� Ŭ�����̴�. 
������ �� ���� �����Ѵ�. 
*/
class CACHE_ALGIN_CORE_DLL UCoreInstance : public UBase {
public:
	UCoreInstance();
	NO_COPY(UCoreInstance)
	DESTRUCTOR(UCoreInstance)
public: /* ThreadManager */
	void RegisterFunc(const THREADFUNC& _CallBack, void* _Data);
	void Join();
public: /* RandomManager */
	_int		ReturnRadomNumber(const _int _iMinNum, const _int _iMaxNum);
	_int		ReturnRadomNumber(const _int _iMaxNum);

public: /* LogManager */
	void PrintOut(const char* _fmt, ...);
	void FileOut(const char* _fmt, ...);

private:
	virtual void Free() override;
private:
	MUTEX											m_Mutex;
	SHPTR<UThreadManager>		m_spThreadManager;
	SHPTR<URandomManager>		m_spRandomManager;
	SHPTR<ULogManager>				m_spLogManager;
};

END


#endif 

