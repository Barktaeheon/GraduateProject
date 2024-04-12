#pragma once
#include "UComponent.h"

BEGIN(Engine)
class UPawn;

enum MapObjectType {
	ROOM,
	DECORATION,
	INTERACTION,
	OBJECTEND
};

typedef struct TransformDesc {
	MapObjectType eType{};
	_float3 vPos{ 0, 0, 0 };
	_quaternion vRotate{ 0 ,0 ,0, 0};
	_float3 vScale{ 0, 0, 0 };
}TFDESC;

using MAPOBJECTSCONTAINER = UNORMAP<_string, TFDESC>; //<������Ʈ �̸�, ������>

class UMapLayout : public UComponent {
public:

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
	_bool Load(const _wstring& _wstrPath);
	
	void AddtoMapContainer(const _string& _ObjName, TFDESC& _ObjData);
	void ConfigModelTFData(CSHPTRREF<UPawn> _Pawn, TFDESC* _TFData, MapObjectType _Type);
private:

	SHPTR<MAPOBJECTSCONTAINER> m_spMapObjectsContainer;



};


END;
