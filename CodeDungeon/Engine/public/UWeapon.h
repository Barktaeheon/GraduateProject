#pragma once
#include "UEquipment.h"

BEGIN(Engine)

/*
@ Date: 2024-04-27, Writer: ������
@ Explain
-  ĳ���Ͱ� ��� �ְ� �ϴ� ���⿡ ���� Ŭ����
*/

class UWeapon : public UEquipment {
public:
	UWeapon(CSHPTRREF<UDevice> _spDevice, const _wstring& _wstrLayer, const CLONETYPE& _eCloneType, 
		WEAPONTYPE _eWeaponType);
	UWeapon(const UWeapon& _rhs);
	DESTRUCTOR(UWeapon)
public:
	// UPawn��(��) ���� ��ӵ�
	CLONE_MACRO(UWeapon, "UWeapon::Clone To Failed")
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
	WEAPONTYPE					m_eWeaponType;
};


END

