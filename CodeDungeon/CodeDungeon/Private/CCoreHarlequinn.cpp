#include "ClientDefines.h"
#include "UGameInstance.h"
#include "UCollider.h"
#include "UTransform.h"
#include "UMethod.h"
#include "CCoreHarlequinn.h"
#include "UPawn.h"
#include "UParticle.h"
#include "UParticleSystem.h"
#include "UModel.h"

CCoreHarlequinn::CCoreHarlequinn(CSHPTRREF<UDevice> _spDevice, const _wstring& _wstrLayer, const CLONETYPE& _eCloneType)
	: CModelObjects(_spDevice, _wstrLayer, _eCloneType)
{
}

CCoreHarlequinn::CCoreHarlequinn(const CCoreHarlequinn& _rhs)
	: CModelObjects(_rhs)
{
}

void CCoreHarlequinn::Free()
{
}

HRESULT CCoreHarlequinn::NativeConstruct()
{
	return __super::NativeConstruct();
}

HRESULT CCoreHarlequinn::NativeConstructClone(const VOIDDATAS& _vecDatas)
{
	RETURN_CHECK_FAILED(__super::NativeConstructClone(_vecDatas), E_FAIL);
	SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);

	SetModel(L"Proto_Res_Model_HarlequinnHead_FBX.bin");

	COREHARLEQUINNDESC tBarsDesc = UMethod::ConvertTemplate_Index<COREHARLEQUINNDESC>(_vecDatas, 0);
	GetTransform()->SetNewWorldMtx(tBarsDesc._Worldm);
	SetPawnType(PAWNTYPE::PAWN_STATICOBJ);
	SetActive(false);
	return S_OK;
}

void CCoreHarlequinn::TickActive(const _double& _dTimeDelta)
{
	__super::TickActive(_dTimeDelta);
}


void CCoreHarlequinn::LateTickActive(const _double& _dTimeDelta)
{
	__super::LateTickActive(_dTimeDelta);
}

HRESULT CCoreHarlequinn::RenderActive(CSHPTRREF<UCommand> _spCommand, CSHPTRREF<UTableDescriptor> _spTableDescriptor)
{
	__super::RenderActive(_spCommand, _spTableDescriptor);
	return S_OK;
}

HRESULT CCoreHarlequinn::RenderShadowActive(CSHPTRREF<UCommand> _spCommand, CSHPTRREF<UTableDescriptor> _spTableDescriptor)
{
	__super::RenderShadowActive(_spCommand, _spTableDescriptor);
	return S_OK;
}
HRESULT CCoreHarlequinn::RenderOutlineActive(CSHPTRREF<UCommand> _spCommand, CSHPTRREF<UTableDescriptor> _spTableDescriptor, _bool _pass)
{
	__super::RenderOutlineActive(_spCommand, _spTableDescriptor,_pass);
	return S_OK;
}

void CCoreHarlequinn::Collision(CSHPTRREF<UPawn> _pEnemy, const _double& _dTimeDelta)
{
}

