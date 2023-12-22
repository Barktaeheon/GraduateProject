#ifndef _PTH_FRAMEWORK_ENGIEN_PUBLIC_UOBJECT_H
#define _PTH_FRAMEWORK_ENGIEN_PUBLIC_UOBJECT_H

#include "UBase.h"

BEGIN(Engine)
class UDevice;
class UCommand;
/*
Device, GpuCommand�� ������ �ִ� Ŭ�����̴�. 
*/


class ENGINE_DLL  UObject : public UBase {
public:
	UObject(CSHPTRREF<UDevice> _spDevice);
	UObject(const UObject& _rhs);
	virtual ~UObject() = default;
public:
	virtual void Free() override PURE;
	virtual HRESULT NativeConstruct() PURE;
#ifdef _USE_IMGUI
	virtual void ShowObjectInfo();
#endif
protected:
	CSHPTRREF<UDevice> GetDevice() const { return m_spDevice; }
private:
	SHPTR<UDevice>						m_spDevice;
	SHPTR<UCommand>				m_spCommand;
};

END

#endif // _PTH_FRAMEWORK_ENGIEN_PUBLIC_UOBJECT_H