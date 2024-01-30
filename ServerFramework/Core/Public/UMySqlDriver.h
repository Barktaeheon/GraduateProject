#ifndef _SERVERFRAMEWORK_CORE_PUBLIC_UMYSQLDRIVER_H
#define _SERVERFRAMEWORK_CORE_PUBLIC_UMYSQLDRIVER_H

#include "UBase.h"

BEGIN(Core)
class UMySqlConnector;
/*
@ Date: 2023-01-28, Writer: ������
@ Explain
-  DataBase�� ������ �ϱ� ���� Ŭ�����̴�. 
������ �����ͺ��̽��� mysql, redis
*/
class CORE_DLL UMySqlDriver final : public UBase {
public:
	using SQLARRAY = ARRAY<SHPTR<UMySqlConnector>, TLS::MAX_WORKTHREAD>;
public:
	UMySqlDriver();
	NO_COPY(UMySqlDriver)
	DESTRUCTOR(UMySqlDriver)
public:
	_bool	NativeConstruct(const _string& _strAddress, const _string& _strName, const _string& _strPassward);
private:
	virtual void Free() override;
private:
	sql::mysql::MySQL_Driver*	 m_pDriver;
	SQLARRAY									 m_MySqlArray;
};

END


#endif // _SERVERFRAMEWORK_CORE_PUBLIC_UMYSQLDRIVER_H