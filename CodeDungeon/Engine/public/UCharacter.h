#pragma once

#include "UPawn.h"

BEGIN(Engine)
class UTransform;
/*
@ Date: 2024-02-25, Writer: ������
@ Explain
-  ����ڰ� �����̰ų� AI�� ������ �� ����ϴ� Ŭ�����̴�. 
*/
class UCharacter abstract : public UPawn {
public:
	UCharacter(CSHPTRREF<UDevice> _spDevice, const _wstring& _wstrLayer, const CLONETYPE& _eCloneType);
	UCharacter(const UCharacter& _rhs);
	DESTRUCTOR(UCharacter)
public:
	// UPawn��(��) ���� ��ӵ�
	virtual SHPTR<UCloneObject> Clone(const VOIDDATAS& _vecDatas) PURE;
	virtual void Free() PURE;
	virtual HRESULT NativeConstruct() PURE;
	virtual HRESULT NativeConstructClone(const VOIDDATAS& _Datas) PURE;
public:
	// �ٸ� ĳ���Ϳ��� �Ÿ� ���� 
	_float OtherCharacterToDistance(CSHPTRREF<UActor> _spOtherActor);
	// �ٸ� ĳ���͸� �ٶ󺸴� �ڱ� �ڽ��� Look Angle 
	_float OtherCharacterDirToLook(CSHPTRREF<UActor> _spOtherActor);
	// �ٸ� ĳ���͸� �������� �ٶ󺸴� �ڱ� �ڽ��� Right Angle 
	_float OhterCharacterDirToRight(CSHPTRREF<UActor> _spOtherActor);
	// �ٸ� ĳ���͸� �ٶ󺸴� �ڱ� �ڽ��� Look Angle ( 180 ~ -180 )
	_float OtherCharacterDirToLookConverter(CSHPTRREF<UActor>  _spOtherActor);
	// �ٸ� ĳ���͸� �ٶ󺸴� �ڱ� �ڽ��� Look Direction
	_float3 OtherCharacterDirToLookVectorF3(CSHPTRREF<UActor>  _spOtherActor);
	// �ٸ� ĳ���Ϳ��� �Ÿ� ���� 
	_float OtherCharacterToDistance(CSHPTRREF<UTransform> _spOtherTransform);
	// �ٸ� ĳ���͸� �ٶ󺸴� �ڱ� �ڽ��� Look Angle 
	_float OtherCharacterDirToLook(CSHPTRREF<UTransform> _spOtherTransform);
	// �ٸ� ĳ���͸� �������� �ٶ󺸴� �ڱ� �ڽ��� Right Angle 
	_float OhterCharacterDirToRight(CSHPTRREF<UTransform> _spOtherTransform);
	// �ٸ� ĳ���͸� �ٶ󺸴� �ڱ� �ڽ��� Look Angle ( 180 ~ -180 )
	_float OtherCharacterDirToLookConverter(CSHPTRREF<UTransform> _spOtherTransform);
	// �ٸ� ĳ���͸� �ٶ󺸴� �ڱ� �ڽ��� Look Direction
	_float3 OtherCharacterDirToLookVectorF3(CSHPTRREF<UTransform> _spOtherTransform);
protected:
	virtual void TickActive(const _double& _dTimeDelta) PURE;
	virtual void LateTickActive(const _double& _dTimeDelta) PURE;
	virtual HRESULT RenderActive(CSHPTRREF<UCommand> _spCommand, CSHPTRREF<UTableDescriptor> _spTableDescriptor) PURE;
	virtual void Collision(CSHPTRREF<UPawn> _pEnemy) PURE;
private:

};

END
