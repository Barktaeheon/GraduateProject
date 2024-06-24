#include "EngineDefine.h"
#include "UPlayer.h"
#include "UMethod.h"
#include "UTransform.h"
#include "UCamera.h"
#include "UAnimationController.h"
#include "UCell.h"
#include "UNavigation.h"

UPlayer::UPlayer(CSHPTRREF<UDevice> _spDevice, const _wstring& _wstrLayer, const CLONETYPE& _eCloneType)
	: UCharacter(_spDevice, _wstrLayer, _eCloneType),
	m_spFollowCamera{nullptr},
	m_bisJumping{false},
	m_bisFalling{false}
{
}

UPlayer::UPlayer(const UPlayer& _rhs) : 
	UCharacter(_rhs),
	m_spFollowCamera{ nullptr },
	m_bisJumping{ false },
	m_bisFalling{ false }
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

	if (_Datas.size() >= 2)
	{
		PLAYERDESC PlayerDesc = UMethod::ConvertTemplate_Index<PLAYERDESC>(_Datas, PLAYERDESCORDER);
		m_spFollowCamera = PlayerDesc.spFollowCamera;
	}

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
		SHPTR<UNavigation> spNavigation = GetCurrentNavi();
		_float3 vPosition{ GetTransform()->GetPos() };
		SHPTR<UCell> PrevCell = spNavigation->GetCurCell();
		SHPTR<UCell> newCell{};
		_double speed = _dTimeDelta * 3.f;

		//����ó��. �������� �����ϸ鼭 ������ �� �Ʒ��� ���������� �� ���°� ���� true�� �Ǹ鼭 ������.
		//�̸� �����ϱ� ���� �� �� true�� �� falling�� false�� �����ν� ������ ���� �ϵ��� ó��. 
		if (m_bisJumping && m_bisFalling)
		{
			m_bisFalling = false;
		}

		//�⺻ �߷� ����. �߷��� Ȱ��ȭ�Ǿ߸� ����
		//�����Ͽ� �ö󰡰� ���� �� ��Ȱ��ȭ
		if(!m_bisJumping && m_bisFalling)
		{
			GetTransform()->GravityFall(speed);
			vPosition = GetTransform()->GetPos();
		}

		//������ �ϰ� �ִٸ� ���� ������ ����
		if(!m_bisFalling && m_bisJumping)
		{
			GetTransform()->JumpMovement(speed);
			vPosition = GetTransform()->GetPos();
		}
		if (GetTransform()->GetJumpVelocity().y < 0.f) //�ִ� ���̱��� �����ߴٸ� 
		{
			//����(���) ����, ���� ����
			m_bisJumping = false;
			m_bisFalling = true;
			GetTransform()->DisableJump();
		}


		//���� ���� �ٴٶ��� ��
		if (false == spNavigation->IsMove(vPosition, REF_OUT newCell))
		{
			//�������� ���� �ƴ� ���
			if (!spNavigation->GetCurCell()->GetJumpableState())
			{
				// �����̷��� ��ġ�� �� �ۿ� �ִ� ��� ���� ���� ���� ����� �� ���� ������ ����
				// �����̵� ������ ȿ���� �� �� ����
				_float3 closestPoint = spNavigation->ClampPositionToCell(vPosition);
				GetTransform()->SetPos(_float3(closestPoint.x, vPosition.y, closestPoint.z));
				vPosition = GetTransform()->GetPos();
			}
			//�������� ���� ���
			else
			{
				//�������� ������ ������ ���� ��, �߷� Ȱ��ȭ(����)
				//���߿��� �� �Ʒ��� ���� ã��, ���� ���� ������
				newCell = spNavigation->FindCell(vPosition);
				m_bisFalling = true;			
			}
		}

		if(m_bisFalling && spNavigation->GetCurCell() != nullptr)
		{
			//���� ������ �Ʒ��� ��������
			_float curCellCenterY = spNavigation->GetCurCell()->GetHeightAtXZ(vPosition.x, vPosition.z);
			if (vPosition.y < curCellCenterY)
			{
				//���� ����vPosition
				m_bisFalling = false;
				GetTransform()->DisableGravity();
				GetTransform()->SetPos(_float3(vPosition.x, curCellCenterY, vPosition.z));
				vPosition = GetTransform()->GetPos();
			}
		}

		//�������� ���� ���� ��, �׸��� �����Ͽ� �ö󰡰� ���� ���� �� �� ���� ���̸� Ž
		if (!m_bisFalling && !m_bisJumping)
		{
			spNavigation->ComputeHeight(GetTransform());
			if (std::abs(GetPrevPos().y - GetTransform()->GetPos().y) > 30.f)
			{
				if (!PrevCell->GetJumpableState())
				{
					GetTransform()->SetPos(GetPrevPos());
					spNavigation->SetCurCell(PrevCell);
				}
			}
		}
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

void UPlayer::Collision(CSHPTRREF<UPawn> _pEnemy, const _double& _dTimeDelta)
{
}

void UPlayer::FollowCameraMove(const _float3& _vPlayerToDistancePosition, const _double& _TimeElapsed)
{
	RETURN_CHECK(nullptr == m_spFollowCamera, ;);
    // ī�޶��� ��ǥ ��ġ�� ���.
    _float3 vTargetPosition = DirectX::XMVector3Transform(_vPlayerToDistancePosition, GetTransform()->GetWorldMatrix());

    // ī�޶��� ���� ��ġ.
    _float3 vCurrentPosition = m_spFollowCamera->GetTransform()->GetPos();

    // ī�޶��� �̵� ������ ���.
    _float3 vDirection = vTargetPosition - vCurrentPosition;
    _float3 f3Distance = DirectX::XMVector3Length(vDirection);
    _float fDistance = f3Distance.x;
    _float fTimeLag = 0.05f;

    vDirection.Normalize();
    // �����̸� ������ �Ÿ��� ���.
	_float fTimeLagScale = (fTimeLag) ? static_cast<_float>(_TimeElapsed) * (1.0f / fTimeLag) : 1.0f;
	_float fAdjustedDistance = fDistance * fTimeLagScale;

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

