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
class CORE_DLL UApp  {
public:
	UApp();
	virtual ~UApp();
public:
	virtual _bool Start() PURE;
protected:
	template<class T, class... Args>
	void RegisterServiceObject(Args&&... _args) {
		m_spCoreInstance->ReadyCoreInstance(CreateInitConstructor<T>(m_spCoreInstance, std::forward<Args>(_args)...));
	}
protected: /* get set*/
	CSHPTRREF<UCoreInstance>	GetCoreInstance() { return m_spCoreInstance; }
private:
	SHPTR<UCoreInstance> m_spCoreInstance;
};

END

#endif // _SERVERFRAMEWORK_CORE_PUBLIC_UAPP_H 
