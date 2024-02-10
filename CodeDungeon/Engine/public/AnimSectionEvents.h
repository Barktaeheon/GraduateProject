#pragma once
#include "AnimEventParents.h"

BEGIN(Engine)

/*
@ Date: 2024-02-08, Writer: ������
@ Explain
- ���� ���̿��� Ư�� �̺�Ʈ�� �߻��� �� ���� �ִϸ��̼����� �Ѿ�� ����� �̺�Ʈ�̴�. 
*/
class UAnimChangeBetweenEvent final : public UAnimSectionEvent{
public:
	UAnimChangeBetweenEvent();
	UAnimChangeBetweenEvent(std::ifstream& _load);
	UAnimChangeBetweenEvent(const ANIMEVENTSECTIONDESC& _AnimEventDesc);
	DESTRUCTOR(UAnimChangeBetweenEvent)
public:
	// UAnimSectionEvent��(��) ���� ��ӵ�
	ANIMOTHEREVENTDESC* const OutOtherEventDesc() override;
	void ChangeOtherEventDesc(ANIMOTHEREVENTDESC* _AnimOtherEventDesc) override;
protected:
	// Event ��Ȳ�� ���� ����
	virtual void EventSituation(CSHPTRREF<UAnimator> _spAnimator, const _double& _dTimeDelta) override;
	virtual void SaveEvent(std::ofstream& _save) override;
	virtual void LoadEvent(std::ifstream& _load) override;
private:
	virtual void Free() override;
private:
	ANIMCHANGEBETWEENEVENTDESC		m_ACBEventDesc;
};

END