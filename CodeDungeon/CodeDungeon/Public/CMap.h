#pragma once
#include "UComponent.h"
#include "UMapLayout.h"

BEGIN(ENGINE)
class UStageManager;
class UMapLayout;
END

/*
@ Date: 2024-04-23, Writer: �̼���
@ Explain
- ���� ����ϴ� Ŭ����
*/
BEGIN(Client)
class CRooms;
class CModelObjects;

using ROOMCONTAINER = UNORMAP<_wstring, SHPTR<CRooms>>;
using OBJCONTAINER = VECTOR<SHPTR<CModelObjects>>;
using STATICOBJCONTAINER = UNORMAP<_wstring, OBJCONTAINER>;
class CMap : public UComponent
{
public:
	CMap(CSHPTRREF <UDevice> _spDevice);
	CMap(const CMap& _rhs);
	DESTRUCTOR(CMap)

	// UComponent��(��) ���� ��ӵ�
	void Free() override;
	SHPTR<UCloneObject> Clone(const VOIDDATAS& _tDatas) override;
	HRESULT NativeConstruct() override;
	HRESULT NativeConstructClone(const VOIDDATAS& _tDatas) override;

	void LoadRooms();
	void LoadStaticObjects();
	
	CSHPTRREF<ROOMCONTAINER> GetRooms() { return m_spRoomContainer; }
	CSHPTRREF<STATICOBJCONTAINER> GetStaticObjs() { return m_spStaticObjContainer;}

	void AddLightCount() { m_iLightCount++; }
	const _uint GetLightCount() { return m_iLightCount; }
private:
	SHPTR<ROOMCONTAINER>			m_spRoomContainer;
	SHPTR<UMapLayout>				m_spMapLayout;
	SHPTR<STATICOBJCONTAINER>		m_spStaticObjContainer;
	_uint							m_iLightCount;

};
END
