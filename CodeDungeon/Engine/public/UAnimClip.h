#pragma once

#include "UBase.h"

BEGIN(Engine)
class UAnimation;
class UChannel;
class UBoneNode;
class UAnimEvent;
class UAnimModel;
class UAnimator;

using ANIMEVENTCONTAINER = UNORMAP<ANIMEVENTTYPE, VECTOR<SHPTR<UAnimEvent>>>;
/*
@ Date: 2024-02-13, Writer: ������
@ Explain
- �ִϸ��̼ǰ� �ִϸ��̼� �̺�Ʈ���� ��� AnimClip Ŭ�����̴�. �ش� Ŭ������ ���� �ִϸ��̼��� �̺�Ʈ�� �����Ѵ�. 
*/
class UAnimClip final : public UBase {
public:
	UAnimClip(SHPTR<UAnimation> _spAnimation);
	UAnimClip(CSHPTRREF<UAnimModel> _spAnimModel, std::ifstream& _Read);
	NO_COPY(UAnimClip)
	DESTRUCTOR(UAnimClip)
public:
	virtual void Free() override;
	void Tick(CSHPTRREF<UAnimator> _spAnimator, const _double& _dTimeDelta);
	void InsertEvent(ANIMEVENTTYPE _AnimEventType, CSHPTRREF<UAnimEvent> _spAnimEvent);
	void RemoveEvent(CSHPTRREF<UAnimEvent> _spAnimEvent);

	void Save(CSHPTRREF<UAnimator> _spAnimator, std::ofstream& _Save);
	void Load(CSHPTRREF<UAnimator> _spAnimator, std::ifstream& _Read);
private:
	ANIMEVENTCONTAINER		m_AnimEventContainer;
	SHPTR<UAnimation>			m_spAnimation;
};

END