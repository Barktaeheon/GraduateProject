#include "EngineDefines.h"
#include "UTerrain.h"
#include "UVIBufferTerrain.h"
#include "UTexGroup.h"
#include "UTransform.h"
#include "UShader.h"
#include "UGameInstance.h"
#include "UMethod.h"
#include "UShaderConstantBuffer.h"
#include "UTexture.h"

UTerrain::UTerrain(CSHPTRREF<UDevice> _spDevice, const _wstring& _wstrLayer, const CLONETYPE& _eCloneType) :
	UPawn(_spDevice, _wstrLayer, _eCloneType, BACKINGTYPE::STATIC), 
	m_spTessTerrainConstantBuffer{ nullptr }, 
	m_spVIBufferTerrain {nullptr }, 
	m_spTextureGroup{ nullptr }, 
	m_wstrDiffuseTexName{ L"" },
	m_TessTerrainParam{}
{
}

UTerrain::UTerrain(const UTerrain& _rhs) : UPawn(_rhs), 
m_spTessTerrainConstantBuffer{ _rhs.m_spTessTerrainConstantBuffer },
m_spVIBufferTerrain{ _rhs.m_spVIBufferTerrain },
m_spTextureGroup{ _rhs.m_spTextureGroup },
m_wstrDiffuseTexName{ _rhs.m_wstrDiffuseTexName },
m_TessTerrainParam{}
{
}

void UTerrain::Free()
{
}

HRESULT UTerrain::NativeConstruct()
{
	RETURN_CHECK_FAILED(__super::NativeConstruct(), E_FAIL);
	return S_OK;
}

HRESULT UTerrain::NativeConstructClone(const VOIDDATAS& _vecDatas)
{
	RETURN_CHECK_FAILED(__super::NativeConstructClone(_vecDatas), E_FAIL);
	RETURN_CHECK(_vecDatas.size() <= 0, E_FAIL);

	TERRAINDESC tTerrainDesc = UMethod::ConvertTemplate_Index<TERRAINDESC>(_vecDatas, 0);

	SHPTR<UGameInstance> pGameInstance = GET_INSTANCE(UGameInstance);
	// Clone To Resource
	m_spVIBufferTerrain = AddResource<UVIBufferTerrain>(tTerrainDesc.wstrBufferProtoName, RES_VIBUFFER);
	// Texture Proto Name
	if (L"" != tTerrainDesc.wstrTextureProtoName) {
		m_spTextureGroup = AddResource<UTexGroup>(tTerrainDesc.wstrTextureProtoName, RES_TEXGROUP);
	}
	AddShader(PROTO_RES_TERRAINTESSALATIONSHADER);

	m_spTessTerrainConstantBuffer = CreateNative<UShaderConstantBuffer>(GetDevice(),
		CBV_REGISTER::TESSTERRAINFACTOR, TERRAINTESSPARAM_SIZE);

	m_wstrDiffuseTexName = tTerrainDesc.wstrDiffuseTexName;
	m_wstrHeightTexName = tTerrainDesc.wstrHeightTexName;

	m_TessTerrainParam.iTiles.x = 15;
	m_TessTerrainParam.iTiles.y =15;
	m_TessTerrainParam.fTesselationMaxLevel = 10;
	if (!m_wstrHeightTexName.empty())
	{
		SHPTR<UTexture> spTexture = m_spTextureGroup->GetTexture(m_wstrHeightTexName);
		if(nullptr != spTexture)
		{
			m_TessTerrainParam.vHeightMapResolution = _float2{ spTexture->GetWidth(), spTexture->GetHeight() };
		}
	}
	else
	{
		m_TessTerrainParam.vHeightMapResolution = _float2{ 100.f, 100.f };
	}
	m_TessTerrainParam.vMinMaxTessellationDistance = _float2{ 1000.f, 5000.f };
	return S_OK;
}

_bool UTerrain::TerrainHeightPos(_float3& _vPosition)
{
	RETURN_CHECK(nullptr == m_spVIBufferTerrain, false);
	return m_spVIBufferTerrain->TerrainHeightPos(_vPosition, GetTransform()->GetWorldMatrix());
}

void UTerrain::TickActive(const _double& _dTimeDelta)
{
}

void UTerrain::LateTickActive(const _double& _dTimeDelta)
{
	AddRenderGroup(RENDERID::RI_NONALPHA_FIRST);
}

HRESULT UTerrain::RenderActive(CSHPTRREF<UCommand> _spCommand, CSHPTRREF<UTableDescriptor> _spTableDescriptor)
{
	__super::RenderActive(_spCommand, _spTableDescriptor);
	// Bind CBV Buffer
	GetShader()->BindCBVBuffer(m_spTessTerrainConstantBuffer, &m_TessTerrainParam, TERRAINTESSPARAM_SIZE);

	if (nullptr != m_spTextureGroup)
	{
		GetShader()->BindSRVBuffer(SRV_REGISTER::T0, m_spTextureGroup->GetTexture(m_wstrDiffuseTexName));
		if (!m_wstrHeightTexName.empty())
		{
			GetShader()->BindSRVBuffer(SRV_REGISTER::T1, m_spTextureGroup->GetTexture(m_wstrHeightTexName));
		}
	}
	{
		GetTransform()->BindTransformData(GetShader());
	}
	m_spVIBufferTerrain->Render(GetShader(), _spCommand);
	return S_OK;
}

void UTerrain::Collision(CSHPTRREF<UPawn> _pEnemy)
{
}
