#ifndef _SERVERFRAMEWORK_CORE_PUBLIC_USPACE_H
#define _SERVERFRAMEWORK_CORE_PUBLIC_USPACE_H

#include "UBase.h"

BEGIN(Core)
class UTransform;
class UCollider;
class USession;

struct SPACEINFO {
	Vector3		vCenter;
	Vector3		vExtents;

	SPACEINFO() : vCenter{}, vExtents{}{}
	SPACEINFO(Vector3 _vCenter, Vector3 _vExtents) : vCenter{_vCenter}, vExtents{_vExtents}{}
};

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
	using SPACECHILD = CONVECTOR<SHPTR<USpace>>;
public:
	USpace();
	NO_COPY(USpace)
	DESTRUCTOR(USpace)
public:
	_bool NativeConstruct(REF_IN _int& _CurIndex, const _int _MaxCnt, const SPACEINFO& _SpaceInfo, SHPTR<USpace> _spParents = nullptr);
	// Insert
	void InsertSession(SHPTR<USession> _spSession);
	void Rebalance();
public: /* get set */
	SHPTR<UCollider> GetCollider() const { return m_spCollider; }
private:
	virtual void Free() override;
private:
	// Child + Parents
	WKPTR<USpace>					m_wpParents;
	SPACECHILD								m_SpaceChild;
	SHPTR<UCollider>					m_spCollider;
	// LIST
	SESSIONCONTAINER				m_SessionContainer;
};

END


#endif // _SERVERFRAMEWORK_CORE_PUBLIC_USPACE_H