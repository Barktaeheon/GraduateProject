#ifndef _SERVERFRAMEWORK_CORE_PUBLIC_USPACE_H
#define _SERVERFRAMEWORK_CORE_PUBLIC_USPACE_H

#include "UBase.h"

BEGIN(Core)
class UTransform;
class UCollider;
class USession;
/*
@ Date: 2023-01-19, Writer: ������
@ Explain
- ���ӻ��� ������ �����Ͽ� �þ� Ȯ���� �ϱ� ���� Ŭ�����̴�.
��ü ������Ʈ�� �˻��ϴ� ���� �����. �׷��� ������ ������ ����� �˻��� �ϴ� ���� �ո����̶�� �����ؼ�
�ش� Ŭ������ �����ߴ�.
*/
class CACHE_ALGIN_CORE_DLL  USpace final : public UBase {
public:
	
public:
	using SPACECHILD = ARRAY<WKPTR<USpace>, MAX_OCTREENODE_LENGTH>;
public:
	USpace();
	NO_COPY(USpace)
	DESTRUCTOR(USpace)
public:

private:
	virtual void Free() override;
private:
	_llong									m_SpaceIndex;
	SHPTR<UTransform>		m_spTransform;
	SHPTR<UCollider>			m_spCollider;
	WKPTR<USpace>			m_wpParents;
	SPACECHILD						m_SpaceChild;
};

END


#endif // _SERVERFRAMEWORK_CORE_PUBLIC_USPACE_H