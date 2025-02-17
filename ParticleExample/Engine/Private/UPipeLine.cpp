#include "EngineDefines.h"
#include "UPipeLine.h"
#include "UGameInstance.h"
#include "UTransform.h"
#include "UCamera.h"
#include "UGrobalConstantBuffer.h"
#include "UGraphicDevice.h"
#include "UFrustomCulling.h"

UPipeLine::UPipeLine() : 
m_arrCameras{},
m_spMainCamera{nullptr},
m_spViewProjGrobalBuffer{nullptr},
m_stViewProjParam{}
{
}

void UPipeLine::Free()
{
	for (auto& iter : m_arrCameras)
	{
		iter.reset();
	}
}

HRESULT UPipeLine::ReadyPipeLine(UGameInstance* _pGameInstance)
{
	_pGameInstance->GetGrobalConstantBuffer(GROBAL_VIEWPROJ, m_spViewProjGrobalBuffer);
	return S_OK;
}

void UPipeLine::RegisterCameraInPipeline(CSHPTRREF<UCamera> _spCamera, CAMID& _iCamID,
	const VIEWPROJMATRIX& _stViewProjMatrix, const CAMERATYPE _eCamType)
{
	RETURN_CHECK(nullptr == _spCamera, ;);
	_uint ID = GetID(_eCamType);
	RETURN_CHECK(m_arrCameras.size() <= ID, ;);

	if(MAIN_CAMERA_ID == ID || DEFFERED_CAMERA_ID == ID || SHADOWLIGHT_CAMERA_ID == ID)
	{
		// Main 카메라가 존재하면 바꾼다. 
		if (nullptr != m_arrCameras[MAIN_CAMERA_ID])
		{
			_uint OtherCamID = GetID(CAMERATYPE::OTEHR);
			std::swap(m_arrCameras[OtherCamID], m_arrCameras[ID]);
		}
		if (MAIN_CAMERA_ID == ID)
			m_spMainCamera = _spCamera;
	}
	_iCamID = ID;
	m_arrCameras[ID] = _spCamera;

	::memcpy(&m_stViewProjParam.arrViewProjParams[ID], &_stViewProjMatrix, sizeof(VIEWPROJMATRIX));

	m_arrFrustoms[ID] = CreateNative<UFrustomCulling>(m_arrCameras[ID]);
}

void UPipeLine::FrustomTick()
{
	for (_uint i = 0; i < m_arrFrustoms.size(); ++i)
	{
		if (nullptr != m_arrFrustoms[i])
		{
			if (true == m_arrCameras[i]->IsFrustomOn())
			{
				m_arrFrustoms[i]->Tick();
			}
		}
	}
}

_bool UPipeLine::IsFrustomContains(const _float3& _vPos, const _float _fRadius, const CAMID& _iCamID)
{
	RETURN_CHECK(nullptr == m_arrFrustoms[_iCamID], false);
	return m_arrFrustoms[_iCamID]->IsContains(_vPos, _fRadius);
}

void UPipeLine::CancelCameraInPipeLine(const CAMID& _iCamID)
{
	RETURN_CHECK(m_arrCameras.size() <= _iCamID, ;);
	m_arrCameras[_iCamID] = nullptr;
 	ArrangeToCameraArr();
}

void UPipeLine::ChangeMainCamera(const _uint& _iCamID)
{
	RETURN_CHECK(_iCamID >= MAX_CAM_SIZE, ;);

	RETURN_CHECK(nullptr == m_arrCameras[_iCamID], ;);

	_uint iCamID = m_spMainCamera->GetCamID();
	_bool bFrustom = m_spMainCamera->IsFrustomOn();
	m_spMainCamera->SetCamID(m_arrCameras[_iCamID]->GetCamID());
	m_arrCameras[_iCamID]->SetCamID(iCamID);

	m_spMainCamera->SetFrunstomOn(m_arrCameras[_iCamID]->IsFrustomOn());
	m_arrCameras[_iCamID]->SetFrunstomOn(bFrustom);

	std::swap(m_arrCameras[MAIN_CAMERA_ID], m_arrCameras[_iCamID]);
	std::swap(m_arrFrustoms[MAIN_CAMERA_ID], m_arrFrustoms[_iCamID]);
	m_spMainCamera = m_arrCameras[_iCamID];
}

void UPipeLine::UpdateViewProjMatrix()
{
	// 글로벌 버퍼인 ViewProj에 값을 모아서 전달한다. 
	for (_uint i = 0; i < m_arrCameras.size(); ++i)
	{
		if (nullptr != m_arrCameras[i])
		{
			m_arrCameras[i]->BindMatrix(m_stViewProjParam.arrViewProjParams[i]);
		} 
	}
}

void UPipeLine::BindViewProjMatrix(CSHPTRREF<UCommand> _spCommand)
{
	m_spViewProjGrobalBuffer->SettingGrobalData(_spCommand, &m_stViewProjParam, VIEWPARAM_SIZE);
}

const _float4x4 UPipeLine::GetMainCamViewMatrix() const
{
	RETURN_CHECK(nullptr == m_spMainCamera, _float4x4::Identity);
	return m_spMainCamera->GetTransform()->GetWorldMatrixInv();
}

const _float4x4& UPipeLine::GetMainCamProjMatrix() const
{
	RETURN_CHECK(nullptr == m_spMainCamera, _float4x4::Identity);
	return m_spMainCamera->GetProjMatrix();
}

const _float3& UPipeLine::GetMainCamPosition()
{
	RETURN_CHECK(nullptr == m_spMainCamera, _float3::Zero);
	return m_spMainCamera->GetTransform()->GetPos();
}

const _float4x4 UPipeLine::GetCamViewMatrix(const CAMID& _iID) const
{
	RETURN_CHECK(m_arrCameras.size() <= _iID, _float4x4::Identity);
	RETURN_CHECK(nullptr == m_arrCameras[_iID], _float4x4::Identity);
	return m_arrCameras[_iID]->GetTransform()->GetWorldMatrixInv();
}

const _float4x4& UPipeLine::GetCamProjMatrix(const CAMID& _iID) const
{
	RETURN_CHECK(m_arrCameras.size() <= _iID, _float4x4::Identity);
	RETURN_CHECK(nullptr == m_arrCameras[_iID], _float4x4::Identity);
	return m_arrCameras[_iID]->GetProjMatrix();
}

const _float3& UPipeLine::GetCameraPosition(const CAMID& _iID)
{
	RETURN_CHECK(m_arrCameras.size() <= _iID, _float3::Zero);
	RETURN_CHECK(nullptr == m_arrCameras[_iID], _float3::Zero);
	return m_arrCameras[_iID]->GetTransform()->GetPos();
}

const _float UPipeLine::GetCamFar(const CAMID& _iID)
{
	RETURN_CHECK(m_arrCameras.size() <= _iID, 0.f);
	RETURN_CHECK(nullptr == m_arrCameras[_iID], 0.f);
	return m_arrCameras[_iID]->GetCamFar();
}

void UPipeLine::ClearOneTypeCamera()
{
	// Find Once Clone Type
	_ubyte bCloneType = static_cast<_ubyte>(CLONETYPE::CLONE_ONCE);
	for (auto& iter : m_arrCameras)
	{
		if (nullptr != iter)
		{
			if (iter->GetCloneData()->bCloneType == bCloneType)
			{
				iter = nullptr;
			}
		}
	}
//	ArrangeToCameraArr();
}

void UPipeLine::ArrangeToCameraArr()
{
	// Set New Array
	CAMERAARR arrNewCamera;
	for (_uint i = 0; i < arrNewCamera.size(); ++i)
	{
		for (auto& iter : m_arrCameras)
		{
			if (nullptr != iter)
			{
				arrNewCamera[i] = iter;
				arrNewCamera[i]->SetCamID(i);
				iter = nullptr;
				break;
			}
		}
	}
	// ChangeNew
	m_arrCameras = arrNewCamera;
}

_uint UPipeLine::GetID(const CAMERATYPE _eCamerType)
{
	_uint iIndex{ MAIN_CAMERA_ID };
	if (_eCamerType != CAMERATYPE::MAIN && _eCamerType != CAMERATYPE::DEFFERED && _eCamerType != CAMERATYPE::SHADOWLIGHT)
	{
		for (_uint i = (_uint)CAMERATYPE::OTEHR; i < MAX_CAM_SIZE; ++i)
		{
			if (nullptr == m_arrCameras[i])
				return i;
		}
	}
	else if (_eCamerType == CAMERATYPE::DEFFERED)
	{
		iIndex = DEFFERED_CAMERA_ID;
	}
	else if (_eCamerType == CAMERATYPE::SHADOWLIGHT)
	{
		iIndex = SHADOWLIGHT_CAMERA_ID;
	}
	else
	{
		iIndex = MAIN_CAMERA_ID;
	}
	return iIndex;
}
