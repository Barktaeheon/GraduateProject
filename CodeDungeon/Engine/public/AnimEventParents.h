#pragma once

#include "UBase.h"

BEGIN(Engine)
class UAnimModel;
class UAnimator;
/*
@ Date: 2024-02-10, Writer: ������
@ Explain
- Animation Event�� ������ �θ� Ŭ����
*/
class UAnimEvent abstract : public UBase {
protected:
	UAnimEvent(ANIMEVENTTYPE _AnimEventType, ANIMEVENTCATEGORY _AnimEventCategory);
	DESTRUCTOR(UAnimEvent)
public:
	const ANIMEVENTTYPE GetAnimEventType() const { return m_AnimEventType; }
	const ANIMEVENTCATEGORY GetAnimEventCategory() const { return m_AnimEventCategory; }
public:
	virtual _bool EventCheck(UAnimModel* _pAnimModel, const _double& _dTimeDelta, const _double& _dTimeAcc, 
		const _wstring& _wstrInputTrigger) PURE;
	// AnimEventDesc�� ��ӹ޴� ����ü�� �����ϴ� �Լ�
	virtual ANIMEVENTDESC* const OutAnimEventDesc() PURE;
	// AnimEvent�� ��ӹ޴� ����ü�� ���� �����ϱ� ���� �Լ�
	virtual void ChangeAnimEventDesc(ANIMEVENTDESC* _AnimEventDesc) PURE;
	// AnimEvent�� �ٸ� ����ü�� �ʿ��� ��� �ش� ����ü�� ����� �޴� �༮�鸸 ������ �� �ֵ��� �Ѵ�.
	virtual ANIMOTHEREVENTDESC* const OutOtherEventDesc() PURE;
	// AnimEvent�� �ٸ� ����ü�� �ʿ��� ��� �ش� ����ü�� ����� �޴� �༮�鸸 ���� �� �ֵ��� �Ѵ�. 
	virtual void ChangeOtherEventDesc(ANIMOTHEREVENTDESC* _AnimOtherEventDesc) PURE;

	virtual void SaveEvent(std::ofstream& _save) PURE;
	virtual void LoadEvent(std::ifstream& _load) PURE;
protected:
	// Event ��Ȳ�� ���� ����
	virtual void EventSituation(UAnimModel* _pAnimModel, const _double& _dTimeDelta) PURE;
private:
	virtual void Free() PURE;
private:
	ANIMEVENTTYPE				m_AnimEventType;
	ANIMEVENTCATEGORY	m_AnimEventCategory;
};
/* 
=====================================
AnimEvent
=====================================
AnimSectionEvent
=====================================
*/
/*
@ Date: 2024-02-08, Writer: ������
@ Explain
- �������� �̺�Ʈ�� �߻���Ű�� Ŭ����
*/
class UAnimSectionEvent abstract : public UAnimEvent {
protected:
	UAnimSectionEvent(ANIMEVENTTYPE _AnimEventType);
	UAnimSectionEvent(const ANIMEVENTSECTIONDESC& _AnimEventDesc, ANIMEVENTTYPE _AnimEventType);
	DESTRUCTOR(UAnimSectionEvent)
public:
	virtual _bool EventCheck(UAnimModel* _pAnimModel, const _double& _dTimeDelta, const _double& _dTimeAcc,
		const _wstring& _wstrInputTrigger) override;
	virtual ANIMEVENTDESC* const OutAnimEventDesc() override { return &m_AnimSectionDesc; }
	virtual void ChangeAnimEventDesc(ANIMEVENTDESC* _AnimEventDesc) override;
protected:
	// Event ��Ȳ�� ���� ����
	virtual void EventSituation(UAnimModel* _pAnimModel, const _double& _dTimeDelta) PURE;
	virtual void SaveEvent( std::ofstream& _save) PURE;
	virtual void LoadEvent( std::ifstream& _load) PURE;
private:
	virtual void Free() PURE;
private:
	ANIMEVENTSECTIONDESC			m_AnimSectionDesc;
};
/*
=====================================
AnimSectionEvent
=====================================
AnimOccurEvent
=====================================
*/
/*
@ Date: 2024-02-08, Writer: ������
@ Explain
- ������ ������ �̺�Ʈ�� �߻���Ű�� Ŭ����
*/
class UAnimOccurEvent abstract : public UAnimEvent {
protected:
	UAnimOccurEvent(ANIMEVENTTYPE _AnimEventType);
	UAnimOccurEvent(const ANIMOCURRESDESC& _AnimEventDesc,  ANIMEVENTTYPE _AnimEvent);
	DESTRUCTOR(UAnimOccurEvent)
public:
	virtual _bool EventCheck(UAnimModel* _pAnimModel, const _double& _dTimeDelta, const _double& _dTimeAcc,
		const _wstring& _wstrInputTrigger) override;
	virtual ANIMEVENTDESC* const OutAnimEventDesc() override { return &m_AnimOccurDesc; }
	virtual void ChangeAnimEventDesc(ANIMEVENTDESC* _AnimEventDesc) override;
protected:
	// Event ��Ȳ�� ���� ����
	virtual void EventSituation(UAnimModel* _pAnimModel, const _double& _dTimeDelta) PURE;
	virtual void SaveEvent(std::ofstream& _save) PURE;
	virtual void LoadEvent(std::ifstream& _load) PURE;
private:
	virtual void Free() PURE;
private:
	ANIMOCURRESDESC		m_AnimOccurDesc;
};

END