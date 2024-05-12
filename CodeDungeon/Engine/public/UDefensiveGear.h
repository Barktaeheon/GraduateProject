#pragma once
#include "UEquipment.h"

BEGIN(Engine)

/*
@ Date: 2024-04-27, Writer: ������
@ Explain
-  ĳ���Ͱ� �Դ� ���ʿ� ���� Ŭ����
*/

class UDefensiveGear : public UEquipment {
public:
	UDefensiveGear(CSHPTRREF<UDevice> _spDevice, const _wstring& _wstrLayer, const CLONETYPE& _eCloneType);
	UDefensiveGear(const UDefensiveGear& _rhs);
	DESTRUCTOR(UDefensiveGear)
public:
	// UPawn��(��) ���� ��ӵ�
	CLONE_MACRO(UDefensiveGear, "UDefensiveGear::Clone To Failed")
	virtual void Free() override;
	virtual HRESULT NativeConstruct() override;
	virtual HRESULT NativeConstructClone(const VOIDDATAS& _Datas) override;
protected:
	virtual void TickActive(const _double& _dTimeDelta) override;
	virtual void LateTickActive(const _double& _dTimeDelta) override;
	virtual HRESULT RenderActive(CSHPTRREF<UCommand> _spCommand, CSHPTRREF<UTableDescriptor> _spTableDescriptor) override;
	virtual HRESULT RenderShadowActive(CSHPTRREF<UCommand> _spCommand, CSHPTRREF<UTableDescriptor> _spTableDescriptor) override;
	virtual void Collision(CSHPTRREF<UPawn> _pEnemy) override;
private:
	DEFENSIVEGEARTYPE				m_eDefensiveGearType;
};


END
