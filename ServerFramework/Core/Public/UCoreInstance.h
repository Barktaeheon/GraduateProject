#ifndef _SERVERFRAMEWORK_CORE_PUBLIC_UCOREINSTANCE_H
#define _SERVERFRAMEWORK_CORE_PUBLIC_UCOREINSTANCE_H

#include "UBase.h"

BEGIN(Core)
class UService;
class UThreadManager;
class URandomManager;
class ULogManager;
class USpaceManager;
class USession;
/*
@ Date: 2024-01-23
@ Writer: ������
@ Explain
- ����, �Ҹ��ڸ� �̿��ؼ� ������ ����� ��, �޸𸮰� ����� �����ǰ� �ϱ� ���� Ŭ�����̴�. 
������ �� ���� �����Ѵ�. 
*/
class CORE_DLL UCoreInstance : public UBase {
public:
	UCoreInstance();
	NO_COPY(UCoreInstance)
	DESTRUCTOR(UCoreInstance)
public:
	void ReadyCoreInstance(SHPTR<UService> _spService);
	_bool Start();
public: /* Service */
	// ID�� ���ؼ� SessionID
	 SHPTR<USession> FindSession(const SESSIONID _SessionID);
	// ��ü ���� �����ڿ��� �޽����� ������ �Լ��̴�. 
	 void BroadCastMessage(_char* _pPacket, const PACKETHEAD& _PacketHead);
	// Session�� TCP Socket ���Ḹ ���� Insert�� ��Ÿ �ٸ� �Լ����� �����ؼ� Socket�� ����
	 void LeaveService(const SESSIONID _SessionID);
	// Session�� Container�� �����ϴ� �Լ��̴�. 
	 void InsertSession(SESSIONID _SessionID, SHPTR<USession> _spSession);

public: /* ThreadManager */
	void RegisterFunc(const THREADFUNC& _CallBack, void* _Data);
	void Join();
public: /* RandomManager */
	_int		ReturnRadomNumber(const _int _iMinNum, const _int _iMaxNum);
	_int		ReturnRadomNumber(const _int _iMaxNum);
public: /* LogManager */
	void PrintOut(const char* _fmt, ...);
	void FileOut(const char* _fmt, ...);
public: /* SpaceManager */
	void BuildGameSpace(const SPACEINFO& _SpaceInfo);

private:
	virtual void Free() override;
private:
	MUTEX											m_Mutex;
	SHPTR<UService>						m_spService;
	SHPTR<UThreadManager>		m_spThreadManager;
	SHPTR<URandomManager>		m_spRandomManager;
	SHPTR<ULogManager>				m_spLogManager;
	SHPTR<USpaceManager>			m_spSpaceManager;
};

END


#endif 

