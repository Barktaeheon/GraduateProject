#pragma once
#include "UPawn.h"

BEGIN(Engine)

class UTexGroup;
class UVIBufferRect;
class UShaderConstantBuffer;
class UCollider;

class UGuard : public UPawn {
public:
	UGuard(CSHPTRREF<UDevice> _spDevice, const _wstring& _wstrLayer, const CLONETYPE& _eCloneType);
	UGuard(const UGuard& _rhs);
	DESTRUCTOR(UGuard)
public:
	CLONE_MACRO(UGuard, "UGuard::Clone To Failed")
		virtual void Free() override;
	virtual HRESULT NativeConstruct() override;
	virtual HRESULT NativeConstructClone(const VOIDDATAS& _vecDatas) override;
	virtual void Collision(CSHPTRREF<UPawn> _pEnemy, const _double& _dTimeDelta) override;
protected:
	// Tick, LateTick, Render
	virtual void TickActive(const _double& _dTimeDelta) override;
	virtual void LateTickActive(const _double& _dTimeDelta) override;
	virtual HRESULT RenderActive(CSHPTRREF<UCommand> _spCommand, CSHPTRREF<UTableDescriptor> _spTableDescriptor) override;
	virtual HRESULT RenderShadowActive(CSHPTRREF<UCommand> _spCommand, CSHPTRREF<UTableDescriptor> _spTableDescriptor) override;
	virtual HRESULT RenderOutlineActive(CSHPTRREF<UCommand> _spCommand, CSHPTRREF<UTableDescriptor> _spTableDescriptor, _bool _pass = true) override;
	
protected:
	SHPTR<UTexGroup>				m_spGuardTexGroup;

private:

	// Cube
	SHPTR< UVIBufferRect>								m_spVIBufferRect;
public:
	CSHPTRREF<UTexGroup> GetTextureGroup() const { return m_spGuardTexGroup; }
	void SetColorTexture(const _wstring& TexName);
	void SetColorTexture(_uint _index);
	
	_uint ColorTextureIndex = 0;
	SHPTR<UCollider>			m_spCollider;
};

END