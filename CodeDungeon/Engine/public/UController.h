#pragma once

#include "UComponent.h"

BEGIN(Engine)
class UCharacter;

/*
@ Date: 2024-04-28, Writer: ������
@ Explain
-  ĳ������ �������� �����ϴ� Ŭ����
*/
class UController abstract : public UComponent{
public:
	struct CONTROLLERDESC {
		SHPTR<UCharacter> spCharacter;

		CONTROLLERDESC() : spCharacter{nullptr}{}
		CONTROLLERDESC(CSHPTRREF<UCharacter> _spCharacter) : spCharacter{_spCharacter} {}
	};
public:
	UController(CSHPTRREF<UDevice> _spDevice);
	UController(const UController& _rhs);
	virtual ~UController() = default;
public:
	virtual void Free() override PURE;
	virtual SHPTR<UCloneObject> Clone(const VOIDDATAS& _tDatas) PURE;
public:
	// Native Construct 
	virtual HRESULT NativeConstruct() override PURE;
	// Clone 
	virtual HRESULT NativeConstructClone(const VOIDDATAS& _tDatas) override PURE;
	// Tick
	virtual void Tick(const _double& _dTimeDelta) PURE;
protected:
	SHPTR<UCharacter> GetOwnerCharacter() { return m_wpOwnerCharacter.lock();  }
private:
	WKPTR<UCharacter>					m_wpOwnerCharacter;


};

END