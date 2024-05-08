#pragma once
#include "UComponent.h"

BEGIN(Engine)
class UPawn;
class UTransform;

class UMapLayout : public UComponent {
public:
	typedef struct ObjectDesc {
		_string		_sModelName{};
		_float4x4		_mWorldMatrix{ _float4x4::Identity };

	}OBJDESC;
	using MAPOBJECTS = VECTOR<OBJDESC>;
	using MAPOBJECTSCONTAINER = UNORMAP<_string, MAPOBJECTS>; //<�� �̸�, ������>

public:
	UMapLayout(CSHPTRREF <UDevice> _spDevice);
	UMapLayout(const UMapLayout& _rhs);
	DESTRUCTOR(UMapLayout)
	CLONE_MACRO(UMapLayout, "MapLayout::Clone To Failed")
	// UComponent��(��) ���� ��ӵ�
	void Free() override;
	HRESULT NativeConstruct() override;
	HRESULT NativeConstructClone(const VOIDDATAS& _tDatas) override;

	_bool Save(const _wstring& _wstrPath);
	_bool Load();
	
	void AddtoMapContainer(const _string& _RoomName, MAPOBJECTS& _ObjData);

	CSHPTRREF<MAPOBJECTSCONTAINER> GetMapObjectsContainer() { return m_spMapObjectsContainer;};

private:

	SHPTR<MAPOBJECTSCONTAINER> m_spMapObjectsContainer;


};


END;
