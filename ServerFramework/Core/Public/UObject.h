#ifndef _SERVERFRAMEWORK_CORE_PUBLIC_UOBJECT_H
#define _SERVERFRAMEWORK_CORE_PUBLIC_UOBJECT_H

#include "UBase.h"

BEGIN(Core)
class UCoreInstance;

/*
@ Date: 2023-12-26
@ Writer: ������
@ Explain
- UCoreInstance ��ü�� �ʿ��� ��� Ŭ����
*/
class CORE_DLL  UObject abstract : public UBase {
public:
	UObject(SHPTR<UCoreInstance> _spCoreInstance);
	UObject(const UObject& _rhs);
	DESTRUCTOR(UObject)

protected:
	SHPTR<UCoreInstance> GetCoreInstance() const { return m_wpCoreInstance.lock(); }

private:
	virtual void Free() override;

private:
	WKPTR<UCoreInstance>		m_wpCoreInstance;
};

END

#endif // _SERVERFRAMEWORK_CORE_PUBLIC_UOBJECT_H