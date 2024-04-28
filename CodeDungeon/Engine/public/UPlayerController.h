#pragma once
#include "UController.h"

BEGIN(Engine)
/*
@ Date: 2024-04-28, Writer: ������
@ Explain
-  ������� �Է��� �޾Ƽ� �����̴� Ŭ����
*/
class UPlayerController abstract : public UController  {
public:
	UPlayerController(CSHPTRREF<UDevice> _spDevice);
	UPlayerController(const UPlayerController& _rhs);
	virtual ~UPlayerController() = default;
public:
	virtual void Free() override PURE;
	virtual SHPTR<UCloneObject> Clone(const VOIDDATAS& _tDatas) PURE;
public:
	// Native Construct 
	virtual HRESULT NativeConstruct() override  PURE;
	// Clone 
	virtual HRESULT NativeConstructClone(const VOIDDATAS& _tDatas) override  PURE;
	// Tick
	virtual void Tick(UCharacter* _pCharacter, const _double& _dTimeDelta) override  PURE;
	// InputTriggerData
	virtual _bool InputTriggerData(UCharacter* _pCharacter, const _int _Data) override  PURE;
};

END