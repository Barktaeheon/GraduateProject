#ifndef _SERVERFRAMEWORK_CORE_PUBLIC_AANIMATOR_H
#define _SERVERFRAMEWORK_CORE_PUBLIC_AANIMATOR_H

#include "ACoreObject.h"

BEGIN(Core)
class AAnimation;
class ACollider;
class ABoneNode;
class ARootBoneNode;
class ATransform;
class APawn;

using ANIMCONTAINER = VECTOR<SHPTR<AAnimation>>;
using BONECONTAINER = VECTOR<SHPTR<ABoneNode>>;
using SELECTOR = CONUNORMAP<_string, _int>;
using ANIMEVENTCOLLICONTAINER = CONUNORMAP<_int, SHPTR<ACollider>>;
/*
@ Date: 2024-07-03
@ Writer: ������
@ Explain
- �������� ����ϴ� �ִϸ�����
*/
class CORE_DLL AAnimator final : public ACoreObject {
public:
	AAnimator(SHPTR<ACoreInstance> _spCoreInstance, const _string& _strFolderPath, const _string& _strFileName);
	AAnimator(const AAnimator& _rhs);
	DESTRUCTOR(AAnimator)
public:
	// Tick Event 
	void TickEvent(APawn* _pPawn, const _string& _strInputTrigger, const _double& _TimeDelta);
	// Tick Event 
	void TickEventToRatio(APawn* _pPawn, const _string& _strInputTrigger, const _double& _dRatio, const _double& _TimeDelta);
	// �ִϸ��̼��� timedelta�� �帧�� ���� �����ϴ� �Լ� (TimeDelta�� ���� ���ӿ��� �帣�� �ð�)
	void TickAnimation(const _double& _dTimeDelta);
	// �ִϸ��̼��� Tick�ϸ鼭 Event�� Tick�ϴ� �Լ�
	void TickAnimChangeTransform(CSHPTRREF<ATransform> _spTransform, const _double& _dTimeDelta);
	// ���� �ִϸ��̼��� TimAcc ���� ���� �����ϴ� �Լ� Evnet�� TimeAcc�� ���� �����Ѵ�. 
	void TickAnimToTimeAccChangeTransform(CSHPTRREF<ATransform> _spTransform, const _double& _dTimeDelta, const _double& _TimeAc);
	// �ִϸ��̼��� timeAcc�� ���� �����ϴ� �Լ� (TimAcc�� �ִϸ��̼� ���)
	void UpdateCurAnimationToRatio(const _double& _dRatio);
	// Get BoneNode
	SHPTR<ABoneNode> FindBoneNode(const _string& _strBoneNode) const;
	// Update 
	void UpdateAttackData(const _bool _isCanAttackSituation, CSHPTRREF<ACollider> _spCollider);
	// AnimEvent Reset
	void ResetCurAnimEvent();
	// Set Animation
	void SetAnimation(const _int& _iAnimIndex);
	void SetAnimation(const _string& _strAnimName);
	void SetAnimation(const _int& _iAnimIndex, const _double& _dNextTimeAcc);
	void SetAnimation(const _string& _strAnimName, const _double& _dNextTimeAcc);
	// Change Animation
	void ChangeAnimation(const _int& _iAnimIndex);
	void ChangeAnimation(const _string& _strAnimName);
	// Change Animation, Apply Next Time Acc 
	void ChangeAnimation(const _int& _iAnimIndex, const _double& _dNextTimeAcc);
	void ChangeAnimation(const _string& _strAnimName, const _double& _dNextTimeAcc);

	SHPTR<ACollider> BringAttackCollider(_int _iColliderType);
	SHPTR<ACollider> GetAttackCollider() { return m_spAttackCollisionCollider; }
	_bool IsCollisionAttackCollider(SHPTR<ACollider> _spEnemyCollider);
public: /* get set */
	const _int GetCurAnimIndex() const { return m_iCurAnimIndex; }
	SHPTR<AAnimation> GetCurAnimation() const { return m_spCurAnimation; }
	SHPTR<AAnimation> GetNextAnimation() const { return m_spNextAnimation; }

	void SetSupplyLerpValue(const _float _fSupplyLerpValue) { this->m_fSupplyLerpValue = _fSupplyLerpValue; }
protected:
	void ChangeAnimIndex(const _int& _iAnimIndex, _int& _iIndex);
	void ChangeAnimIndex(const _string& _strAnimName, _int& _iIndex);
	void SettingCurAnimSituation();
	void SettingNextAnimSituation();
private:
	virtual void Free() override;
private:
	_int													m_iCurAnimIndex;
	_int													m_iNextAnimIndex;
	_float4x4										m_PivotMatirx;
	_float												m_fSupplyLerpValue;
	_bool												m_isCanAttackSituation;

	BONECONTAINER							m_BoneContainer;
	SELECTOR										m_BoneSelector;
	ANIMCONTAINER							m_AnimContainer;
	SELECTOR										m_AnimSelector;

	SHPTR<ARootBoneNode>			m_spRootBoneNode;
	SHPTR<AAnimation>					m_spCurAnimation;
	SHPTR<AAnimation>					m_spNextAnimation;
	ANIMEVENTCOLLICONTAINER	m_AnimEventColliderContainer;
	SHPTR<ACollider>						m_spAttackCollisionCollider;
};

END

#endif // _SERVERFRAMEWORK_CORE_PUBLIC_AANIMATOR_H