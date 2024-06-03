#pragma once
#include "CMob.h"
BEGIN(Engine)
class UGameInstance;
class UParticle;
class UParticleSystem;
END

BEGIN(Client)
/*
@ Date: 2024-05-13, Writer: �̼���
@ Explain
-  �̶� ���� Ŭ����.
*/
class CMummy final : public CMob {
public:
	CMummy(CSHPTRREF<UDevice> _spDevice, const _wstring& _wstrLayer, const CLONETYPE& _eCloneType);
	CMummy(const CMummy& _rhs);
	enum MUMMYTYPE {
		TYPE_STANDING, TYPE_LYING, TYPE_END
	};
	DESTRUCTOR(CMummy)
public:
	// UPlayer��(��) ���� ��ӵ�
	CLONE_MACRO(CMummy, "CMummy::CloneToFailed")
		virtual void Free() override;
	virtual HRESULT NativeConstruct() override;
	virtual HRESULT NativeConstructClone(const VOIDDATAS& _Datas) override;
	SHPTR<UParticle>& GetParticle() { return m_spParticle; }
	void SetMummyType(MUMMYTYPE _type) { m_MummyType = _type; }
	MUMMYTYPE GetMummyType() { return m_MummyType; }
protected:
	virtual void TickActive(const _double& _dTimeDelta) override;
	virtual void LateTickActive(const _double& _dTimeDelta) override;
	virtual HRESULT RenderActive(CSHPTRREF<UCommand> _spCommand, CSHPTRREF<UTableDescriptor> _spTableDescriptor) override;
	virtual HRESULT RenderShadowActive(CSHPTRREF<UCommand> _spCommand, CSHPTRREF<UTableDescriptor> _spTableDescriptor) override;
	virtual void Collision(CSHPTRREF<UPawn> _pEnemy) override;

private:
	MUMMYTYPE m_MummyType;
	SHPTR<UParticle>		m_spParticle;
	PARTICLEPARAM* m_stParticleParam;
	ComputeParticleType* m_stParticleType;

	
};

END

