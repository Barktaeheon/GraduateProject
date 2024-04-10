#ifndef _SERVERFRAMEWORK_CORE_PUBLIC_DBGROUP_H
#define _SERVERFRAMEWORK_CORE_PUBLIC_DBGROUP_H


BEGIN(Core)

USING_SHARED_PTR(USqlColumn);
USING_SHARED_PTR(UIndex);

//USING_SHARED_PTR(Table);
//USING_SHARED_PTR(Procedure);

/*
@ Date: 2024-03-25, Writer: ������
@ Explain: Sql�� �����ִ� �Լ� ����
*/
class ASqlHelpers
{
public:
	static _string		Format(const _char* _format, ...);
	static _string		DataType2String(SQLDATATYPE _type);
	static _string		RemoveWhiteSpace(const _string& _str);
	static SQLDATATYPE		String2DataType(const _char* _str, OUT _int& _maxLen);
};

/*
=========================
ASqlHelpers
=========================
AQueryFunc
=========================
*/

class AQueryFunc {
public:
	/*
	Sql�� Procedure�� ����� �Լ�
	*/
	static _bool MakeProcedureFunc(sql::Statement* _pStatement, 
		const _char* _pProcedureName, const _char* _pDefineFunc);
	/*
	Procedure�Լ��� �����ϴ� �Լ� 
	*/
	static sql::PreparedStatement* ExcuteProcedureFunc(CSHPTRREF<class AMySqlConnector> _spConnector, 
		const _char* _pProcedureName, _int _MethodLength);
};

END

#endif // _SERVERFRAMEWORK_CORE_PUBLIC_DBGROUP_H