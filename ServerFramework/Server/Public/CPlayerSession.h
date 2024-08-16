#ifndef _SERVERFRAMEWORK_SERVER_PUBLIC_CPLAYERSESSION_H
#define _SERVERFRAMEWORK_SERVER_PUBLIC_CPLAYERSESSION_H

#include "ASession.h"

BEGIN(Core)
class AService;
class ACoreInstance;
class AGameTimer;
END

BEGIN(Server)
/*
@ Date: 2023-01-02, Writer: ������
@ Explain
- Client�� �����ϴ� �÷��̾��� ����ϱ� ���� Ŭ����
*/
class  CPlayerSession final : public Core::ASession {
public:
	CPlayerSession(SESSION_CONSTRUCTOR);
	NO_COPY(CPlayerSession)
	DESTRUCTOR(CPlayerSession)
public:
	virtual _bool Start(const VOIDDATAS& _ReceiveDatas = {}) override;
	virtual void Tick(const _double& _dTimeDelta) override;
	// Ŭ���̾�Ʈ���� ���۵� ���۸� �д� �Լ�
	virtual void RecvData() override;
	// Ŭ���̾�Ʈ���� ���۸� �����Ͽ� �����ϴ� �Լ�
	virtual _bool SendData(_char* _pPacket, const Core::PACKETHEAD& _PacketHead) override;
	virtual void Disconnect() override;
	virtual void ConnectTcpSocket() override;
	virtual void Collision(AGameObject* _pGameObject, const _double& _dTimeDelta) override;
protected:
	virtual _bool ProcessPacket(_char* _pPacket, const Core::PACKETHEAD& _PacketHead) override;
private: 
	void LoginState(SHPTR<ACoreInstance> _spCoreInstance, SESSIONID _SessionID, _char* _pPacket, const Core::PACKETHEAD& _PacketHead);
	void PlayerState(SHPTR<ACoreInstance> _spCoreInstance, SESSIONID _SessionID, _char* _pPacket, const Core::PACKETHEAD& _PacketHead);
	void PlayerCollisionState(SHPTR<ACoreInstance> _spCoreInstance, SESSIONID _SessionID, _char* _pPacket, const Core::PACKETHEAD& _PacketHead);
	void MonsterCollisionState(SHPTR<ACoreInstance> _spCoreInstance, SESSIONID _SessionID, _char* _pPacket, const Core::PACKETHEAD& _PacketHead);
	void PressKeyState(SHPTR<ACoreInstance> _spCoreInstance, SESSIONID _SessionID, _char* _pPacket, const Core::PACKETHEAD& _PacketHead);
private:
	virtual void Free() override;
private:
	_int											m_iStartCellIndex;
	SHPTR<AGameTimer>		m_spGameTimer;
};


END
#endif // _SERVERFRAMEWORK_SERVER_PUBLIC_CPLAYERSESSION_H