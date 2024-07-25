#include "EngineDefine.h"
#include "AnimOccursEvents.h"
#include "UMethod.h"
#include "UAnimModel.h"
#include "UGameInstance.h"
#include "USound.h"
#include "UCharacter.h"
#include "UPawn.h"
#include "UAnimModel.h"
#include "UAnimation.h"

UAnimOccursTimePassEvent::UAnimOccursTimePassEvent() : 
	UAnimOccurEvent(ANIMEVENTTYPE::ANIMEVENT_ANIMOCCURSTIMEPASS)
{
}

UAnimOccursTimePassEvent::UAnimOccursTimePassEvent(const UAnimOccursTimePassEvent& _rhs) :
	UAnimOccurEvent(_rhs), m_AnimChangeDesc{_rhs.m_AnimChangeDesc}
{
}

UAnimOccursTimePassEvent::UAnimOccursTimePassEvent(CSHPTRREF<UAnimModel> _spAnimModel, std::ifstream& _load) :
	UAnimOccurEvent(ANIMEVENTTYPE::ANIMEVENT_ANIMOCCURSTIMEPASS)
{
	LoadEvent(_spAnimModel, _load);
}

SHPTR<UAnimEvent> UAnimOccursTimePassEvent::Clone(UAnimModel* _pAnimModel)
{
	return std::move(CloneThis<UAnimOccursTimePassEvent>(*this));
}

const ANIMOTHEREVENTDESC*  UAnimOccursTimePassEvent::OutOtherEventDesc()
{
	return &m_AnimChangeDesc;
}

_bool UAnimOccursTimePassEvent::EventCheck(UPawn* _pPawn, UAnimModel* _pAnimModel, const _double& _dTimeDelta, const _double& _dTimeAcc, const _wstring& _wstrInputTrigger)
{
	if (GetAnimOccursDesc().IsAnimOcurrs(_dTimeAcc))
	{
		EventSituation(_pPawn, _pAnimModel, _dTimeDelta, _dTimeAcc);
		return true;
	}
	return false;
}

void UAnimOccursTimePassEvent::EventSituation(UPawn* _pPawn, UAnimModel* _pAnimModel, const _double& _dTimeDelta, const _double& _dTimeAcc)
{
	// ���� EnableLastLerp�� ���� ���� �ʴٸ�
	if (false == m_AnimChangeDesc.isEnableLastSettingAnim)
	{
		// �ٲ�� �ϴ� ������ �ٲ۴�. 
		if (m_AnimChangeDesc.fAnimChangeTime <= _dTimeAcc)
		{
			_pAnimModel->SetSupplyLerpValue(m_AnimChangeDesc.fSupplyAnimValue);
			_pAnimModel->ChangeAnimation(m_AnimChangeDesc.iNextAnimIndex, m_AnimChangeDesc.dNextAnimTimeAcc);
		}
	}
	// ���� �ִٸ�
	else
	{
		// ����
		if (_pAnimModel->GetCurrentAnimation()->GetAnimationProgressRate() >= m_AnimChangeDesc.fLastProgressValue)
		{
			_pAnimModel->SetAnimation(m_AnimChangeDesc.iNextAnimIndex);
		}
	}
}

void UAnimOccursTimePassEvent::SaveEvent(std::ofstream& _save)
{
	__super::SaveEvent(_save);
	_save.write((_char*)&m_AnimChangeDesc, sizeof(ANIMCHANGEDESC));
}

void UAnimOccursTimePassEvent::LoadEvent(CSHPTRREF<UAnimModel> _spAnimModel, std::ifstream& _load)
{
	__super::LoadEvent(_spAnimModel, _load);
	_load.read((_char*)&m_AnimChangeDesc, sizeof(ANIMCHANGEDESC));
}

void UAnimOccursTimePassEvent::Free()
{
}

/*
=================================================
UAnimOccursTimePassEvent
=================================================
AnomSoundEvent
=================================================
*/