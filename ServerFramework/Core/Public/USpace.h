#ifndef _SERVERFRAMEWORK_CORE_PUBLIC_USPACE_H
#define _SERVERFRAMEWORK_CORE_PUBLIC_USPACE_H

#include "UBase.h"

BEGIN(Core)
class UTransform;
class UCollider;
class USession;

/*
@ Date: 2023-01-22, Writer: ������
@ Explain
- ���ӻ��� ������ �����Ͽ� �þ� Ȯ���� �ϱ� ���� Ŭ�����̴�.
��ü ������Ʈ�� �˻��ϴ� ���� �����. �׷��� ������ ������ ����� �˻��� �ϴ� ���� �ո����̶�� �����ؼ�
�ش� Ŭ������ �����ߴ�.
*/
class CORE_DLL  USpace final : public UBase {
public:
	using SESSIONCONTAINER = CONLIST<SHPTR<USession>>;
	using SPACECHILD = CONVECTOR<SHPTR<USpace>>;
public:
	USpace();
	NO_COPY(USpace)
	DESTRUCTOR(USpace)
public:
	_bool NativeConstruct(const SPACEINFO& _SpaceInfo, SHPTR<USpace> _spParents = nullptr);
	_bool InsertSession(SHPTR<USession> _spSession);
	void BroadCastMessage(_char* _pPacket, const PACKETHEAD& _PacketHead);
	_int GetChildIndexToPosition(const Vector3 _vPos);
	_bool IsContains(SHPTR<UCollider> _spCollider);

	SHPTR<USpace> MakeChild(const _int _Index, const SPACEINFO& _SpaceInfo);
	void OnChildNode() { m_isChildNode = true; }
public: /* get set */
	const _int GetSpaceIndex() const { return m_SpaceIndex; }
	const _int GetDepthLevel() const { return m_DepthLevel; }
	const _bool IsChildNode() const { return m_isChildNode; }
	SHPTR<UCollider> GetCollider() const { return m_spCollider; }
	const SPACECHILD& GetSpaceChild() const { return m_SpaceChild; }
protected:
	_bool InsertSessionRecursively(CSHPTRREF<USession> _spSession);
private:
	virtual void Free() override;
private:
	_int												m_SpaceIndex;
	_int												m_DepthLevel;
	_bool											m_isChildNode;
	// Child + Parents
	WKPTR<USpace>					m_wpParents;
	SPACECHILD								m_SpaceChild;
	SHPTR<UCollider>					m_spCollider;
	// LIST
	SESSIONCONTAINER				m_SessionContainer;
};

END


#endif // _SERVERFRAMEWORK_CORE_PUBLIC_USPACE_H