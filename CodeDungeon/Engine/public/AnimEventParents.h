#pragma once

#include "UBase.h"

BEGIN(Engine)
class UAnimModel;
class UAnimator;
class UPawn;
/*
@ Date: 2024-02-10, Writer: ������
@ Explain
- Animation Event�� ������ �θ� Ŭ����
*/
class UAnimEvent abstract : public UBase {
protected:
	UAnimEvent(ANIMEVENTTYPE _AnimEventType, ANIMEVENTCATEGORY _AnimEventCategory);
	UAnimEvent(const UAnimEvent& _rhs);
	DESTRUCTOR(UAnimEvent)
public:
	const ANIMEVENTTYPE GetAnimEventType() const { return m_AnimEventType; }
	const ANIMEVENTCATEGORY GetAnimEventCategory() const { return m_AnimEventCategory; }
public:
	virtual SHPTR<UAnimEvent> Clone(UAnimModel* _pAnimModel = nullptr) PURE;
public:
	virtual _bool EventCheck(UPawn* _pPawn, UAnimModel* _pAnimModel, const _double& _dTimeDelta, const _double& _dTimeAcc,
		const _wstring& _wstrInputTrigger) PURE;
	// AnimEventDesc�� ��ӹ޴� ����ü�� �����ϴ� �Լ�
	virtual const  ANIMEVENTDESC*  OutAnimEventDesc() PURE;
	// AnimEvent�� �ٸ� ����ü�� �ʿ��� ��� �ش� ����ü�� ����� �޴� �༮�鸸 ������ �� �ֵ��� �Ѵ�.
	virtual const ANIMOTHEREVENTDESC*  OutOtherEventDesc() PURE;

	virtual void SaveEvent(std::ofstream& _save) PURE;
	virtual void LoadEvent(CSHPTRREF<UAnimModel> _spAnimModel, std::ifstream& _load) PURE;
protected:
	// Event ��Ȳ�� ���� ����
	virtual void EventSituation(UPawn* _pPawn, UAnimModel* _pAnimModel, const _double& _dTimeDelta, const _double& _dTimeAcc) PURE;
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
	UAnimSectionEvent(const UAnimSectionEvent& _rhs);
	UAnimSectionEvent(const ANIMEVENTSECTIONDESC& _AnimEventDesc, ANIMEVENTTYPE _AnimEventType);
	DESTRUCTOR(UAnimSectionEvent)
public:
	virtual SHPTR<UAnimEvent> Clone(UAnimModel* _pAnimModel = nullptr) PURE;
public:
	virtual _bool EventCheck(UPawn* _pPawn, UAnimModel* _pAnimModel, const _double& _dTimeDelta, const _double& _dTimeAcc,
		const _wstring& _wstrInputTrigger) override;
	virtual const ANIMEVENTDESC*  OutAnimEventDesc() override { return &m_AnimSectionDesc; }
protected: /* get set*/
	const ANIMEVENTSECTIONDESC& GetAnimSectionDesc() const { return m_AnimSectionDesc; }
protected:
	// Event ��Ȳ�� ���� ����
	virtual void EventSituation(UPawn* _pPawn, UAnimModel* _pAnimModel, const _double& _dTimeDelta, const _double& _dTimeAcc) PURE;
	virtual void SaveEvent( std::ofstream& _save) PURE;
	virtual void LoadEvent(CSHPTRREF<UAnimModel> _spAnimModel, std::ifstream& _load) PURE;
protected:
	void ResetEventData(UAnimModel* _pAnimModel);
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
	UAnimOccurEvent(const UAnimOccurEvent& _rhs);
	UAnimOccurEvent(const ANIMOCURRESDESC& _AnimEventDesc,  ANIMEVENTTYPE _AnimEvent);
	DESTRUCTOR(UAnimOccurEvent)
public:
	virtual SHPTR<UAnimEvent> Clone(UAnimModel* _pAnimModel = nullptr) PURE;
	virtual _bool EventCheck(UPawn* _pPawn, UAnimModel* _pAnimModel, const _double& _dTimeDelta, const _double& _dTimeAcc,
		const _wstring& _wstrInputTrigger) override;
	virtual const ANIMEVENTDESC*  OutAnimEventDesc() override { return &m_AnimOccurDesc; }
protected: /* get set*/
	const ANIMOCURRESDESC& GetAnimOccursDesc() const { return m_AnimOccurDesc; }
protected:
	// Event ��Ȳ�� ���� ����
	virtual void EventSituation(UPawn* _pPawn, UAnimModel* _pAnimModel, const _double& _dTimeDelta, const _double& _dTimeAcc) PURE;
	virtual void SaveEvent(std::ofstream& _save) PURE;
	virtual void LoadEvent(CSHPTRREF<UAnimModel> _spAnimModel, std::ifstream& _load) PURE;
private:
	virtual void Free() PURE;
private:
	ANIMOCURRESDESC		m_AnimOccurDesc;
};

END