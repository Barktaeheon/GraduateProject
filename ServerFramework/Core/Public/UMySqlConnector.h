#ifndef _SERVERFRAMEWORK_CORE_PUBLIC_UMYSQLCONNECTOR_H
#define _SERVERFRAMEWORK_CORE_PUBLIC_UMYSQLCONNECTOR_H

#include "UBase.h"

BEGIN(Core)
class UMySqlDriver;

/*
@ Date: 2023-01-28, Writer: ������
@ Explain
-  MySqlConnector�� Connector�� ������ ���� ���� Ŭ����
�׳� connector ��ü�� Lapping �� ���� ��
*/
class CORE_DLL UMySqlConnector final : public UBase {
public:
	UMySqlConnector();
	NO_COPY(UMySqlConnector)
	DESTRUCTOR(UMySqlConnector)
public:
	_bool NativeConstruct(sql::mysql::MySQL_Driver* _pDriver, const _string& _strAddress, const _string& _strName, 
		const _string& _strPassward);

	sql::Statement* MakeStatement();
	sql::PreparedStatement* MakePrepareStatement(const _string& _strStateString);
private:
	virtual void Free() override;
private:
	sql::Connection*	m_pConnection;
};

END

#endif // _SERVERFRAMEWORK_CORE_PUBLIC_UMYSQLCONNECTOR_H