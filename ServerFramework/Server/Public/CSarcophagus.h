#ifndef _SERVERFRAMEWORK_SERVER_PUBLIC_CSARCOPHAGUS_H
#define _SERVERFRAMEWORK_SERVER_PUBLIC_CSARCOPHAGUS_H

#include "AMonster.h"

BEGIN(Server)
class CMummy;

class CSarcophagus final : public AMonster {
public:
	CSarcophagus(OBJCON_CONSTRUCTOR, SESSIONID _ID, SARCOPHAGUSTYPE _eSarcophagusType, SHPTR<AJobTimer> _spMonsterJobTimer);
	DESTRUCTOR(CSarcophagus)
public:
	virtual _bool Start(const VOIDDATAS& _ReceiveDatas = {}) override;
	virtual void Tick(const _double& _dTimeDelta) override;
	virtual void State(SHPTR<ASession> _spSession, _int _MonsterState = 0) override;
	virtual bool IsHit(APawn* _pPawn, const _double& _dTimeDelta) override;
protected:
	// Damaged
	virtual void Collision(APawn* _pPawn, const _double& _dTimeDelta) override;
	// ���������� �ش� ������Ʈ�� ������� �ʵ��� ������ ��� ���� �޽��� 
	virtual void LastBehavior() override;
private:
	virtual void Free() override;
private:
	SARCOPHAGUSTYPE		m_eSarcophagusType;
	_bool									m_isInitStart;
};

END

#endif // _SERVERFRAMEWORK_SERVER_PUBLIC_CSARCOPHAGUS_H