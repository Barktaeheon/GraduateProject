#ifndef _SERVERFRAMEWORK_CORE_PUBLIC_AGAMEOBJECT_H
#define _SERVERFRAMEWORK_CORE_PUBLIC_AGAMEOBJECT_H

#include "ACoreObject.h"

BEGIN(Core)
class ATransform;
class ACollider;
class ASpace;

/*
@ Date: 2024-07-04, Writer: ������
@ Explain
- ������ �ʿ��� Object�� �����Ѵ�. 
*/
class CORE_DLL AGameObject abstract : public ACoreObject {
	using BUFFER = ARRAY<_char, MAX_BUFFER_LENGTH>;
public:
	AGameObject(OBJCON_CONSTRUCTOR, SESSIONID _ID, SESSIONTYPE _SessionType);
	DESTRUCTOR(AGameObject)
public:
	virtual _bool Start(const VOIDDATAS& _ReceiveDatas = {}) PURE;
	void CreateColliderAndTransform(const COLLIDERINFO& _ColliderInfo, const Vector3& _vPos);
	void BringSpaceIndex(SHPTR<ASpace> _spSpace);
	void Placement(_int _CellIndex);
	void BringCellIndextoPosition();

	// �ٸ� ĳ���Ϳ��� �Ÿ� ���� 
	_float OtherCharacterToDistance(SHPTR<ATransform> _spOtherTransform);
	// �ٸ� ĳ���͸� �ٶ󺸴� �ڱ� �ڽ��� Look Angle 
	_float OtherCharacterDirToLook(SHPTR<ATransform> _spOtherTransform);
	// �ٸ� ĳ���͸� �������� �ٶ󺸴� �ڱ� �ڽ��� Right Angle 
	_float OhterCharacterDirToRight(SHPTR<ATransform> _spOtherTransform);
	// �ٸ� ĳ���͸� �ٶ󺸴� �ڱ� �ڽ��� Look Angle ( 180 ~ -180 )
	_float OtherCharacterDirToLookConverter(SHPTR<ATransform> _spOtherTransform);
	// �ٸ� ĳ���͸� �ٶ󺸴� �ڱ� �ڽ��� Look Direction
	Vector3 OtherCharacterDirToLookVectorF3(SHPTR<ATransform> _spOtherTransform);

	_bool IsCanSee(Vector3 _OtherPos);
	_bool IsCanSee(SHPTR<ATransform> _spTransform);
public:/*Get Set */
	const SESSIONID& GetSessionID() const { return m_SessionID; }
	const SESSIONTYPE& GetSessionType() const { return m_SessionType; }
	const _int& GetSpaceIndex() const { return m_SpaceIndex; }
	const CHARSTATUS& GetCharStatus() const { return m_CharStatus; }
	SHPTR<ATransform> GetTransform() const { return m_spTransform; }
	SHPTR<ACollider> GetCollider() const { return m_spCollider; }
	const _int GetGameObjectType() const { return m_GameObjectType; }
	const _int GetCellIndex() const { return m_CellIndex; }
	const _bool IsJumpable() const { return m_isJumpable; }
	const _float GetMoveSpeed() const { return m_fMoveSpeed; }
	const _float GetRunSpeed() const { return m_fRunSpeed; }
	const _int GetCurOnCellIndex() const { return m_iCurOnCellIndex; }
	const _bool IsActive() const { return m_isActive; }
	const _bool IsPermanentDisable() const { return m_isPermanentDisable; }

	void SetJumpable(const _bool _isJumpable) { this->m_isJumpable = _isJumpable; }
	virtual void SetActiveWeak(const _bool _isActive);
	virtual void SetActiveStrong(const _bool _isActive);
	void ActivePermanentDisable();
	/*
	@ Date: 2024-01-04, Writer: ������
	@ Explain
	- ProtocolBuffer�� �����ϱ� ���� �Լ��̴�.
	*/
	template<class T>
	void CombineProto(REF_IN BUFFER& _Buffer, REF_IN PACKETHEAD& _PacketHead, const T& _data, _int _tag)
	{
		_data.SerializePartialToArray((void*)&_Buffer[0], static_cast<int>(_data.ByteSizeLong()));
		_PacketHead.PacketSize = static_cast<short>(_data.ByteSizeLong());
		_PacketHead.PacketType = static_cast<short>(_tag);
	}
protected:
	// ���������� �ش� ������Ʈ�� ������� �ʵ��� ������ ��� ���� �޽��� 
	virtual void SendLastMessage();
protected: /* Get Set */
	void SetSessionID(const SESSIONID& _SessionID) { this->m_SessionID = _SessionID; }
	void SetSessionType(const SESSIONTYPE& _SessionType) { this->m_SessionType = _SessionType; }
	void SetSpaceIndex(const _int& _SpaceIndex) { this->m_SpaceIndex = _SpaceIndex; }
	void SetCharStatus(const CHARSTATUS& _charStatus) { ::memcpy(&m_CharStatus, &_charStatus, sizeof(CHARSTATUS)); }
	void SetGameObjectType(const _int _GameObjectType) { this->m_GameObjectType = _GameObjectType; }
	void SetCellIndex(const _int _CellIndex) { this->m_CellIndex = _CellIndex; }
	void SetMoveSpeed(const _float _fMoveSpeed) { this->m_fMoveSpeed = _fMoveSpeed; }
	void SetRunSpeed(const _float _fRunSpeed) { this->m_fRunSpeed = _fRunSpeed; }
	void SetCurOnCellIndex(const _int _iCurOnCellIndex) { this->m_iCurOnCellIndex = _iCurOnCellIndex; }

	PACKETHEAD& GetPacketHead() { return m_CopyHead; }
	BUFFER& GetCopyBuffer(REF_RETURN) { return m_CopyBuffer; }
	_char* GetCopyBufferPointer(_int _iBufferIndex = 0) { return &m_CopyBuffer[0]; }
private:
	virtual void Free() override;
public:
	static	constexpr _int		SEE_RANGE{ 100 };
private:
	SESSIONID							m_SessionID;
	SESSIONTYPE					m_SessionType;
	_int										m_SpaceIndex;
	_int										m_GameObjectType;
	_int										m_CellIndex;
	_bool									m_isJumpable;
	CHARSTATUS					m_CharStatus;

	SHPTR<ATransform>		m_spTransform;
	SHPTR<ACollider>			m_spCollider;
	// ���� �� ���� �ִ� �ε���
	_int										m_iCurOnCellIndex;

	_float									m_fMoveSpeed;
	_float									m_fRunSpeed;
	ATOMIC<_bool>				m_isActive;
	ATOMIC<_bool>				m_isPermanentDisable;

	PACKETHEAD					m_CopyHead;
	BUFFER								m_CopyBuffer;
};


END

#endif // _SERVERFRAMEWORK_CORE_PUBLIC_AGAMEOBJECT_H