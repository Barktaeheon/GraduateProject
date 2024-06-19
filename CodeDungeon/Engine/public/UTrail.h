#pragma once
#include "UPawn.h"

BEGIN(Engine)

class UTexGroup;
class UVIBufferTrail;
class UShaderConstantBuffer;

class UTrail : public UPawn {
public:
	UTrail(CSHPTRREF<UDevice> _spDevice, const _wstring& _wstrLayer, const CLONETYPE& _eCloneType);
	UTrail(const UTrail& _rhs);
	DESTRUCTOR(UTrail)
public:
	CLONE_MACRO(UTrail, "UTrail::Clone To Failed")
		virtual void Free() override;
	virtual HRESULT NativeConstruct() override;
	virtual HRESULT NativeConstructClone(const VOIDDATAS& _vecDatas) override;
	virtual void Collision(CSHPTRREF<UPawn> _pEnemy) override;
protected:
	// Tick, LateTick, Render
	virtual void TickActive(const _double& _dTimeDelta) override;
	virtual void LateTickActive(const _double& _dTimeDelta) override;
	virtual HRESULT RenderActive(CSHPTRREF<UCommand> _spCommand, CSHPTRREF<UTableDescriptor> _spTableDescriptor) override;
	virtual HRESULT RenderShadowActive(CSHPTRREF<UCommand> _spCommand, CSHPTRREF<UTableDescriptor> _spTableDescriptor) override;
protected:
	SHPTR<UTexGroup>				m_spTrailTexGroup;
	SHPTR< UShaderConstantBuffer>				m_spTrailColorBuffer;
	_float4				m_vCurTrailColor;
private:

	// Cube
	SHPTR< UVIBufferTrail>								m_spVIBufferTrail;

	_int								m_iMaxCount = 0;
	_int								m_iDivide = 0;
	_bool							m_bRender;
	_float							m_fCreateTime = 0.f;
	_float							m_fTime = 0.f;
	LIST<std::pair<_float3, _float3>>	m_listPos; //Top,Bottom
	LIST<std::pair<_float3, _float3>>	m_listRomPos;
public:
	void AddTrail(_float3 _top, _float3 _bottom);
	void SetRenderingTrail(_bool isOn);
	void SetColor(_float4 _col);
	void Update(const _double& _dTimeDelta);
};

END