#ifndef _SERVERFRAMEWORK_CORE_PUBLIC_AMYSQLDRIVER_H
#define _SERVERFRAMEWORK_CORE_PUBLIC_AMYSQLDRIVER_H

#include "ACoreBase.h"

BEGIN(Core)
class AMySqlConnector;
class AMySqlTable;
/*
@ Date: 2024-01-28, Writer: ������
@ Explain
-  DataBase�� ������ �ϱ� ���� Ŭ�����̴�. 
������ �����ͺ��̽��� mysql, redis
*/
class CORE_DLL AMySqlDriver final : public ACoreBase {
public:
	using SQLARRAY = ARRAY<SHPTR<AMySqlConnector>, TLS::MAX_WORKTHREAD>;
	using SQLTABLE = ARRAY<SHPTR<AMySqlTable>, MAX_SQLTABLE_LENGTH>;
public:
	AMySqlDriver();
	NO_COPY(AMySqlDriver)
	DESTRUCTOR(AMySqlDriver)
public:
	_bool	NativeConstruct(const _string& _strAddress, const _string& _strName, const _string& _strPassward);
private:
	virtual void Free() override;
private:
	sql::mysql::MySQL_Driver*	 m_pDriver;
	SQLARRAY									 m_MySqlConnectContainer;
	SQLTABLE									 m_MySqlTableContainer;
};

END


#endif // _SERVERFRAMEWORK_CORE_PUBLIC_AMYSQLDRIVER_H