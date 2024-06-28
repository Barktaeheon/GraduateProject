#pragma once
#include "UController.h"

BEGIN(Engine)
class UAnimModel;
class UCharacter;
class UProcessedData;
/*
@ Date: 2024-04-28, Writer: ������
@ Explain
-  �ִϸ��̼��� ��Ʈ���ϴ� Ŭ����
*/
class UAnimationController abstract : public UController {
public: 
	struct ANIMCONTROLLERDESC {
		SHPTR<UCharacter> spCharacter;

		ANIMCONTROLLERDESC() : spCharacter{ nullptr } {}
		ANIMCONTROLLERDESC(CSHPTRREF<UCharacter> _spCharacter) : spCharacter{ _spCharacter } {}
	};

	enum COMMONSTATE
	{
		ANIM_IDLE, ANIM_MOVE, ANIM_ATTACK, ANIM_DEATH, ANIM_HIT
	};
public:
	UAnimationController(CSHPTRREF<UDevice> _spDevice);
	UAnimationController(const UAnimationController& _rhs);
	DESTRUCTOR(UAnimationController)
public:
	virtual void Free() override PURE;
	virtual SHPTR<UCloneObject> Clone(const VOIDDATAS& _tDatas) PURE;
public:
	// Native Construct 
	virtual HRESULT NativeConstruct() override PURE;
	// Clone 
	virtual HRESULT NativeConstructClone(const VOIDDATAS& _tDatas) override PURE;
	// Tick
	virtual void Tick(const _double& _dTimeDelta) override PURE;
	// ��Ʈ��ũ ������ �޾ƿ��� �Լ�
	virtual void ReceiveNetworkProcessData(void* _pData);
protected:
	void ClearTrigger() { this->m_wstrTrigger.clear(); }
public: /* get set*/
	const _int GetAnimState() const { return m_iAnimState;  }
	const _wstring& GetTrigger() const { return m_wstrTrigger; }

	void SetAnimState(const _int _iAnimState) { this->m_iAnimState = _iAnimState; }
	void SetTrigger(const _wstring& _wstrTrigger) { this->m_wstrTrigger = _wstrTrigger; }
protected:
	SHPTR<UCharacter> GetOwnerCharacter() { return m_wpOwnerCharacter.lock(); }
protected: /* get set  */
	void UpdateState(CSHPTRREF<UAnimModel> _spAnimModel, const _int _iAnimState, const _wstring& _wstrTrigger);
private:
	_int										m_iAnimState;
	_wstring									m_wstrTrigger;
	WKPTR<UCharacter>			m_wpOwnerCharacter;
};

END