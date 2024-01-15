#ifndef _SERVERFRAMEWORK_CORE_PUBLIC_UNAVIGATION_H
#define _SERVERFRAMEWORK_CORE_PUBLIC_UNAVIGATION_H

#include "UBase.h"

BEGIN(Core)
/*
@ Date: 2023-01-14, Writer: ������
@ Explain
-  �ش� �� ���� ���� �ʴ��� �Ǵ��ϰ�, ���� ���ϴ� ��ġ������ ã���� Ŭ����
 Index ���� ��� ���� ���� �ƴϹǷ� datarace�� �Ͼ ���� �������� �ʴ´�. �׷��� atomic�� ���� �ʴ´�.
*/
class UCell;
using CELLCONTAINER = VECTOR<SHPTR<UCell>>;

class CACHE_ALGIN_CORE_DLL UNavigation final : public UBase {
public:
	UNavigation();
	NO_COPY(UNavigation)
	DESTRUCTOR(UNavigation)
public:
	_bool NativeConstruct(const _string& _Paths);
	// ��ü�� Y���� �ش� Cell �ٷ� ���� ����� �Լ� 
	const _float ComputeHeight(const _float3 _vPosition, REF_IN  _int& _Index);
	// �׺���̼��� ������ ������� Ȯ���ϴ� �Լ�
	_bool IsCheckOverNavigationArea(const _float3 _vPosition, REF_IN _int& _Index);
	// Cell�� ã�� �����ϴ� �Լ�
	SHPTR<UCell>	FindCell(const _float3 _vPosition, REF_IN _int& _Index);
	SHPTR<UCell> FindCell(const _float3 _vPosition);
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