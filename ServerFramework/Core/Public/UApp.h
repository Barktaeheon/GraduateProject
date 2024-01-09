#ifndef _SERVERFRAMEWORK_CORE_PUBLIC_UAPP_H
#define _SERVERFRAMEWORK_CORE_PUBLIC_UAPP_H

BEGIN(Core)
class UCoreInstance;
class UService;

/*
@ Date: 2024-01-09
@ Writer: ������
@ Explain
- ���α׷��� �����ϱ� ���� App Ŭ�����̴�. 
*/
class CACHE_ALGIN_CORE_DLL UApp  {
public:
	UApp();
	virtual ~UApp();
public:
	virtual _bool Start() PURE;
protected:
	template<class T>
	 requires ParentsChildType<UService, T>
	SHPTR<T> CreateServiceObject() {
		return CreateInitConstructor<T>(m_spCoreInstance);
	}
private:
	SHPTR<UCoreInstance> m_spCoreInstance;
	SHPTR<UService>			 m_spService;
};

END

#endif // _SERVERFRAMEWORK_CORE_PUBLIC_UAPP_H 
