#ifndef _SERVERFRAMEWORK_CORE_PUBLIC_COREMACRO_H
#define _SERVERFRAMEWORK_CORE_PUBLIC_COREMACRO_H

/*
==========================
					SHOW
==========================
*/

/*
@ Date: 2023-12-28
@ Writer: ������
@ Explain:  �Լ��� �����ų ��, ���۷����� ���� �ش��ϴ� ������ ���� �ٲ� �� ���� ��츦 ��Ÿ��
*/
#define REF_OUT

/*
@ Date: 2023-12-28
@ Writer: ������
@ Explain:  �Լ��� ������ ��, ���۷����� ���� �ش��ϴ� ������ ���� �ٲ� �� ���� ��츦 ��Ÿ��
*/
#define REF_INNER 


/*
@ Date: 2023-12-28
@ Writer: ������
@ Explain: �Լ��� �����ų ��,  Pointer�� ���� �ش��ϴ� ������ ���� �ٲ� �� ������ ��Ÿ��
*/
#define POINTER_OUT 


/*
@ Date: 2023-12-28
@ Writer: ������
@ Explain: �Լ��� ������ ��,  Pointer�� ���� �ش��ϴ� ������ ���� �ٲ� �� ������ ��Ÿ��
*/
#define POINTER_INNTER 

/*
@ Date: 2023-12-28
@ Writer: ������
@ Explain: �б� ���� ������ ��Ÿ����.
*/
#define READONLY

/*
==========================
					LOCK	
==========================
@ Date: 2023-12-28
@ Writer: ������
@ Explain: ��ũ�η� Lock���� �ٷ궧 ����Ѵ�.
*/


#ifdef USE_DEBUG
#define		READ_LOCK_IDX(idx)				Core::ReadLockGuard readLockGuard_##idx(_locks[idx], typeid(this).name());
#define		WRITE_LOCK_IDX(idx)				Core::WriteLockGuard writeLockGuard_##idx(_locks[idx], typeid(this).name());
#else
#define		READ_LOCK_IDX(idx)				Core::ReadLockGuard readLockGuard_##idx(_locks[idx]);
#define		WRITE_LOCK_IDX(idx)				Core::WriteLockGuard writeLockGuard_##idx(_locks[idx]);
#endif

#define		USE_MANY_LOCKS(count)		ARRAY<Core::RWLock, count>;
#define		USE_LOCK									USE_MANY_LOCKS(1)
#define		READ_LOCK								READ_LOCK_IDX(0)
#define		WRITE_LOCK								READ_LOCK_IDX(0)

/*
==========================
					DEBUG
==========================
*/

/*
@ Date: 2023-12-26
@ Writer: ������
@ Explain: ������ ũ������ ����Ű�� ��ũ��
*/
#define CRASH(Value) Core::DEBUG::Crash(Value);
#define ASSERT_CRASH(COND, Value) Core::DEBUG::AssertCrash(Value, COND);

/*
@ Date: 2023-12-26
@ Writer: ������
@ Explain: DEBUG_MACRO�� ���� FILE_LINE_FUNCTION���� ���� 
*/
#define DEBUG_MACRO_TO_METHOD \
__FILE__, __LINE__, __FUNCTION__ 
#define LOG_MACRO_TO_METHOD(x) \
typeid(x).name() , __LINE__, __FUNCTION__

/*
@ Date: 2023-12-26
@ Writer: ������
@ Explain: NULL�̸� Message �ڽ� ���
*/
#ifdef USE_DEBUG

#define ISNULLPTR(T) \
Is_Nullptr_Debug(T, DEBUG_MACRO_TO_METHOD)

#endif 



/*
==========================
					CLASS
==========================
*/


/*
@ Date: 2023-12-26
@ Writer: ������
@ Explain: NO_COPY ��ũ�ΰ� �����ϸ� ���� �Ұ��� Ŭ������� ���̴�. 
*/
#define NO_COPY(ClassName)							\
public: \
ClassName(const ClassName&) = delete;					\
ClassName& operator=(const ClassName&) = delete; 

/*
@ Date: 2023-12-26
@ Writer: ������
@ Explain: �Ҹ��ڿ� Free�� ȣ���ϰ� ����� ��ũ��
*/
#define DESTRUCTOR(ClassName) virtual ~ClassName() {Free();}

/*
@ Date: 2023-12-26
@ Writer: ������
@ Explain: ������Ͽ� �̱��濡 ����� �Լ��� ������ ����
*/
#define DECLARE_SINGLETON(ClassName)					\
		NO_COPY(ClassName)								\
public :												\
	static  SHPTR<ClassName> GetInstance();					\
	static  void DestoryInstance();		\
private:												\
	static SHPTR<ClassName> m_pInstance;

/*
@ Date: 2023-12-26
@ Writer: ������
@ Explain: ������Ͽ��� ������ �Լ��� ������ CPP���Ͽ��� �ʱ�ȭ �׸��� �����Ͽ� �̱����� �ǵ����Ѵ�. 
*/
#define IMPLEMENT_SINGLETON(ClassName)					\
SHPTR<ClassName> ClassName::m_pInstance = nullptr;			\
SHPTR<ClassName> ClassName::GetInstance()					\
{														\
	if (nullptr == m_pInstance)							\
		m_pInstance = Core::PTH::MakeShared<ClassName>(std::make_shared<ClassName>());		\
	return m_pInstance;									\
}														\
void ClassName::DestoryInstance(){  m_pInstance.reset(); }		

/*
@ Date: 2023-12-26
@ Writer: ������
@ Explain: �̱��� Ŭ������ ���� �޾ƿ��� ��ũ��
*/
#define GET_INSTANCE(CLASSNAME) [](){ \
  SHPTR<CLASSNAME> pInstance = CLASSNAME::GetInstance();	\
    return pInstance; \
}();


/*
@ Date: 2023-12-26
@ Writer: ������
@ Explain:  DLL ���� �����ϴ� ��ũ��, EXPORT �϶��� Exprot �ƴ� �� �����ϰ� ���ش�. 
*/
#ifdef CORE_EXPORTS
#define CORE_DLL   _declspec(dllexport)
#define CACHE_ALGIN_CORE_DLL  _declspec(dllexport) __declspec(align(16))
#else
#define CORE_DLL  _declspec(dllimport)
#define CACHE_ALGIN_CORE_DLL  _declspec(dllimport) __declspec(align(16))
#endif



/*
==========================
					ECT
==========================
*/

/*
@ Date: 2023-12-26
@ Writer: ������
@ Explain: namespace�� ���۰� ���� �����ϴ� ���� ����
*/
#define BEGIN(NAME) namespace NAME{
#define END }

/*
@ Date: 2023-12-26
@ Writer: ������
@ Explain: ���ǿ� ������ �����ϴ� ��ũ��
*/
#define RETURN_CHECK(SITUATION, RETURN) if(SITUATION) return RETURN;

#endif // _SERVERFRAMEWORK_CORE_PUBLIC_COREMACRO_H