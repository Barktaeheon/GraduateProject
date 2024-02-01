#ifndef _SERVERFRAMEWORK_CORE_PUBLIC_ACOREINSTANCE_H
#define _SERVERFRAMEWORK_CORE_PUBLIC_ACOREINSTANCE_H

#include "ACoreBase.h"

BEGIN(Core)
class AService;
class AThreadManager;
class ARandomManager;
class ALogManager;
class ASpaceManager;
class ASession;
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
	// ��ü ���� �����ڿ��� �޽����� ������ �Լ��̴�. 
	 void BroadCastMessage(_char* _pPacket, const PACKETHEAD& _PacketHead);
	// Session�� TCP Socket ���Ḹ ���� Insert�� ��Ÿ �ٸ� �Լ����� �����ؼ� Socket�� ����
	 void LeaveService(const SESSIONID _SessionID);
	// Session�� Container�� �����ϴ� �Լ��̴�. 
	 void InsertSession(SESSIONID _SessionID, SHPTR<ASession> _spSession);

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
	SHPTR<AService>						m_spService;
	SHPTR<AThreadManager>		m_spThreadManager;
	SHPTR<ARandomManager>		m_spRandomManager;
	SHPTR<ALogManager>				m_spLogManager;
	SHPTR<ASpaceManager>			m_spSpaceManager;
};

END


#endif // _SERVERFRAMEWORK_CORE_PUBLIC_ACOREINSTANCE_H

