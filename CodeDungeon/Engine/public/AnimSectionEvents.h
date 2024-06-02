#pragma once
#include "AnimEventParents.h"

BEGIN(Engine)
class UCollider;
class UBoneNode;

/*
@ Date: 2024-02-17, Writer: ������
@ Explain
- ���� ���̿��� Ư�� �̺�Ʈ�� �߻��� �� ���� �ִϸ��̼����� �Ѿ�� ����� �̺�Ʈ�̴�. 
*/
class UAnimChangeBetweenEvent final : public UAnimSectionEvent{
public:
	UAnimChangeBetweenEvent();
	UAnimChangeBetweenEvent(const UAnimChangeBetweenEvent& _rhs);
	UAnimChangeBetweenEvent(CSHPTRREF<UAnimModel> _spAnimModel, std::ifstream& _load);
	DESTRUCTOR(UAnimChangeBetweenEvent)
public:
	virtual SHPTR<UAnimEvent> Clone(UAnimModel* _pAnimModel = nullptr) override;
public:
	// UAnimSectionEvent��(��) ���� ��ӵ�
	virtual const ANIMOTHEREVENTDESC*  OutOtherEventDesc() override;
protected:
	// Event ��Ȳ�� ���� ����
	virtual void EventSituation(UPawn* _pPawn, UAnimModel* _pAnimModel, const _double& _dTimeDelta, const _double& _dTimeAcc) override;
	virtual void SaveEvent(std::ofstream& _save) override;
	virtual void LoadEvent(CSHPTRREF<UAnimModel> _spAnimModel, std::ifstream& _load) override;
private:
	virtual void Free() override;
private:
	// �ִϸ��̼ǰ� �ִϸ��̼� ���̸� �����ϴ� �̺�Ʈ
	ANIMCHANGEDESC		m_AnimChangeDesc;
};

/*
=================================================
AnimChangeBetweenEvent
=================================================
AnimColliderEvent
=================================================
*/

/*
@ Date: 2024-02-22, Writer: ������
@ Explain
- ���� ���̿��� Ư�� �̺�Ʈ�� �߻��� �� ���� �ִϸ��̼����� �Ѿ�� ����� �̺�Ʈ�̴�.
*/
class UAnimColliderEvent final : public UAnimSectionEvent {
public:
	UAnimColliderEvent();
	UAnimColliderEvent(const UAnimColliderEvent& _rhs);
	UAnimColliderEvent(CSHPTRREF<UAnimModel> _spAnimModel, std::ifstream& _load);
	DESTRUCTOR(UAnimColliderEvent)
public:
	virtual SHPTR<UAnimEvent> Clone(UAnimModel* _pAnimModel = nullptr) override;
public:
	virtual _bool EventCheck(UPawn* _pPawn, UAnimModel* _pAnimModel, const _double& _dTimeDelta, const _double& _dTimeAcc, 
		const _wstring& _wstrInputTrigger) override;
	// UAnimSectionEvent��(��) ���� ��ӵ�
	virtual const  ANIMOTHEREVENTDESC*  OutOtherEventDesc() override;
protected:
	// Event ��Ȳ�� ���� ����
	virtual void EventSituation(UPawn* _pPawn, UAnimModel* _pAnimModel, const _double& _dTimeDelta, const _double& _dTimeAcc) override;
	virtual void SaveEvent(std::ofstream& _save) override;
	virtual void LoadEvent(CSHPTRREF<UAnimModel> _spAnimModel, std::ifstream& _load) override;
private:
	virtual void Free() override;
private:
	ANIMCOLLIDERDESC	m_AnimColliderDesc;
};

/*
=================================================
AnimColliderEvent
=================================================
AnimParticleEvent
=================================================
*/



END