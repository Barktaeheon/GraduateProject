#include "EngineDefine.h"
#include "UPlayer.h"
#include "UMethod.h"
#include "UTransform.h"
#include "UCamera.h"
#include "UStageManager.h"
#include "UStage.h"
#include "URegion.h"
#include "UNavigation.h"

UPlayer::UPlayer(CSHPTRREF<UDevice> _spDevice, const _wstring& _wstrLayer, const CLONETYPE& _eCloneType)
	: UCharacter(_spDevice, _wstrLayer, _eCloneType)
{
}

UPlayer::UPlayer(const UPlayer& _rhs) : 
	UCharacter(_rhs)
{
}

void UPlayer::Free()
{
}

HRESULT UPlayer::NativeConstruct()
{
	return __super::NativeConstruct();
}

HRESULT UPlayer::NativeConstructClone(const VOIDDATAS& _Datas)
{
	RETURN_CHECK_FAILED(__super::NativeConstructClone(_Datas), E_FAIL);
	// VOIDDATAS�� CharacterDesc 0��, PlayerDesc 1���� ���� ä�����Ѵ�. 
	assert(_Datas.size() > 1);

	PLAYERDESC PlayerDesc = UMethod::ConvertTemplate_Index<PLAYERDESC>(_Datas, PLAYERDESCORDER);
	assert(nullptr != PlayerDesc.spFollowCamera);

	m_spFollowCamera = PlayerDesc.spFollowCamera;
	assert(nullptr != m_spFollowCamera);

	m_wpCurRegion = PlayerDesc.spStageManager->GetStage()->GetRegion(0);
	assert(nullptr != m_wpCurRegion.lock());

	SHPTR<URegion> spCurRegion = m_wpCurRegion.lock();
	SHPTR<UNavigation> spNavigation = spCurRegion->GetNavigation();

	SHPTR<UCell> spCell = spNavigation->FindCell(GetTransform()->GetPos());
	GetTransform()->SetPos(spCell->GetCenterPos());
	return S_OK;
}

void UPlayer::TickActive(const _double& _dTimeDelta)
{
	__super::TickActive(_dTimeDelta);
}

void UPlayer::LateTickActive(const _double& _dTimeDelta)
{
	__super::LateTickActive(_dTimeDelta);
	// Region 
	{
		SHPTR<URegion> spCurRegion = m_wpCurRegion.lock();
		SHPTR<UNavigation> spNavigation = spCurRegion->GetNavigation();

		SHPTR<UCell> spCell{ nullptr };
		if (false == spNavigation->IsMove(GetTransform()->GetPos(), REF_OUT spCell))
		{
			GetTransform()->SetPos(GetPrevPos());
		}
	}
	// Camera 
	{
		_float3 vPosition = DirectX::XMVector3Transform(_float3(0.f, 10.f, -10.f), GetTransform()->GetWorldMatrix());
		m_spFollowCamera->GetTransform()->SetPos(vPosition);
		m_spFollowCamera->GetTransform()->LookAt(GetTransform()->GetPos());
	}
}

HRESULT UPlayer::RenderActive(CSHPTRREF<UCommand> _spCommand, CSHPTRREF<UTableDescriptor> _spTableDescriptor)
{
	return __super::RenderActive(_spCommand, _spTableDescriptor);
}

HRESULT UPlayer::RenderShadowActive(CSHPTRREF<UCommand> _spCommand, CSHPTRREF<UTableDescriptor> _spTableDescriptor)
{
	return S_OK;
}

void UPlayer::Collision(CSHPTRREF<UPawn> _pEnemy)
{
}

