#ifndef _SERVERFRAMEWORK_CORE_PUBLIC_COREMACRO_H
#define _SERVERFRAMEWORK_CORE_PUBLIC_COREMACRO_H


/*
==========================
					SHOW
==========================
*/
/*
@ Date: 2023-12-28, Writer: ������
@ Explain
- �Լ��� �����ų ��, ���۷����� ���� �ش��ϴ� ������ ���� �ٲ� �� ���� ��츦 ��Ÿ��
*/
#define REF_OUT


/*
@ Date: 2023-12-28,  Writer: ������
@ Explain
- �Լ� ������ ��, ���۷����� ���� �ش��ϴ� ������ ���� �ٲ� �� ���� ��츦 ��Ÿ��
*/
#define REF_IN


/*
@ Date: 2023-12-28, Writer: ������
@ Explain
- ���۷��� �Լ��� �����ϴ� ��츦 ��Ÿ����. 
*/
#define REF_RETURN

/*
@ Date: 2023-12-28
@ Writer: ������
@ Explain
- �Լ��� �����ų ��,  Pointer�� ���� �ش��ϴ� ������ ���� �ٲ� �� ������ ��Ÿ��
*/
#define POINTER_OUT 

/*
@ Date: 2023-12-28
@ Writer: ������
@ Explain
- �б� ���� ������ ��Ÿ����.
*/
#define READONLY

/*
@ Date: 2023-12-30
@ Writer: ������
@ Explain
- �����Լ���� ���� ��Ÿ���� ��ũ��
*/
#define PURE = 0

/*
==========================
					LOCK	
==========================
@ Date: 2023-12-28
@ Writer: ������
@ Explain
- ��ũ�η� Lock���� �ٷ궧 ����Ѵ�.
*/


#ifdef USE_DEBUG
#define		READ_LOCK_IDX(idx)				Core::ReadLockGuard readLockGuard_##idx(_locks[idx], typeid(this).name());
#define		WRITE_LOCK_IDX(idx)				Core::WriteLockGuard writeLockGuard_##idx(_locks[idx], typeid(this).name());
#else
#define		READ_LOCK_IDX(idx)				Core::ReadLockGuard readLockGuard_##idx(_locks[idx]);
#define		WRITE_LOCK_IDX(idx)				Core::WriteLockGuard writeLockGuard_##idx(_locks[idx]);
#endif

#define		USE_MANY_LOCKS(count)		Core::ARRAY<Core::URWLock, count>
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
@ Explain
- ������ ũ������ ����Ű�� ��ũ��
*/
#define CRASH(cause) 	\
{											\
	int* crash = nullptr;				\
	__analysis_assume(crash != nullptr);	\
	*crash = 0xDEADBEEF;					\
}

#define ASSERT_CRASH(COND) \
{									\
	if (!(COND))					\
	{								\
		CRASH("ASSERT_CRASH");		\
		__analysis_assume(COND);	\
	}								\
}


/*
@ Date: 2023-12-26
@ Writer: ������
@ Explain
- DEBUG_MACRO�� ���� FILE_LINE_FUNCTION���� ���� 
*/
#define DEBUG_MACRO_TO_METHOD \
__FILE__, __LINE__, __FUNCTION__ 
#define LOG_MACRO_TO_METHOD(x) \
typeid(x).name() , __LINE__, __FUNCTION__


/*
==========================
					CLASS
==========================
*/


/*
@ Date: 2023-12-26
@ Writer: ������
@ Explain
- NO_COPY ��ũ�ΰ� �����ϸ� ���� �Ұ��� Ŭ������� ���̴�. 
*/
#define NO_COPY(ClassName)							\
public: \
ClassName(const ClassName&) = delete;					\
ClassName& operator=(const ClassName&) = delete; 

/*
@ Date: 2023-12-26
@ Writer: ������
@ Explain
- �Ҹ��ڿ� Free�� ȣ���ϰ� ����� ��ũ��
*/
#define DESTRUCTOR(ClassName) virtual ~ClassName() {Free();}

/*
@ Date: 2023-12-26
@ Writer: ������
@ Explain
- ������Ͽ� �̱��濡 ����� �Լ��� ������ ����
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
@ Explain
- ������Ͽ��� ������ �Լ��� ������ CPP���Ͽ��� �ʱ�ȭ �׸��� �����Ͽ� �̱����� �ǵ����Ѵ�. 
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
@ Explain
- �̱��� Ŭ������ ���� �޾ƿ��� ��ũ��
*/
#define GET_INSTANCE(CLASSNAME) [](){ \
  SHPTR<CLASSNAME> pInstance = CLASSNAME::GetInstance();	\
    return pInstance; \
}();

/*
==========================
			Type List Define 
==========================
*/

/*
@ Date: 2024-01-01
@ Writer: ������
@ Explain
- TypeList�� �����ϱ� ���� ��ũ�� 
*/
#define DEFCLARE_TL using TL = TL;  int typeId;
#define INIT_TL(Type) typeId = Core::IndexOf<TL, Type>::value;

/*
==========================
			CONSTRUCTOR 
==========================
*/

/*
@ Date: 2024-01-02,  Writer: ������
@ Explain
-  OBJECT �����ڿ� ���ǵǾ� �ִ� ������ ��ũ��ȭ ���״�. 
*/
#define OBJCON_CONSTRUCTOR Core::SHPTR<Core::UCoreInstance> _spCoreInstance
#define OBJCON_CONDATA _spCoreInstance 

/*
@ Date: 2024-01-02,  Writer: ������
@ Explain
-  Session �����ڿ� ���ǵǾ� �ִ� ������ ��ũ��ȭ ���״�.
*/
#define SESSION_CONSTRUCTOR OBJCON_CONSTRUCTOR,  Asio::ip::tcp::socket _TcpSocket, \
Core::SHPTR<Core::UService> _spService,  SESSIONID _ID
#define SESSION_CONDATA(SessionType) OBJCON_CONDATA, std::move(_TcpSocket), _spService, _ID, SessionType
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