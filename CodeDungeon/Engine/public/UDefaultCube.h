#pragma once
#include "UPawn.h"

BEGIN(Engine)
class UVIBufferCube;
class UShaderConstantBuffer;

class UDefaultCube : public UPawn{
public:
	struct CUBEDESC
	{
		_float4 vColor{};
	};
public:
	UDefaultCube(CSHPTRREF<UDevice> _spDevice, const _wstring& _wstrLayer, const CLONETYPE& _eCloneType);
	UDefaultCube(const UDefaultCube& _rhs);
	DESTRUCTOR(UDefaultCube)

public:
	CLONE_MACRO(UDefaultCube, "UDefaultCube::Clone To Failed")
	virtual void Free() override;
	virtual HRESULT NativeConstruct() override;
	virtual HRESULT NativeConstructClone(const VOIDDATAS& _vecDatas) override;
protected:
	// Tick, LateTick, Render
	virtual void TickActive(const _double& _dTimeDelta) override;
	virtual void LateTickActive(const _double& _dTimeDelta) override;
	virtual HRESULT RenderActive(CSHPTRREF<UCommand> _spCommand, CSHPTRREF<UTableDescriptor> _spTableDescriptor) override;
	virtual HRESULT RenderShadowActive(CSHPTRREF<UCommand> _spCommand, CSHPTRREF<UTableDescriptor> _spTableDescriptor) override;
	// Damaged
	virtual void Collision(CSHPTRREF<UPawn> _pEnemy, const _double& _dTimeDelta) override;
private:
	static const _uint									CUBEPARAM_SIZE{sizeof(NORMALCUBEPARAM)};
	// Cube
	SHPTR< UVIBufferCube>					m_spVIBufferSphere;
	SHPTR<UShaderConstantBuffer>	m_spNormalCubeShaderConstnatBuffer;
	NORMALCUBEPARAM							m_stNormalCubeParam;
};

END