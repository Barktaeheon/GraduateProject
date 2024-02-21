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
	UAnimEvent(ANIMEVENTCATEGORY _AnimEventCategory);
	DESTRUCTOR(UAnimEvent)
public:
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
	ANIMEVENTCATEGORY	m_AnimEventCategory;
};

END