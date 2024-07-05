#ifndef _SERVERFRAMEWORK_CORE_PUBLIC_ACOREINSTANCE_H
#define _SERVERFRAMEWORK_CORE_PUBLIC_ACOREINSTANCE_H

#include "ACoreBase.h"

BEGIN(Core)
class AService;
class AThreadManager;
class ARandomManager;
class ALogger;
class ASpaceManager;
class ASession;
class AMySqlDriver;
class AGameObject;
class ANavigation;
class APathFinder;
class ATransform;
class ACell;
class AJobTimer;
class AMonster;
class AServerService;

using NAVIGATIONWORKBENCH = ARRAY<SHPTR<ANavigation>, TLS::MAX_WORKTHREAD + EXTRA_NAV_COUNT>;

using SESSIONCONTAINER = CONUNORMAP<SESSIONID, SHPTR<ASession>>;
using MOBOBJCONTAINER = CONUNORMAP<SESSIONID, SHPTR<AMonster>>;
/*
@ Date: 2024-01-23
@ Writer: ������
@ Explain
- ����, �Ҹ��ڸ� �̿��ؼ� ������ ����� ��, �޸𸮰� ����� �����ǰ� �ϱ� ���� Ŭ�����̴�. 
������ �� ���� �����Ѵ�. 
*/
class CORE_DLL ACoreInstance : public ACoreBase {
public:
	ACoreInstance();
	NO_COPY(ACoreInstance)
	DESTRUCTOR(ACoreInstance)
public:
	void ReadyCoreInstance(SHPTR<AService> _spService);
	_bool Start();
public: /* Service */
	// ID�� ���ؼ� SessionID
	 SHPTR<ASession> FindSession(const SESSIONID _SessionID);
	 // ID�� ���ؼ� GameObject�� ã�ƿ´�. 
	 SHPTR<AMonster> FindMobObject(const SESSIONID _SessionID);
	// ��ü ���� �����ڿ��� �޽����� ������ �Լ��̴�. 
	 void BroadCastMessage(_char* _pPacket, const PACKETHEAD& _PacketHead);
	 // �ش� Session ID�� ������ ��ü ���� �����ڿ��� �޽����� ������ �Լ�
	 void BroadCastMessageExcludingSession(const SESSIONID _SessionID, _char* _pPacket, const PACKETHEAD& _PacketHead);
	 // Direct�� �޽��� ������ �Լ�
	 void DirectSendMessage(const SESSIONID _SessionID, _char* _pPacket, const PACKETHEAD& _PacketHead);
	// Session�� TCP Socket ���Ḹ ���� Insert�� ��Ÿ �ٸ� �Լ����� �����ؼ� Socket�� ����
	 void LeaveService(const SESSIONID _SessionID);
	// Session�� Container�� �����ϴ� �Լ��̴�. 
	 void InsertSession(SESSIONID _SessionID, SHPTR<ASession> _spSession);
	 // GameObject�� ����ִ� �Լ�
	 void InsertMobObject(SESSIONID _SessionID, SHPTR<AMonster> _spMobObject);
	 // Get 
	 const SESSIONCONTAINER& GetSessionContainer() const;
	 const MOBOBJCONTAINER& GetMobObjContainer() const;

	 SHPTR<AServerService> GetServerService();
public: /* ThreadManager */
	void RegisterFunc(const THREADFUNC& _CallBack, void* _Data);
	void RegisterJob(_int _jobType, CSHPTRREF<AJobTimer> _spJobTimer);
	SHPTR<AJobTimer> FindJobTimer(_int _JobTimer);
	void Join();
public: /* RandomManager */
	_int		ReturnRadomNumber(const _int _iMinNum, const _int _iMaxNum);
	_int		ReturnRadomNumber(const _int _iMaxNum);
public: /* SpaceManager */
	void BuildGameSpace(const SPACEINFO& _SpaceInfo);
public: /* MySqlDriver */
	// ExcuteQueryMessage
	_bool ExcuteQueryMessage(SQLTABLETYPE _TableType, SQLQUERYTYPE _sqlQueryType, const _string& _strQueryData);

	void BindParam(SQLTABLETYPE _TableType, _int _ParamIndex, _bool _Value);
	void BindParam(SQLTABLETYPE _TableType, _int _ParamIndex, _float _Value);
	void BindParam(SQLTABLETYPE _TableType, _int _ParamIndex, _double _Value);
	void BindParam(SQLTABLETYPE _TableType, _int _ParamIndex, _short _Value);
	void BindParam(SQLTABLETYPE _TableType, _int _ParamIndex, _int _Value);
	void BindParam(SQLTABLETYPE _TableType, _int _ParamIndex, _llong _Value);
	void BindParam(SQLTABLETYPE _TableType, _int _ParamIndex, const _string& _Value);
public: /* Navigation*/
	// Compute Height
	SHPTR<ANavigation> GetNavigation(_int _index = TLS::g_ThreadID) const { return m_NavigationWorkBench[_index]; }
private:
	virtual void Free() override;
private:
	MUTEX											m_Mutex;
	SHPTR<AService>						m_spService;
	SHPTR<AThreadManager>		m_spThreadManager;
	SHPTR<ARandomManager>		m_spRandomManager;
	SHPTR<ASpaceManager>			m_spSpaceManager;
	SHPTR< AMySqlDriver>				m_spMySqlDriver;

	NAVIGATIONWORKBENCH			m_NavigationWorkBench;
};

END


#endif // _SERVERFRAMEWORK_CORE_PUBLIC_ACOREINSTANCE_H

