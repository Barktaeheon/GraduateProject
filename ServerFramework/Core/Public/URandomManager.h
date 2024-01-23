#ifndef _SERVERFRAMEWORK_CORE_PUBLIC_URANDOMMANAGER_H
#define _SERVERFRAMEWORK_CORE_PUBLIC_URANDOMMANAGER_H

#include "UBase.h"

BEGIN(Core)
/*
@ Date: 2023-01-22, Writer: ������
@ Explain
- Random�� ������ �����ϴ� Ŭ�����̴�. 
*/
class  URandomManager final : public UBase {
public:
	URandomManager();
	NO_COPY(URandomManager)
	DESTRUCTOR(URandomManager)
public:
	_int		ReturnRadomNumber(const _int _iMinNum, const _int _iMaxNum);
	_int		ReturnRadomNumber(const _int _iMaxNum);
private:
	virtual void Free() override;
private:
	std::uniform_int_distribution<_int>			m_UniformDistribution;
	std::default_random_engine						m_dre;
};

END

#endif // _SERVERFRAMEWORK_CORE_PUBLIC_URANDOMMANAGER_H