#pragma once
#include "AnimEventParents.h"


BEGIN(Engine)
class USound;
class UCharacter;
/*
@ Date: 2024-02-17, Writer: ������
@ Explain
- ���� ���̿��� Ư�� �̺�Ʈ�� �߻��� �� ���� �ִϸ��̼����� �Ѿ�� ����� �̺�Ʈ�̴�.
*/
class UAnimOccursTimePassEvent final : public UAnimOccurEvent {
public:
	UAnimOccursTimePassEvent();
	UAnimOccursTimePassEvent(const UAnimOccursTimePassEvent& _rhs);
	UAnimOccursTimePassEvent(CSHPTRREF<UAnimModel> _spAnimModel, std::ifstream& _load);
	DESTRUCTOR(UAnimOccursTimePassEvent)
public:
	virtual SHPTR<UAnimEvent> Clone(UAnimModel* _pAnimModel = nullptr) override;
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
UAnimOccursTimePassEvent
=================================================
AnomSoundEvent
=================================================
*/

class UAnimSoundEvent final : public UAnimOccurEvent {
public:
	UAnimSoundEvent();
	UAnimSoundEvent(const UAnimSoundEvent& _rhs);
	UAnimSoundEvent(CSHPTRREF<UAnimModel> _spAnimModel, std::ifstream& _load);
	DESTRUCTOR(UAnimSoundEvent)
public:
	virtual SHPTR<UAnimEvent> Clone(UAnimModel* _pAnimModel = nullptr) override;
	// UAnimSectionEvent��(��) ���� ��ӵ�
	virtual const ANIMOTHEREVENTDESC* OutOtherEventDesc() override;
protected:
	// Event ��Ȳ�� ���� ����
	virtual void EventSituation(UPawn* _pPawn, UAnimModel* _pAnimModel, const _double& _dTimeDelta, const _double& _dTimeAcc) override;
	virtual void SaveEvent(std::ofstream& _save) override;
	virtual void LoadEvent(CSHPTRREF<UAnimModel> _spAnimModel, std::ifstream& _load) override;
private:
	virtual void Free() override;
private:
	ANIMSOUNDDESC			m_AnimSoundDesc;
	SHPTR<USound>				m_spSound;
	SHPTR<UCharacter>		m_spPlayerCharacter;


};

END