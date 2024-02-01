#ifndef _SERVERFRAMEWORK_CORE_PUBLIC_ASPACE_H
#define _SERVERFRAMEWORK_CORE_PUBLIC_ASPACE_H

#include "ACoreBase.h"

BEGIN(Core)
class ATransform;
class ACollider;
class ASession;

/*
@ Date: 2023-01-22, Writer: ������
@ Explain
- ���ӻ��� ������ �����Ͽ� �þ� Ȯ���� �ϱ� ���� Ŭ�����̴�.
��ü ������Ʈ�� �˻��ϴ� ���� �����. �׷��� ������ ������ ����� �˻��� �ϴ� ���� �ո����̶�� �����ؼ�
�ش� Ŭ������ �����ߴ�.
*/
class CORE_DLL  ASpace final : public ACoreBase {
public:
	using SESSIONCONTAINER = CONLIST<SHPTR<ASession>>;
	using SPACECHILD = CONVECTOR<SHPTR<ASpace>>;
public:
	ASpace();
	NO_COPY(ASpace)
	DESTRUCTOR(ASpace)
public:
	_bool NativeConstruct(const SPACEINFO& _SpaceInfo, SHPTR<ASpace> _spParents = nullptr);
	_bool InsertSession(SHPTR<ASession> _spSession);
	void BroadCastMessage(_char* _pPacket, const PACKETHEAD& _PacketHead);
	_int GetChildIndexToPosition(const Vector3 _vPos);
	_bool IsContains(SHPTR<ACollider> _spCollider);

	SHPTR<ASpace> MakeChild(const _int _Index, const SPACEINFO& _SpaceInfo);
	void OnChildNode() { m_isChildNode = true; }
public: /* get set */
	const _int GetSpaceIndex() const { return m_SpaceIndex; }
	const _int GetDepthLevel() const { return m_DepthLevel; }
	const _bool IsChildNode() const { return m_isChildNode; }
	SHPTR<ACollider> GetCollider() const { return m_spCollider; }
	const SPACECHILD& GetSpaceChild() const { return m_SpaceChild; }
protected:
	_bool InsertSessionRecursively(CSHPTRREF<ASession> _spSession);
private:
	virtual void Free() override;
private:
	_int												m_SpaceIndex;
	_int												m_DepthLevel;
	_bool											m_isChildNode;
	// Child + Parents
	WKPTR<ASpace>					m_wpParents;
	SPACECHILD								m_SpaceChild;
	SHPTR<ACollider>					m_spCollider;
	// LIST
	SESSIONCONTAINER				m_SessionContainer;
};

END


#endif // _SERVERFRAMEWORK_CORE_PUBLIC_ASPACE_H