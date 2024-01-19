#include "CoreDefines.h"
#include "UPathFinder.h"
#include "UNavigation.h"
#include "UCell.h"

namespace Core {

	UPathFinder::UPathFinder() : 
		m_MaxCellCount{0}, m_CheckOpens{}, m_DistanceWeights{}
		,m_VisitedListes{}, m_BestListes{}
	{
	}

	_bool UPathFinder::NativeConstruct(const VECTOR<SHPTR<UCell>>& _Cells)
	{
		// Cells
		m_MaxCellCount = static_cast<_int>(_Cells.size());
		m_ParentsCellNodes.resize(m_MaxCellCount);
		// value 
		for (_int i = 0; i < TLS::MAX_THREAD; ++i)
		{
			m_CheckOpens.resize(m_MaxCellCount);
			m_DistanceWeights.resize(m_MaxCellCount);
		}
		return true;
	}

	void UPathFinder::FindPath(SHPTR<UNavigation> _spNavigation, Vector3 _vStartPos, Vector3 _vEndPos)
	{
		RETURN_CHECK(nullptr == _spNavigation, ;);
		Release();
		SHPTR<UCell> spStartCell = _spNavigation->FindCell(_vStartPos);
		SHPTR<UCell> spEndCell = _spNavigation->FindCell(_vEndPos);
		RETURN_CHECK(nullptr == spStartCell || nullptr == spEndCell, ;);
		MakeRoutine(spStartCell, spEndCell);
	}

	void UPathFinder::MakeRoutine(SHPTR<UCell> _spStartCell, SHPTR<UCell> _spEndCell)
	{
		UCell* pCell = _spStartCell.get();
		m_CheckOpens[pCell->GetIndex()] = VISITED_VALUE;
		// Find Size��ŭ�� ã�� ��Ʈ�� ã�� �ʴ´�(����ȭ�� ����)
		for (_int i = 0; i < FIND_SIZE; ++i)
		{
			_int Index = pCell->GetIndex();
			// ���� �湮�ߴ� ��尡 ������� ������ ����
			if (false == m_VisitedListes.empty())
			{
				m_VisitedListes.pop_front();
			}

			for (auto& CheckCell : pCell->GetNeighborCelles())
			{
				_int CheckIndex = CheckCell->GetIndex();
				if (VISITED_VALUE == m_CheckOpens[CheckIndex])
					continue;

				m_ParentsCellNodes[CheckIndex] = CheckCell;
				if (_spEndCell.get()  == CheckCell)
				{
					MakeBestRoutine(_spStartCell.get(), CheckCell);
					return;
				}
				// �ڽ�Ʈ�� ���� 
				_float Cost = ComputeDistCost(_spStartCell->GetCenterPos(), _spEndCell->GetCenterPos(), 
					CheckCell->GetCenterPos(), pCell->GetCenterPos());
				// CheckOpen 
				m_CheckOpens[CheckCell->GetIndex()] = VISITED_VALUE;
				// �湮�� ��忡 ���� �ִ´�. 
				m_VisitedListes.push_back(CheckCell);
			}
			if (0 < m_VisitedListes.size())
			{
				VECTOR<_float>& DistanceWeights = m_DistanceWeights;
				m_VisitedListes.sort([&DistanceWeights]( UCell* _visited1, UCell* _visited2) {
					_int Index1 = _visited1->GetIndex();
					_int Index2 = _visited2->GetIndex();
					return DistanceWeights[Index1] < DistanceWeights[Index2];
					});

				pCell =  m_VisitedListes.front();
			}
			else
			{
				break;
			}
		}
		MakeBestRoutine(_spStartCell.get(), pCell);
	}

	void UPathFinder::MakeBestRoutine(UCell* _pStartCell, UCell* _pEndCell)
	{
		UCell* pCell = _pEndCell;
		while (true)
		{
			RETURN_CHECK(nullptr == pCell, ;);
			m_BestListes.push_back(pCell);
			_int index = pCell->GetIndex();
			pCell = m_ParentsCellNodes[index];
			RETURN_CHECK(_pStartCell == pCell, ;);
		}
	}

	void UPathFinder::Release()
	{
		// value release
		MemoryInitialization(&m_CheckOpens[0], m_MaxCellCount);
		MemoryInitialization(&m_DistanceWeights[0], m_MaxCellCount);

		m_VisitedListes.clear();
		m_BestListes.clear();
	}

	_float UPathFinder::ComputeDistCost(const Vector3 _vStart, const Vector3 _vEnd, 
		const Vector3 _vVisited, const Vector3 _vCur)
	{
		// ���������� �湮������ �Ÿ� 
		Vector3 vCostF = _vVisited - _vStart;
		// �湮������ ��ǥ���� �Ÿ�
		Vector3 vCostG = _vVisited - _vEnd;
		// �湮������ ���� �ִ� Ÿ�ϱ��� �Ÿ�
		Vector3 vCostT = _vVisited - _vCur;
		/* YPos */
		_float PosYF = vCostF.y; _float PosGF = vCostG.y; _float PosTF = vCostT.y;
		vCostF.y = 0; vCostG.y = 0; vCostT.y = 0;
		/* Weight */
		return glm::length2(vCostF) + glm::length2(vCostG) + glm::length2(vCostT) + PosYF + PosGF + PosTF;
	}

	void UPathFinder::Free()
	{
	}
}