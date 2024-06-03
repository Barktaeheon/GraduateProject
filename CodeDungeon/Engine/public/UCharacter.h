#pragma once

#include "UPawn.h"

BEGIN(Engine)
class UTransform;
class UAnimModel;
class UAnimationController;
class UNavigation;
/*
@ Date: 2024-02-25, Writer: ������
@ Explain
-  ����ڰ� �����̰ų� AI�� ������ �� ����ϴ� Ŭ�����̴�. 
*/
class UCharacter abstract : public UPawn {
public:
	enum{
		CHARACTERDESCORDER = 0
	};
	struct CHARACTERDESC {
		_wstring		wstrAnimModelProtoData;
		_wstring		wstrAnimControllerProtoData;

		CHARACTERDESC() :wstrAnimModelProtoData{L" "}, wstrAnimControllerProtoData{L" "}
		{ }
		CHARACTERDESC(const _wstring& _wstrAnimModelPathData, const _wstring& _wstrAnimControllerProtoData) :
			wstrAnimModelProtoData{ _wstrAnimModelPathData }, wstrAnimControllerProtoData{ _wstrAnimControllerProtoData }{}
	};
public:
	UCharacter(CSHPTRREF<UDevice> _spDevice, const _wstring& _wstrLayer, const CLONETYPE& _eCloneType);
	UCharacter(const UCharacter& _rhs);
	DESTRUCTOR(UCharacter)
public:
	// UPawn��(��) ���� ��ӵ�
	virtual SHPTR<UCloneObject> Clone(const VOIDDATAS& _vecDatas) PURE;
	virtual void Free() PURE;
	virtual HRESULT NativeConstruct() PURE;
	virtual HRESULT NativeConstructClone(const VOIDDATAS& _Datas) PURE;
public:
	// �ٸ� ĳ���Ϳ��� �Ÿ� ���� 
	_float OtherCharacterToDistance(CSHPTRREF<UActor> _spOtherActor);
	// �ٸ� ĳ���͸� �ٶ󺸴� �ڱ� �ڽ��� Look Angle 
	_float OtherCharacterDirToLook(CSHPTRREF<UActor> _spOtherActor);
	// �ٸ� ĳ���͸� �������� �ٶ󺸴� �ڱ� �ڽ��� Right Angle 
	_float OhterCharacterDirToRight(CSHPTRREF<UActor> _spOtherActor);
	// �ٸ� ĳ���͸� �ٶ󺸴� �ڱ� �ڽ��� Look Angle ( 180 ~ -180 )
	_float OtherCharacterDirToLookConverter(CSHPTRREF<UActor>  _spOtherActor);
	// �ٸ� ĳ���͸� �ٶ󺸴� �ڱ� �ڽ��� Look Direction
	_float3 OtherCharacterDirToLookVectorF3(CSHPTRREF<UActor>  _spOtherActor);
	// �ٸ� ĳ���Ϳ��� �Ÿ� ���� 
	_float OtherCharacterToDistance(CSHPTRREF<UTransform> _spOtherTransform);
	// �ٸ� ĳ���͸� �ٶ󺸴� �ڱ� �ڽ��� Look Angle 
	_float OtherCharacterDirToLook(CSHPTRREF<UTransform> _spOtherTransform);
	// �ٸ� ĳ���͸� �������� �ٶ󺸴� �ڱ� �ڽ��� Right Angle 
	_float OhterCharacterDirToRight(CSHPTRREF<UTransform> _spOtherTransform);
	// �ٸ� ĳ���͸� �ٶ󺸴� �ڱ� �ڽ��� Look Angle ( 180 ~ -180 )
	_float OtherCharacterDirToLookConverter(CSHPTRREF<UTransform> _spOtherTransform);
	// �ٸ� ĳ���͸� �ٶ󺸴� �ڱ� �ڽ��� Look Direction
	_float3 OtherCharacterDirToLookVectorF3(CSHPTRREF<UTransform> _spOtherTransform);
public: /* get set */
	CSHPTRREF<UAnimModel> GetAnimModel() const { return m_spAnimModel; }
	SHPTR<UNavigation> GetCurrentNavi() const { return m_wpCurNavi; }
	CSHPTRREF<UCollider> GetHitCollider() const { return m_spHitCollider; }

	const _bool& GetHitState() const { return m_bisHit; }
	void SetHitstate(_bool _newState) { m_bisHit = _newState; }

	const _bool& GetCollisionState() const { return m_bisCollision; }
	void SetCollisionState(_bool _newState) { m_bisCollision = _newState; }
protected:
	virtual void TickActive(const _double& _dTimeDelta) PURE;
	virtual void LateTickActive(const _double& _dTimeDelta) PURE;
	virtual HRESULT RenderActive(CSHPTRREF<UCommand> _spCommand, CSHPTRREF<UTableDescriptor> _spTableDescriptor) PURE;
	virtual HRESULT RenderShadowActive(CSHPTRREF<UCommand> _spCommand, CSHPTRREF<UTableDescriptor> _spTableDescriptor) PURE;
	virtual void Collision(CSHPTRREF<UPawn> _pEnemy) PURE;

	virtual HRESULT MakeCollider(const _float3& _vTranslate, const _float3& _vScale, const _int _ColliderType);
protected: /* get set */
	CSHPTRREF<UAnimationController> GetAnimationController() const { return m_spAnimationController; }
	const _float3& GetPrevPos() const { return m_vPrevPos; }
	const _float GetMovingSpeed() const { return m_fMoveSpeed; }
	const _float GetRunningSpeed() const { return m_fRunSpeed; }

	void SetMovingSpeed(const float& _newSpeed) { m_fMoveSpeed = _newSpeed; }
	void SetRunningSpeed(const float& _newSpeed) { m_fRunSpeed = _newSpeed; }

	void SetRunState(_bool _runState) { m_bIsRunning = _runState; }
private:
	// AnimationModel
	SHPTR< UAnimModel>				m_spAnimModel;

	SHPTR<UAnimationController>		m_spAnimationController;
	// ���� ��ġ ����
	_float3								m_vPrevPos;
	// ���� ��������
	SHPTR<UNavigation>		m_wpCurNavi;
	SHPTR<UCollider>			m_spHitCollider;

	//�̵� �ӵ�
	_float							m_fMoveSpeed;
	_float							m_fRunSpeed;
	_bool							m_bIsRunning;
	_bool							m_bisHit;
	_bool							m_bisCollision;
};

END
