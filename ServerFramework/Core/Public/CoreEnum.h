#ifndef _SERVERFRAMEWORK_CORE_PUBLIC_COREENUM_H
#define _SERVERFRAMEWORK_CORE_PUBLIC_COREENUM_H


namespace Core
{
	/*
	@ Date: 2024-01-09,  Writer: ������
	@ Explain:  LOGTYPE�� enum 
	*/
	enum class LOGTYPE : unsigned int
	{
		CONSOL,
		SAVE,
		// End 
		LOG_END,
	};

	namespace Sql
	{
		/*
		@ Date: 2024-03-24,  Writer: ������
		@ Explain:  MSSql�� �����ϴ� DataType, ���� �����Ϳ� �����ϴ� ��ȣ�� ��Ÿ����. 
		*/
		enum class  DATATYPE
		{
			NONE = 0,
			TINYINT = 48,
			SMALLINT = 52,
			INT = 56,
			REAL = 59,
			DATETIME = 61,
			FLOAT = 62,
			BIT = 104,
			NUMERIC = 108,
			BIGINT = 127,
			VARBINARY = 165,
			VARCHAR = 167,
			BINARY = 173,
			NVARCHAR = 231,
			DATATYPE_END
		};
		/*
		@ Date: 2024-03-24,  Writer: ������
		@ Explain:  �����͸� ��� �������� ���ϴ� enum 
		*/
		enum class INDEXTYPE
		{
			CLUSTERED = 1,
			NONCLUSTERED = 2,
			INDEX_END
		};		
		/*
		@ Date: 2024-03-24,  Writer: ������
		@ Explain:  Table�� Type 
		*/
		enum TABLETYPE
		{
			LOGIN, 
			TABLE_END
		};
		/*
		@ Date: 2024-03-31,  Writer: ������
		@ Explain:  Query Type�� �����Ѵ�. 
		*/
		enum class QUERYTYPE
		{
			INSERT, 
			UPDATE,
			REMOVE, 
			FIND, 
			QUERY_END
		};
	}

	using SQLDATATYPE = Sql::DATATYPE;
	using SQLINDEXTYPE = Sql::INDEXTYPE;
	using SQLTABLETYPE = Sql::TABLETYPE;
	using SQLQUERYTYPE = Sql::QUERYTYPE;

	enum class SESSIONTYPE : unsigned int {
		OBJECT, PLAYER, MONSTER, TYPE_END
	};

	enum class SERVICETYPE { CLIENT, SERVER, TYPE_END };

	enum {
		MAX_BUFFER_LENGTH = 512,
		MAX_PROCESSBUF_LENGTH = MAX_BUFFER_LENGTH * 5,
		MAX_OCTREENODE_LENGTH = 8,
		MAX_SQLSTRING_LENGTH = 200,
		MAX_SQLTABLE_LENGTH = (_int)(SQLTABLETYPE::TABLE_END),
	};

	/*
@ Date: 2024-02-10, Writer: ������
@ Explain
- . �ִϸ��̼� ī�װ��� �����Ѵ�.
*/
	enum class ANIMEVENTCATEGORY : _int {
		CATEGROY_SECTION,
		CATEGROY_OCCUR,
		CATEGROY_END
	};
	/*
	@ Date: 2024-02-10, Writer: ������
	@ Explain
	- �ִϸ��̼� Event�� ���� �ʿ��� ��ü�� ������
	*/
	enum ANIMEVENTTYPE : _int {
		ANIMEVENT_EFFECT,     // ����Ʈ �߻� 			
		ANIMEVENT_SOUND,     // �Ҹ� �߻� 
		ANIMEVENT_COLLIDER,	  // �ݶ��̴� �߻�
		ANIMEVENT_CAMERA,    // ī�޶� ����ٴ�
		ANIMEVENT_OBJACTIVE, // OBJȰ��ȭ
		ANIMEVENT_ANIMCHANGESBETWEEN,
		ANIMEVENT_ANIMOCCURSTIMEPASS,
		ANIMEVENT_END
	};

	enum EVENT_TYPE : _int
	{ 
		// MOB
		EV_MOB_ACTIVE,
		EV_MOB_FIND, 
		EV_MOB_ATTACK,
		EV_MOB_DAMAGED, 
		EV_MOB_DEAD,
		// DB
		EV_DB_CHNAGE,
		EV_DB_GET, 
		EV_DB_DELETE,
		EV_END 
	};

	enum MONSTERSTATE : _int
	{
		MOB_DISABLE_STATE,
		MOB_IDLE_STATE,
		MOB_MOVE_STATE,
		MOB_ATTACK_STATE ,
		MOB_DEATH_STATE ,
		MOB_HIT_STATE ,
		MOB_SLEEP_STATE ,
		MOB_AWAKE_STATE,
		MOB_FIND_STATE,
		MOB_FIRSTFIND_STATE,
		MOB_TAUNT_STATE
	};

	enum
	{
		ASTAR_NAV_INDEX = 20,
		EXTRA_NAV_COUNT = 1,
	};
}



#endif // _SERVERFRAMEWORK_CORE_PUBLIC_COREENUM_H