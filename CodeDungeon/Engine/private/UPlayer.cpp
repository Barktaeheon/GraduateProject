#include "EngineDefine.h"
#include "UPlayer.h"
#include "UMethod.h"
#include "UTransform.h"
#include "UCamera.h"
#include "UAnimationController.h"

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
	return S_OK;
}

void UPlayer::TickActive(const _double& _dTimeDelta)
{
	__super::TickActive(_dTimeDelta);
}

void UPlayer::LateTickActive(const _double& _dTimeDelta)
{
	__super::LateTickActive(_dTimeDelta);
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

void UPlayer::FollowCameraMove(const _float3& _vPlayerToDistancePosition, const _double& _TimeElapsed)
{
    // ī�޶��� ��ǥ ��ġ�� ���.
    _float3 vTargetPosition = DirectX::XMVector3Transform(_vPlayerToDistancePosition, GetTransform()->GetWorldMatrix());

    // ī�޶��� ���� ��ġ.
    _float3 vCurrentPosition = m_spFollowCamera->GetTransform()->GetPos();

    // ī�޶��� �̵� ������ ���.
    _float3 vDirection = vTargetPosition - vCurrentPosition;
    _float3 f3Distance = DirectX::XMVector3Length(vDirection);
    _float fDistance = f3Distance.x;
    _float fTimeLag = 0.15f;

    vDirection.Normalize();
    // �����̸� ������ �Ÿ��� ���.
    float fTimeLagScale = (fTimeLag) ? _TimeElapsed * (1.0f / fTimeLag) : 1.0f;
    float fAdjustedDistance = fDistance * fTimeLagScale;

    // ī�޶� �̵� �Ÿ��� ����.
    if (fAdjustedDistance > fDistance) fAdjustedDistance = fDistance;
    if (fDistance < 0.01f) fAdjustedDistance = fDistance;

    // ����� �̵� �Ÿ��� ���� ī�޶� �̵�.
    if (fAdjustedDistance > 0)
    {
        _float3 vNewPosition = vCurrentPosition + vDirection * fAdjustedDistance;;
        m_spFollowCamera->GetTransform()->SetPos(vNewPosition);
    }
    m_spFollowCamera->GetTransform()->LookAt(_float3(GetTransform()->GetPos().x, GetTransform()->GetPos().y + 7.f, GetTransform()->GetPos().z));
}

