#pragma once
#include "UAnimEvent.h"

BEGIN(Engine)

/*
@ Date: 2024-02-21, Writer: ������
@ Explain
- �������� �̺�Ʈ�� �߻���Ű�� Ŭ����
*/
class UAnimSectionEvent final : public UAnimEvent {
public:
	UAnimSectionEvent();
	UAnimSectionEvent(std::ifstream& _load);
	DESTRUCTOR(UAnimSectionEvent)
public:
	virtual _bool EventCheck(UAnimModel* _pAnimModel, const _double& _dTimeDelta, const _double& _dTimeAcc,
		const _wstring& _wstrInputTrigger) override;
	virtual ANIMEVENTDESC* const OutAnimEventDesc() override { return &m_AnimSectionDesc; }
	virtual void ChangeAnimEventDesc(ANIMEVENTDESC* _AnimEventDesc) override;
	virtual ANIMOTHEREVENTDESC* const OutOtherEventDesc() override;
	virtual void ChangeOtherEventDesc(ANIMOTHEREVENTDESC* _AnimOtherEventDesc) override;
protected:
	// Event ��Ȳ�� ���� ����
	virtual void EventSituation(UAnimModel* _pAnimModel, const _double& _dTimeDelta) override;
	virtual void SaveEvent(std::ofstream& _save) override;
	virtual void LoadEvent(std::ifstream& _load) override;
private:
	virtual void Free() override;
private:
	ANIMEVENTSECTIONDESC			m_AnimSectionDesc;
	// �ִϸ��̼ǰ� �ִϸ��̼� ���̸� �����ϴ� �̺�Ʈ
	ANIMCHANGEDESC						m_AnimChangeDesc;
};

END
