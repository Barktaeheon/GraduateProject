#include "ClientDefines.h"
#include "CLogoScene.h"
#include "UGameInstance.h"
#include "CMainScene.h"
#include "CImageUI.h"

BEGIN(Client)

CLogoScene::CLogoScene(CSHPTRREF<UDevice> _spDevice) : 
	UScene(_spDevice, SCENE_LOGO), m_spImageUI{nullptr}
{
}

void CLogoScene::Free()
{
}

HRESULT CLogoScene::LoadSceneData()
{
	SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);
	CImageUI::UIDESC tDesc;
	{
		tDesc.fZBufferOrder = 0.1f;
		tDesc.strImgName = L"Background";
		tDesc.v2Size.x = static_cast<_float>(500);
		tDesc.v2Size.y = static_cast<_float>(500);
		tDesc.v2Pos = _float2{ (_float)(WINDOW_WIDTH), (_float)(WINDOW_HEIGHT) } / 2.f;
		m_spImageUI = std::static_pointer_cast<CImageUI>(spGameInstance->CloneActorAdd(PROTO_ACTOR_IMAGEUI, { &tDesc }));
	}
	{
		// ZBufferOrder�� �̹��� Order ������ ǥ���Ѵ�. 0�� �������� �� ��, 1�� �������� �� �ڿ� �ִ´�. (0, 1)�� ��� X
		tDesc.fZBufferOrder = 0.2f;
		tDesc.strImgName = L"Icon_Oni";
		tDesc.v2Size.x = static_cast<_float>(400);
		tDesc.v2Size.y = static_cast<_float>(400);
		tDesc.v2Pos = _float2{ (_float)(WINDOW_WIDTH), (_float)(WINDOW_HEIGHT) } / 2.f;
		tDesc.v2Pos.x += 200.f;
		m_spOtherImageUI = std::static_pointer_cast<CImageUI>(spGameInstance->CloneActorAdd(PROTO_ACTOR_IMAGEUI, { &tDesc }));
	}
	return S_OK;
}

void CLogoScene::Tick(const _double& _dTimeDelta)
{
	if (m_spImageUI->IsMouseOnRect())
	{
		// Ratio�� �����ϴ� �Լ�
		m_spOtherImageUI->UpdateRatio(0.05f);
		SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);
	//	if (true == spGameInstance->IsNetworkResourceRecvSuccess())

		//if(spGameInstance->IsP)
		{
		//	spGameInstance->RegisterScene(CreateConstructorNative<CMainScene>(GetDevice()));
		}
	}
	else
	{
		m_spOtherImageUI->UpdateRatio(0.2f);
	}
	// �������� �Ѿ���� �Ʒ� �ڵ� ����
}

void CLogoScene::LateTick(const _double& _dTimeDelta)
{

	// ��ġ ����
	//m_spOtherImageUI->UpdatePos(_float2{});

	// ������ ���� ����
	// m_spOtherImageUI->UpdateSize(_float2{});
}

END