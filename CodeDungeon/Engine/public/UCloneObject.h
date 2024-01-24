#pragma once

#include "UObject.h"

BEGIN(Engine)
class UDevice;
class UGpuCommand;

/*
Actor, Resource, Component ���� �θ� Ŭ����
������ �ʿ��� Ŭ������ �ݵ�� �� Ŭ������ ��� �޾ƾ� �Ѵ�.
*/

class UCloneObject  abstract : public UObject {
public:
	UCloneObject(CSHPTRREF<UDevice> _spDevice);
	UCloneObject(const UCloneObject& _rhs);
	virtual ~UCloneObject() = default;
public:
	virtual SHPTR<UCloneObject> Clone(const VOIDDATAS& _stDatas) PURE;
	virtual void Free() override PURE;
	virtual HRESULT NativeConstruct() override PURE;
	virtual HRESULT NativeConstructClone(const VOIDDATAS& _stDatas) PURE;
protected:
	const _bool IsClone() const { return m_isClone; }
private:
	_bool											m_isClone;
};


END
