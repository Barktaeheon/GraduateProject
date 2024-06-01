#pragma once

#include "UComponent.h"
#include "UCell.h"

BEGIN(Engine)
class UTransform;
class UCollider;
using CELLCONTAINER = std::vector<SHPTR<UCell>>;

class UNavigation : public UComponent{
public:
	typedef struct tagNavDesc {
		_uint iCurIndex{ 0 };
	}NAVDESC;
public:
	UNavigation(CSHPTRREF <UDevice> _spDevice);
	UNavigation(const UNavigation& _rhs);
	DESTRUCTOR(UNavigation)
public:
	CLONE_MACRO(UNavigation, "Navigation::Clone To Failed")
	virtual void Free() override;
	virtual HRESULT NativeConstruct() override;
	//HRESULT NativeConstruct(const VECTOR<SHPTR< UVIBufferTerrain>>& _vecTerrain);
	/*HRESULT NativeConstruct(const VECTOR<_float3>& _vecPosList);*/
	virtual HRESULT NativeConstructClone(const VOIDDATAS& _vecDatas) override;
	// Compute Height
	const _float  ComputeHeight(const _float3& _vPosition);
	void ComputeHeight(CSHPTRREF<UTransform> _spTransform);
	// Move
	_bool IsMove(const _float3& _vPosition, SHPTR<UCell>& _spCell);
	// Find
	SHPTR<UCell> FindCell(const _float3& _vPosition);
	SHPTR<UCell> FindCell(const _int& _iIndex);
	// Is Collision
	_bool IsCollision(SHPTR<UCollider>& _pCollider);
	// Insert Cell
	void AddCell(SHPTR<UCell>& _spCell);
	// SaveLoada
	_bool Load(const _wstring& _wstrPath);
	_bool Save(const _wstring& _wstrPath);

	CSHPTR<CELLCONTAINER> GetCells() const { return m_spCellContainer; }
	const _int& GetCurIndex() const { return m_iCurIndex; }
	void SetCurIndex(const _int& _iIndex) { m_iCurIndex = _iIndex; }
	// Get Collider
	SHPTR<UCollider> GetCollider() { return m_spCollider; }
	SHPTR<UCell> GetCurCell() { return m_spCurCell; }
	void SetCurCell(CSHPTRREF<UCell> newCell) { m_spCurCell = newCell; SetCurIndex(newCell->GetIndex()); }
	// Ready Neighbor
	HRESULT ReadyNeighbor();

	/*
	@ Date: 2024-05-14, Writer: �̼���
	@ Explain
	-  �����̵� ���� ��� �Լ�.
	*/
	_float3 ClampPositionToCell(const _float3& position);


private:
	SHPTR<CELLCONTAINER>		m_spCellContainer;
	SHPTR<UCell>				m_spCurCell;
	_int						m_iCurIndex;
	SHPTR<UCollider>			m_spCollider;

};

END

