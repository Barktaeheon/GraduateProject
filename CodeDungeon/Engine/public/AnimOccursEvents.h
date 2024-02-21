#pragma once
#include "UAnimEvent.h"


BEGIN(Engine)

/*
@ Date: 2024-02-17, Writer: ������
@ Explain
- ���� ���̿��� Ư�� �̺�Ʈ�� �߻��� �� ���� �ִϸ��̼����� �Ѿ�� ����� �̺�Ʈ�̴�.
*/
class UAnimOccurEvent final : public UAnimEvent {
public:
	UAnimOccurEvent();
	UAnimOccurEvent(std::ifstream& _load);
	DESTRUCTOR(UAnimOccurEvent)
public:
	virtual _bool EventCheck(UAnimModel* _pAnimModel, const _double& _dTimeDelta, const _double& _dTimeAcc,
		const _wstring& _wstrInputTrigger) override;

	virtual ANIMEVENTDESC* const OutAnimEventDesc() override;
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
	ANIMOCURRESDESC					m_AnimOccurDesc;
	// �ִϸ��̼ǰ� �ִϸ��̼� ���̸� �����ϴ� �̺�Ʈ
	ANIMCHANGEDESC						m_AnimChangeDesc;
};

END