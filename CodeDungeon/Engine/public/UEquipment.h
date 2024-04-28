#pragma once

#include "UPawn.h"


BEGIN(Engine)
class UCharacter;
class UCollider;
/*
@ Date: 2024-04-27, Writer: ������
@ Explain
-  ĳ���Ͱ� ��� �ְ� �ϴ� ��� ���� Ŭ�����̴�. 
*/
class UEquipment abstract : public UPawn {
public:
	struct DESC {
		// ������ �Ǵ� ĳ���� 
		WKPTR<UCharacter> wpOwnerCharacter;
	};
public:
	UEquipment(CSHPTRREF<UDevice> _spDevice, const _wstring& _wstrLayer, const CLONETYPE& _eCloneType);
	UEquipment(const UEquipment& _rhs);
	DESTRUCTOR(UEquipment)
public:
	// UPawn��(��) ���� ��ӵ�
	virtual SHPTR<UCloneObject> Clone(const VOIDDATAS& _vecDatas) PURE;
	virtual void Free() PURE;
	virtual HRESULT NativeConstruct() PURE;
	virtual HRESULT NativeConstructClone(const VOIDDATAS& _Datas) PURE;
protected:
	virtual void TickActive(const _double& _dTimeDelta) PURE;
	virtual void LateTickActive(const _double& _dTimeDelta) PURE;
	virtual HRESULT RenderActive(CSHPTRREF<UCommand> _spCommand, CSHPTRREF<UTableDescriptor> _spTableDescriptor) PURE;
	virtual HRESULT RenderShadowActive(CSHPTRREF<UCommand> _spCommand, CSHPTRREF<UTableDescriptor> _spTableDescriptor) PURE;
	virtual void Collision(CSHPTRREF<UPawn> _pEnemy) PURE;
private:
	// Owner Character
	WKPTR<UCharacter>		m_wpOwnerCharacter;
	

};

END

