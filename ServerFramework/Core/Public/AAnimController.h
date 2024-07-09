#ifndef _SERVERFRAMEWORK_CORE_PUBLIC_AANIMCONTROLLER_H
#define _SERVERFRAMEWORK_CORE_PUBLIC_AANIMCONTROLLER_H

#include "ACoreObject.h"

BEGIN(Core)
class AAnimator;
class APawn;
class ABoneNode;
class ACollider;
class AAnimation;

class CORE_DLL AAnimController abstract : public ACoreObject {
public:
	AAnimController(OBJCON_CONSTRUCTOR, SHPTR<APawn> _spPawn, 
		const _string& _strFolderPath, const _string& _strFileName);
	DESTRUCTOR(AAnimController)
public:
	virtual void Tick(const _double& _dTimeDelta) PURE;

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
	SHPTR<ACollider> GetAttackCollider();
	_bool IsCollisionAttackCollider(SHPTR<ACollider> _spEnemyCollider);
public: /* get set */
	const _int GetCurAnimIndex() const;
	SHPTR<AAnimation> GetCurAnimation() const;
	SHPTR<AAnimation> GetNextAnimation() const;
	SHPTR<APawn> GetOwner() const { return m_wpOwnerPawn.lock(); }
	_bool IsOwnerPawnActive() const { return m_isOwnerPawnActive; }
	_int GetPawnState() const { return m_iPawnState; }

	void SetSupplyLerpValue(const _float _fSupplyLerpValue);
	void SetOwnerPawnActiveStrong(_bool _isOwnerPawnActive);
	void SetOwnerPawnActiveWeak(_bool _isOwnerPawnActive);
	void SetPawnState(_int _State);
protected: /* get set */
	SHPTR<AAnimator> GetAnimator() { return m_spAnimator; }
	const _double GetAccumulator() const { return m_dAccumulator; }
	const _double GetElapsedTime() const { return m_dElapsedTime; }
	const _string& GetInputTrigger() const { AReadSpinLockGuard(m_TriggerLock); return m_strInputTrigger; }

	void SetAccumulator(const _double& _dAccumulator) { this->m_dAccumulator = _dAccumulator; }
	void SetElapsedTime(const _double& _dDelapsedTime) { this->m_dElapsedTime = _dDelapsedTime; }
	void SetInputTrigger(const _string& _inputTrigger) { AWriteSpinLockGuard(m_TriggerLock);  this->m_strInputTrigger = _inputTrigger; }

private:
	virtual void Free() override;
private:
	_double							m_dAccumulator;
	_double							m_dElapsedTime;
	_string							m_strInputTrigger;
	// Owner Pawn Active
	ATOMIC<_bool>			m_isOwnerPawnActive;
	ATOMIC<_int>				m_iPawnState;

	AFastSpinLock				m_TriggerLock;
	// Animator, OwenrPawn
	SHPTR<AAnimator>	m_spAnimator;
	WKPTR<APawn>			m_wpOwnerPawn;
};

END

#endif // _SERVERFRAMEWORK_CORE_PUBLIC_AANIMCONTROLLER_H
