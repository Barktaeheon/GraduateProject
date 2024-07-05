#ifndef _SERVERFRAMEWORK_CORE_PUBLIC_AANIMEVENT_H
#define _SERVERFRAMEWORK_CORE_PUBLIC_AANIMEVENT_H

#include "ACoreBase.h"

BEGIN(Core)
class AAnimator;
class APawn;
/*
@ Date: 2024-02-10, Writer: ������
@ Explain
- Animation Event�� ������ �θ� Ŭ����
*/
class AAnimEvent abstract : public ACoreBase {
public:
	AAnimEvent(ANIMEVENTTYPE _AnimEventType, ANIMEVENTCATEGORY _AnimEventCategory);
	AAnimEvent(const AAnimEvent& _rhs);
	DESTRUCTOR(AAnimEvent)
public:
	virtual SHPTR<AAnimEvent> Clone(AAnimator* _pAnimator = nullptr) PURE;
public:
	virtual _bool EventCheck(APawn* _pPawn, AAnimator* _pAnimator,
		const _double& _dTimeDelta, const _double& _dTimeAcc,	const _string& _strInputTrigger) PURE;
	// AnimEventDesc�� ��ӹ޴� ����ü�� �����ϴ� �Լ�
	virtual const  ANIMEVENTDESC* OutAnimEventDesc() PURE;
	// AnimEvent�� �ٸ� ����ü�� �ʿ��� ��� �ش� ����ü�� ����� �޴� �༮�鸸 ������ �� �ֵ��� �Ѵ�.
	virtual const ANIMOTHEREVENTDESC* OutOtherEventDesc() PURE;
protected:
	// Event ��Ȳ�� ���� ����
	virtual void EventSituation(APawn* _pPawn, AAnimator* _pAnimator,
		const _double& _dTimeDelta, const _double& _dTimeAcc) PURE;
	virtual void LoadEvent(AAnimator* _pAnimator, std::ifstream& _load) PURE;
private:
	virtual void Free() PURE;
private:
	ANIMEVENTTYPE					m_AnimEventType;
	ANIMEVENTCATEGORY		m_AnimEventCategory;
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
class AAnimSectionEvent abstract : public AAnimEvent {
public:
	AAnimSectionEvent(ANIMEVENTTYPE _AnimEventType);
	AAnimSectionEvent(const AAnimSectionEvent& _rhs);
	AAnimSectionEvent(const ANIMEVENTSECTIONDESC& _AnimEventDesc, ANIMEVENTTYPE _AnimEventType);
	DESTRUCTOR(AAnimSectionEvent)
public:
	virtual SHPTR<AAnimEvent> Clone(AAnimator* _pAnimator = nullptr) PURE;
public:
	virtual _bool EventCheck(APawn* _pPawn, AAnimator* _pAnimator,
		const _double& _dTimeDelta, const _double& _dTimeAcc, const _string& _strInputTrigger) PURE;
public: /* get set*/
	virtual const ANIMEVENTDESC* OutAnimEventDesc() override { return &m_AnimSectionDesc; }
protected:
	// Event ��Ȳ�� ���� ����
	virtual void EventSituation(APawn* _pPawn, AAnimator* _pAnimator,
		const _double& _dTimeDelta, const _double& _dTimeAcc) PURE;
	virtual void LoadEvent(AAnimator* _pAnimator, std::ifstream& _load) PURE;
	void ResetEventData(AAnimator* _pAnimator);
protected: /* get set*/
	const ANIMEVENTSECTIONDESC& GetAnimSectionDesc() const { return m_AnimSectionDesc; }
private:
	virtual void Free() PURE;
private:
	ANIMEVENTSECTIONDESC			m_AnimSectionDesc;
};

/*
=====================================
AnimSectionEvent
=====================================
AnimChangeBetweenEvent
=====================================
*/

/*
@ Date: 2024-02-17, Writer: ������
@ Explain
- ���� ���̿��� Ư�� �̺�Ʈ�� �߻��� �� ���� �ִϸ��̼����� �Ѿ�� ����� �̺�Ʈ�̴�.
*/
class AAnimChangeBetweenEvent final : public AAnimSectionEvent {
public:
	AAnimChangeBetweenEvent();
	AAnimChangeBetweenEvent(const AAnimChangeBetweenEvent& _rhs);
	AAnimChangeBetweenEvent(AAnimator* _pAnimator, std::ifstream& _load);
	DESTRUCTOR(AAnimChangeBetweenEvent)
public:
	virtual SHPTR<AAnimEvent> Clone(AAnimator* _pAnimator = nullptr) override;
public:
	virtual _bool EventCheck(APawn* _pPawn, AAnimator* _pAnimator,
		const _double& _dTimeDelta, const _double& _dTimeAcc, const _string& _strInputTrigger) override;
	// UAnimSectionEvent��(��) ���� ��ӵ�
	virtual const ANIMOTHEREVENTDESC* OutOtherEventDesc() override;
protected:
	// Event ��Ȳ�� ���� ����
	virtual void EventSituation(APawn* _pPawn, AAnimator* _pAnimator,
		const _double& _dTimeDelta, const _double& _dTimeAcc) override;
	virtual void LoadEvent(AAnimator* _pAnimator, std::ifstream& _load) override;
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
class AAnimColliderEvent final : public AAnimSectionEvent {
public:
	AAnimColliderEvent();
	AAnimColliderEvent(const AAnimColliderEvent& _rhs);
	AAnimColliderEvent(AAnimator* _pAnimator, std::ifstream& _load);
	DESTRUCTOR(AAnimColliderEvent)
public:
	virtual SHPTR<AAnimEvent> Clone(AAnimator* _pAnimator = nullptr) override;
public:
	virtual _bool EventCheck(APawn* _pPawn, AAnimator* _pAnimator,
		const _double& _dTimeDelta, const _double& _dTimeAcc, const _string& _strInputTrigger) override;
	// UAnimSectionEvent��(��) ���� ��ӵ�
	virtual const ANIMOTHEREVENTDESC* OutOtherEventDesc() override;
protected:
	// Event ��Ȳ�� ���� ����
	virtual void EventSituation(APawn* _pPawn, AAnimator* _pAnimator,
		const _double& _dTimeDelta, const _double& _dTimeAcc) override;
	virtual void LoadEvent(AAnimator* _pAnimator, std::ifstream& _load) override;
private:
	virtual void Free() override;
private:
	ANIMCOLLIDERDESC	m_AnimColliderDesc;
};

END

#endif