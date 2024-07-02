#ifndef _SERVERFRAMEWORK_CORE_PUBLIC_UNAVIGATION_H
#define _SERVERFRAMEWORK_CORE_PUBLIC_UNAVIGATION_H

#include "ACoreBase.h"

BEGIN(Core)
/*
@ Date: 2023-01-14, Writer: ������
@ Explain
-  �ش� �� ���� ���� �ʴ��� �Ǵ��ϰ�, ���� ���ϴ� ��ġ������ ã���� Ŭ����
 Index ���� ��� ���� ���� �ƴϹǷ� datarace�� �Ͼ ���� �������� �ʴ´�. �׷��� atomic�� ���� �ʴ´�.
*/
class ACell;
class ATransform;
class ACollider;
using CELLCONTAINER = VECTOR<SHPTR<ACell>>;

class CORE_DLL ANavigation final : public ACoreBase {
public:
	typedef struct tagNavDesc {
		_uint iCurIndex{ 0 };
	}NAVDESC;

	struct CellPathNode {
		SHPTR<ACell> cell;
		_float cost;
		_float heuristic;
		_float totalCost;
		SHPTR<ACell> parent;

		bool operator>(const CellPathNode& other) const {
			return totalCost > other.totalCost;
		}
	};

	struct PathFindingState {
		std::priority_queue<CellPathNode, VECTOR<CellPathNode>, std::greater<CellPathNode>> openSet;
		std::unordered_map<SHPTR<ACell>, _float> costSoFar;
		std::unordered_map<SHPTR<ACell>, SHPTR<ACell>> cameFrom;
		SHPTR<ACell> endCell;
		bool pathFound = false;
		VECTOR<SHPTR<ACell>> path;
	};
public:
	ANavigation();
	NO_COPY(ANavigation)
	DESTRUCTOR(ANavigation)
public:
	_bool NativeConstruct(const _string& _Paths);
	// Compute Height
	const _float  ComputeHeight(const Vector3& _vPosition);
	void ComputeHeight(CSHPTRREF<ATransform> _spTransform);
	// Move
	_bool IsMove(const Vector3& _vPosition, SHPTR<ACell>& _spCell);
	_bool IsMove(_int _iCurOnCellIndex, const Vector3& _vPosition, SHPTR<ACell>& _spCell);
	// Find
	SHPTR<ACell> FindCell(const Vector3& _vPosition);
	SHPTR<ACell> FindCellWithoutUpdate(const Vector3& _vPosition);
	SHPTR<ACell> FindCell(const _int& _iIndex);
	SHPTR<ACell> FindCellWithoutUpdate(const _int& _iIndex);
	// Is Collision
	_bool IsCollision(SHPTR<ACollider>& _pCollider);
	// Insert Cell
	void AddCell(SHPTR<ACell>& _spCell);
	// SaveLoad
	_bool Load(const _string& _wstrPath);

	CSHPTR<CELLCONTAINER> GetCells() const { return m_spCellContainer; }
	const _int& GetCurIndex() const { return m_iCurIndex; }
	void SetCurIndex(const _int& _iIndex) { m_iCurIndex = _iIndex; }
	// Get Collider
	CSHPTRREF<ACollider> GetCollider() { return m_spCollider; }
	CSHPTRREF<ACell> GetCurCell() { return m_spCurCell; }
	CSHPTRREF<ACell> GetPrevCell() { return m_spPrevCell; }

	SHPTR<ACell> GetCell(const _int _index) { return (*m_spCellContainer.get())[_index]; }

	void SetCurCell(CSHPTRREF<ACell> newCell);
	// Ready Neighbor
	_bool ReadyNeighbor();

	/*
	@ Date: 2024-05-14, Writer: �̼���
	@ Explain
	-  �����̵� ���� ��� �Լ�.
	*/
	Vector3 ClampPositionToCell(const Vector3& position);

	/*
	@ Date: 2024-06-03, Writer: �̼���
	@ Explain
	-  A* �˰���
	*/
	_float Heuristic(const Vector3& a, const Vector3& b);
	ANavigation::PathFindingState StartPathFinding(const Vector3& start, const Vector3& end, CSHPTRREF<ACell> _startCell, CSHPTRREF<ACell> _destCell);
	bool StepPathFinding(PathFindingState& state);
	VECTOR<Vector3> OptimizePath(const VECTOR<SHPTR<ACell>>& path, const Vector3& start, const Vector3& end);
	bool LineTest(const Vector3& start, const Vector3& end);
private:
	virtual void Free() override;
private:
	SHPTR<CELLCONTAINER>		m_spCellContainer;
	SHPTR<ACell>							m_spCurCell;
	SHPTR<ACell>							m_spPrevCell;
	_int												m_iPrevIndex;
	_int												m_iCurIndex;
	SHPTR<ACollider>					m_spCollider;
};

END

#endif //_SERVERFRAMEWORK_CORE_PUBLIC_UNAVIGATION_H