#pragma once

#include "UBase.h"

BEGIN(Engine)
class UAnimModel;
class UAnimator;
/*
@ Date: 2024-02-08, Writer: ������
@ Explain
- Animation Event�� ������ �θ� Ŭ����
*/
class UAnimEvent abstract : public UBase {
public:
	UAnimEvent();
	UAnimEvent(const _int _EventID, ANIMEVENT _AnimEvent);
	DESTRUCTOR(UAnimEvent)
public:
	const _int GetEventID() const { return m_EventID; }
	const ANIMEVENT GetAnimEventType() const { return m_AnimEventType; }
public:
	virtual HRESULT NativeConstruct() PURE;
	virtual _bool EventCheck(CSHPTRREF<UAnimator> _spAnimator, const _double& _dTimeDelta, const _double& _dTimeAcc) PURE;

	void Save(const _wstring& _wstrPath);
	void Load(const _wstring& _wstrPath);
protected:
	// Event ��Ȳ�� ���� ����
	virtual void EventSituation(CSHPTRREF<UAnimator> _spAnimator, const _double& _dTimeDelta) PURE;
	virtual void SaveEvent( std::ofstream& _save) PURE;
	virtual void LoadEvent( std::ifstream& _load) PURE;
private:
	virtual void Free() PURE;
private:
	_int										m_EventID;
	ANIMEVENT						m_AnimEventType;
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
public:
	UAnimSectionEvent();
	UAnimSectionEvent(const ANIMEVENTSECTIONDESC& _AnimEventDesc, const _int _EventID, ANIMEVENT _AnimEvent);
	DESTRUCTOR(UAnimSectionEvent)
public:
	virtual HRESULT NativeConstruct() PURE;
	virtual _bool EventCheck(CSHPTRREF<UAnimator> _spAnimator, const _double& _dTimeDelta, const _double& _dTimeAcc) override;

protected:
	// Event ��Ȳ�� ���� ����
	virtual void EventSituation(CSHPTRREF<UAnimator> _spAnimator, const _double& _dTimeDelta) PURE;
	virtual void SaveEvent( std::ofstream& _save) PURE;
	virtual void LoadEvent( std::ifstream& _load) PURE;
private:
	virtual void Free() PURE;
private:
	ANIMEVENTSECTIONDESC			m_AnimEventDesc;
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
public:
	UAnimOccurEvent();
	UAnimOccurEvent(const ANIMOCURRESDESC& _AnimEventDesc, const _int _EventID, ANIMEVENT _AnimEvent);
	DESTRUCTOR(UAnimOccurEvent)
public:
	virtual HRESULT NativeConstruct() PURE;
	virtual _bool EventCheck(CSHPTRREF<UAnimator> _spAnimator, const _double& _dTimeDelta, const _double& _dTimeAcc) override;
protected:
	// Event ��Ȳ�� ���� ����
	virtual void EventSituation(CSHPTRREF<UAnimator> _spAnimator, const _double& _dTimeDelta) PURE;
	virtual void SaveEvent(std::ofstream& _save) PURE;
	virtual void LoadEvent(std::ifstream& _load) PURE;
private:
	virtual void Free() PURE;
private:
	ANIMOCURRESDESC		m_AnimOccurDesc;
};

END