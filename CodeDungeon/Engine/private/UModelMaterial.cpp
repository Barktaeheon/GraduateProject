#include "EngineDefine.h"
#include "UModelMaterial.h"
#include "UMethod.h"
#include "UShader.h"
#include "UTexGroup.h"

// Material�� ��ȣ
_int													UModelMaterial::s_AllMaterialDrawLength;
// �ѹ����� �ֱ� ���� �����̳�
 VECTOR<_int>								UModelMaterial::s_RemainMaterialDrawNumber;

UModelMaterial::UModelMaterial(CSHPTRREF<UDevice> _spDevice, const DESC& _MaterialDesc) :
	UComponent(_spDevice), m_MaterialInfoContainer{ _MaterialDesc.MaterialInfoContainer },  
	m_MaterialTexContainer{ _MaterialDesc.MaterialTexContainer }, m_BindTextureContainer{}, m_iMaterialIndex{0}
{
}

UModelMaterial::UModelMaterial(const UModelMaterial& _rhs) : 
	UComponent(_rhs), m_MaterialInfoContainer{ _rhs.m_MaterialInfoContainer },
	m_MaterialTexContainer{ _rhs.m_MaterialTexContainer }, m_BindTextureContainer{}, m_iMaterialIndex{ 0 }
{ 
}

void UModelMaterial::Free()
{
	s_RemainMaterialDrawNumber.push_back(m_iMaterialIndex);
}

HRESULT UModelMaterial::NativeConstruct()
{
	// ���� �����ִ� ���͸��� DrawNumber�� ����������, Remain Numbering���� ã�ƿ´�. 
	if (s_RemainMaterialDrawNumber.size() <= 0)
	{
		m_iMaterialIndex = s_RemainMaterialDrawNumber.front();
		s_RemainMaterialDrawNumber.erase(s_RemainMaterialDrawNumber.begin());
	}
	else
	{
		m_iMaterialIndex = s_AllMaterialDrawLength;
		s_AllMaterialDrawLength++;
	}
	return __super::NativeConstruct();
}

HRESULT UModelMaterial::NativeConstructClone(const VOIDDATAS& _stDatas)
{
	return __super::NativeConstructClone(_stDatas);
}

void UModelMaterial::BindTexture(CSHPTRREF<UShader> _spShader, const SRV_REGISTER _srvRegister, TEXTYPE _TexType, _int _TextureOrder)
{
	assert(nullptr != _spShader);
	RETURN_CHECK(nullptr == m_MaterialTexContainer[_TexType], ;);
	_spShader->BindSRVBuffer(_srvRegister, m_MaterialTexContainer[_TexType]->GetTexture(_TextureOrder));
}

void UModelMaterial::BindTexture(CSHPTRREF<UShader> _spShader, const SRV_REGISTER _srvRegister, TEXTYPE _TexType, const _wstring& _wstrTextureName)
{
	assert(nullptr != _spShader);
	_spShader->BindSRVBuffer(_srvRegister, m_MaterialTexContainer[_TexType]->GetTexture(_wstrTextureName));
}

void UModelMaterial::RegisterBindTextureContainer(const SRV_REGISTER _srvRegister, TEXTYPE _TexType, _int _TextureOrder)
{
	assert(nullptr != m_MaterialTexContainer[_TexType]);
	m_BindTextureContainer.emplace(MakePair(_srvRegister, m_MaterialTexContainer[_TexType]->GetTexture(_TextureOrder)));
}

void UModelMaterial::RegisterBindTextureContainer(const SRV_REGISTER _srvRegister, TEXTYPE _TexType, const _wstring& _wstrTextureName)
{
	assert(nullptr != m_MaterialTexContainer[_TexType]);
	m_BindTextureContainer.emplace(MakePair(_srvRegister, m_MaterialTexContainer[_TexType]->GetTexture(_wstrTextureName)));
}

