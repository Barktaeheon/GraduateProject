#ifndef _SERVERFRAMEWORK_CORE_PUBLIC_COREMACRO_H
#define _SERVERFRAMEWORK_CORE_PUBLIC_COREMACRO_H


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
@ Date: 2023-12-26
@ Writer: ������
@ Explain: NO_COPY ��ũ�ΰ� �����ϸ� ���� �Ұ��� Ŭ������� ���̴�. 
*/
#define NO_COPY(ClassName)							\
private: \
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
@ Explain: ������ ũ������ ����Ű�� ��ũ��
*/
#define CRASH(Value) Core::DEBUG::Crash(Value);
#define ASSERT_CRASH(COND, Value) Core::DEBUG::AssertCrash(Value, COND);


#endif // _SERVERFRAMEWORK_CORE_PUBLIC_COREMACRO_H