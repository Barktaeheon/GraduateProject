#ifndef _SERVERFRAMEWORK_CORE_PUBLIC_ACELL_H
#define _SERVERFRAMEWORK_CORE_PUBLIC_ACELL_H

#include "ACoreBase.h"

BEGIN(Core)
/*
@ Date: 2023-01-14, Writer: ������
@ Explain
- Navigation���� ���� �浹�� ������ �����ε� �װ��� ���� Ŭ������ 3���� ���� �̷�
�ﰢ���� �̷�� �� ������ �÷��̾ ������� ����� �ʴ��� Ȯ���ϴ� Ŭ�����̴�.
*/
class CORE_DLL  ACell final : public ACoreBase {
public:
	enum POINT : _int { POINT_A, POINT_B, POINT_C, POINT_END };
	enum LINE : _int { LINE_AB, LINE_BC, LINE_CA, LINE_END };

	// CellDecs
	typedef struct tagCellDesc {
		ARRAY<Vector3, POINT_END>	vPoints{};
		ARRAY<Vector3, LINE_END>		vNormal{};
		ARRAY<Vector3, LINE_END>		vLine{};
		ARRAY<_int, LINE_END>				iNeighbor{ -1, -1, -1 };
		_int													iIndex{ -1 };
		_bool												bisJumpable{ false };
	}CELLDECS;
public:
	ACell();
	NO_COPY(ACell)
	DESTRUCTOR(ACell)
public:
	_bool NativeConstruct(const ARRAY<Vector3, POINT_END>& _Points, const _int _iIndex);
	_bool NativeConstruct(const CELLDECS& _tCellDesc);
	_bool IsIn(const Vector3& _vPos, REF_IN _int& _NeighborIndex);
	_bool UpdateNeighbor(SHPTR<ACell> _spCell, POINT _Point1, POINT _Point2, LINE _Line);
	_bool IsCompareCell(SHPTR<ACell> _spCell, POINT _Point1, POINT _Point2);
	_float ComputeHeight(const Vector3& _vPosition);

	Vector3 GetClosestPointOnEdges(const Vector3& position) const;
	Vector3 ClosestPointOnLine(const Vector3& lineStart, const Vector3& lineEnd, const Vector3& point) const;

	_float GetHeightAtXZ(const _float& x, const _float& z) const;

	SHPTR<ACell> FindNeighborCell(const _int _iIndex);
public: /* Get Set */
	const Vector3& GetPoint(const POINT& _ePoint) const { return m_arrPoints[_ePoint]; }
	const Vector3& GetLine(const LINE& _eLine) const { return m_arrLines[_eLine]; }
	const ARRAY<Vector3, POINT_END>& GetPoints() const { return m_arrPoints; }
	const ARRAY<Vector3, LINE_END>& GetLines() const { return m_arrLines; }
	const ARRAY<Vector3, LINE_END>& GetNormal() const { return m_arrNormals; }
	const ARRAY<_int, LINE_END>& GetNeighbor() const { return m_arrNeighbors; }
	const Vector3& GetCenterPos() const { return m_vCenterPos; }
	const _int& GetIndex() const { return m_iIndex; }
	const _bool IsJumpable() const { return m_isJumpable; }

	// Index Return
	const ARRAY<_int, LINE_END>& GetNeightborIndexes() const { return m_arrNeighbors; }
	const ARRAY<WKPTR<ACell>, POINT_END>& GetNeighborCells() const { return m_arrNeighborCells; }
private:
	void ResortPoints();
	void CalculateCrossResult(ARRAY<Vector3, POINT_END>& _Crosses);
	void MakeLineAndNormal();
private:
	virtual void Free() override;
private:
	// Points
	ARRAY<Vector3, POINT_END>					m_arrPoints;
	// Line
	ARRAY<Vector3, LINE_END>						m_arrLines;
	// Normal
	ARRAY<Vector3, LINE_END>						m_arrNormals;
	// Neighbor
	ARRAY<_int, LINE_END>								m_arrNeighbors;
	// Cells
	ARRAY<WKPTR<ACell>, POINT_END>		m_arrNeighborCells;
	// CenterPos
	Vector3															m_vCenterPos;
	// Index
	_int																	m_iIndex;
	// Planes
	Vector4															m_vPlane;
	// Jumpable
	_bool																m_isJumpable;
};
END

#endif // _SERVERFRAMEWORK_CORE_PUBLIC_ACELL_H