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
using CELLCONTAINER = VECTOR<SHPTR<ACell>>;

class CORE_DLL ANavigation final : public ACoreBase {
public:
	ANavigation();
	NO_COPY(ANavigation)
	DESTRUCTOR(ANavigation)
public:
	_bool NativeConstruct(const _string& _Paths);
	// ��ü�� Y���� �ش� Cell �ٷ� ���� ����� �Լ� 
	const _float ComputeHeight(const Vector3 _vPosition, REF_IN  _int& _Index);
	// �׺���̼��� ������ ������� Ȯ���ϴ� �Լ�
	_bool IsCheckOverNavigationArea(const Vector3 _vPosition, REF_IN _int& _Index);
	// Cell�� ã�� �����ϴ� �Լ�
	SHPTR<ACell>	FindCell(const Vector3 _vPosition, REF_IN _int& _Index);
	SHPTR<ACell> FindCell(const Vector3 _vPosition);
private:
	void ReadyNeighbor();
private:
	virtual void Free() override;
private:
	static constexpr	_int		FINDCELL_CNT{ 10 };
	// MaxCellCount 
	_int										m_MaxCellCount;
	CELLCONTAINER				m_CellContainer;
};

END

#endif //_SERVERFRAMEWORK_CORE_PUBLIC_UNAVIGATION_H