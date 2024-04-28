#pragma once

#include "UComponent.h"

BEGIN(Engine)
class UCollider;
/*
@ Date: 2024-04-27, Writer: ������
@ Explain
-  ��Ʋ�� ���� �ݶ��̴��鿡 ���� ������ ��Ƴ��� Ŭ�����̴�. 
*/
class UCombatCollGroupComp final : public UComponent{
public:
	UCombatCollGroupComp(CSHPTRREF<UDevice> _spDevice);
	UCombatCollGroupComp(const UCombatCollGroupComp& _rhs);
	virtual ~UCombatCollGroupComp() = default;
public:
	virtual void Free() override;
	virtual SHPTR<UCloneObject> Clone(const VOIDDATAS& _tDatas) override;
public:
	// Native Construct 
	virtual HRESULT NativeConstruct() override;
	// Clone 
	virtual HRESULT NativeConstructClone(const VOIDDATAS& _tDatas) override;
private:

};

END