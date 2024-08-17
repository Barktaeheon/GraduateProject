#pragma once
#include "UPawn.h"

BEGIN(Engine)
class UModel;
class UShaderConstantBuffer;
class UCollider;
END

BEGIN(Client)
/*
@ Date: 2024-04-23, Writer: �̼���
@ Explain
- ���� ������ ������Ʈ�� ��Ÿ���� Ŭ����
*/
class CModelObjects : public UPawn {
public:
	CModelObjects(CSHPTRREF<UDevice> _spDevice,
		const _wstring& _wstrLayer, const CLONETYPE& _eCloneType);
	CModelObjects(const CModelObjects& _rhs);
	DESTRUCTOR(CModelObjects)
		// UPawn��(��) ���� ��ӵ�
		CLONE_MACRO(CModelObjects, "CModelObjects::Clone To Failed")
		virtual void Free() override;
	virtual HRESULT NativeConstruct() override;
	virtual HRESULT NativeConstructClone(const VOIDDATAS& _vecDatas) override;

	void SetModel(const _wstring& _ProtoModelName);
	CSHPTRREF<UModel> GetModel(){ return m_spModel; }
	void CalculateAndSetCollider();
	void SetOutline(_bool _draw) { m_bDrawOutline = _draw; }
	const _bool& GetOutlineState()const { return m_bDrawOutline; }
	void SetIfOutlineScale(_bool _Draw) { m_OutlineWithScale = _Draw; }
	const _bool& GetOutlineScaleState()const { return m_OutlineWithScale; }

	void SetCheckPointToOtherColor(_bool _true) { isCheckpoint = _true; }
	_bool GetCheckPointToOtherColor() { return isCheckpoint; }

	const _bool& GetInteractionState() { return m_bisInteraction; }
	void SetInteractionState(const _bool& _newState) { m_bisInteraction = _newState; }
protected:
	virtual void TickActive(const _double& _dTimeDelta) override;
	virtual void LateTickActive(const _double& _dTimeDelta) override;
	virtual HRESULT RenderActive(CSHPTRREF<UCommand> _spCommand, CSHPTRREF<UTableDescriptor> _spTableDescriptor) override;
	virtual HRESULT RenderShadowActive(CSHPTRREF<UCommand> _spCommand, CSHPTRREF<UTableDescriptor> _spTableDescriptor) override;
	virtual HRESULT RenderOutlineActive(CSHPTRREF<UCommand> _spCommand, CSHPTRREF<UTableDescriptor> _spTableDescriptor, _bool _pass = true) override;
	virtual void Collision(CSHPTRREF<UPawn> _pEnemy, const _double& _dTimeDelta) override;
protected: /* get set */
	_bool IsEnable() const { return m_isEnable; }
	void SetEnable(const _bool _isEnable) { this->m_isEnable = _isEnable; }

	_bool IsActiveEnable() const { return m_isActiveEnable; }
	void SetActiveEnable(const _bool _isEnable) { this->m_isActiveEnable = _isEnable; }
private:
	SHPTR<UModel>								m_spModel;
	SHPTR<UShaderConstantBuffer>				m_spShaderTexCheckBuffer;
	SHPTR<UShaderConstantBuffer>				m_spShaderCheckPointBuffer;
	_bool isCheckpoint;
	SHPTR< UShaderConstantBuffer>						m_spScaleOutlineBuffer;
	int HasTex[4]{};

	SHPTR<UCollider>			m_spCollider;
	_bool									m_bDrawOutline;
	_bool									m_OutlineWithScale = false;
	_bool									m_bisInteraction;
	_bool									m_isEnable;
	_bool									m_isActiveEnable;
	_float									m_fActiveRange;
};

END