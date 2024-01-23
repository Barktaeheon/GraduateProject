#ifndef _SERVERFRAMEWORK_CORE_PUBLIC_UCELL_H
#define _SERVERFRAMEWORK_CORE_PUBLIC_UCELL_H

#include "UBase.h"

BEGIN(Core)
/*
@ Date: 2023-01-14, Writer: ������
@ Explain
- Navigation���� ���� �浹�� ������ �����ε� �װ��� ���� Ŭ������ 3���� ���� �̷�
�ﰢ���� �̷�� �� ������ �÷��̾ ������� ����� �ʴ��� Ȯ���ϴ� Ŭ�����̴�.
*/
class CORE_DLL  UCell final : public UBase {
public:
	enum POINT : _int { POINT_A, POINT_B, POINT_C, POINT_END };
	enum LINE : _int { LINE_AB, LINE_BC, LINE_CA, LINE_END };
public:
	UCell();
	NO_COPY(UCell)
	DESTRUCTOR(UCell)
public:
	_bool NativeConstruct(const ARRAY<Vector3, POINT_END>& _Points, const _int _iIndex);
	_bool IsIn(const Vector3& _vPos, REF_IN _int& _NeighborIndex);
	_bool UpdateNeighbor(SHPTR<UCell> _spCell, POINT _Point1, POINT _Point2, LINE _Line);
	_bool IsCompareCell(SHPTR<UCell> _spCell, POINT _Point1, POINT _Point2);
	_float ComputeHeight(const Vector3& _vPosition);
public: /* Get Set */
	Vector3 GetCenterPos() const { return m_vCenterPos; }
	_int GetIndex() const { return m_Index; }
	Vector3 GetPoint(POINT _point) const { return m_Points[_point]; }
	// Index Return
	const ARRAY<_int, LINE_END>& GetNeightborIndexes() const { return m_NeighborIndexes; }
	ARRAY<UCell*, LINE_END> GetNeighborCelles() const { return m_NeightborCells; }
private:
	void ResortPoints();
	void CalculateCrossResult(ARRAY<Vector3, POINT_END>& _Crosses);
	void MakeLineAndNormal();
private:
	virtual void Free() override;
private:
	Vector3												m_vCenterPos;
	_int														m_Index;
	Vector4												m_vPlane;
	// Points
	ARRAY<Vector3, POINT_END>		m_Points;
	ARRAY<Vector3, LINE_END>			m_Lines;
	ARRAY<Vector3, LINE_END>			m_Normals;
	ARRAY<_int, LINE_END>					m_NeighborIndexes;
	// Use Extern
	ARRAY<UCell*, LINE_END>			m_NeightborCells;
};
END

#endif // _SERVERFRAMEWORK_CORE_PUBLIC_UCELL_H