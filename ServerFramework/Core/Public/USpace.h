#ifndef _SERVERFRAMEWORK_CORE_PUBLIC_USPACE_H
#define _SERVERFRAMEWORK_CORE_PUBLIC_USPACE_H

#include "UBase.h"

BEGIN(Core)
class UTransform;
class UCollider;
class USession;
/*
@ Date: 2023-01-21, Writer: ������
@ Explain
- ���ӻ��� ������ �����Ͽ� �þ� Ȯ���� �ϱ� ���� Ŭ�����̴�.
��ü ������Ʈ�� �˻��ϴ� ���� �����. �׷��� ������ ������ ����� �˻��� �ϴ� ���� �ո����̶�� �����ؼ�
�ش� Ŭ������ �����ߴ�.
*/
class CORE_DLL  USpace final : public UBase {
public:
	using SESSIONCONTAINER = CONLIST<SHPTR<USession>>;
public:
	using SPACECHILD = ARRAY<WKPTR<USpace>, MAX_OCTREENODE_LENGTH>;
public:
	USpace();
	NO_COPY(USpace)
	DESTRUCTOR(USpace)
public:
	// Insert
	void InsertSession(SHPTR<UCollider>	 _spCollider, SHPTR<USession> _spSession);
	void Rebalance(SHPTR<UCollider>	 _spCollider, SHPTR<USession> _spSession);
	
private:
	virtual void Free() override;
private:
	_llong											m_SpaceIndex;
	// Child + Parents
	WKPTR<USpace>					m_wpParents;
	SPACECHILD								m_SpaceChild;
	// Components
	SHPTR<UTransform>				m_spTransform;
	SHPTR<UCollider>					m_spCollider;
	// LIST
	SESSIONCONTAINER				m_SessionContainer;
};

END


#endif // _SERVERFRAMEWORK_CORE_PUBLIC_USPACE_H