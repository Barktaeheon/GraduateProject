#ifndef _SERVERFRAMEWORK_SERVER_PUBLIC_CPLAYERSESSION_H
#define _SERVERFRAMEWORK_SERVER_PUBLIC_CPLAYERSESSION_H

#include "ASession.h"

BEGIN(Core)
class AService;
class ACoreInstance;
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
	// Ŭ���̾�Ʈ���� ���۵� ���۸� �д� �Լ�
	virtual void RecvData() override;
	// Ŭ���̾�Ʈ���� ���۸� �����Ͽ� �����ϴ� �Լ�
	virtual _bool SendData(_char* _pPacket, const Core::PACKETHEAD& _PacketHead) override;
	virtual void Disconnect() override;
	virtual void ConnectTcpSocket() override;
protected:
	virtual _bool ProcessPacket(_char* _pPacket, const Core::PACKETHEAD& _PacketHead) override;
private: 
	void LoginState(SHPTR<ACoreInstance> _spCoreInstance, SESSIONID _SessionID, _char* _pPacket, const Core::PACKETHEAD& _PacketHead);
	void MoveState(SHPTR<ACoreInstance> _spCoreInstance, SESSIONID _SessionID, _char* _pPacket, const Core::PACKETHEAD& _PacketHead);
	void PlayerState(SHPTR<ACoreInstance> _spCoreInstance, SESSIONID _SessionID, _char* _pPacket, const Core::PACKETHEAD& _PacketHead);
private:
	virtual void Free() override;
private:
	_int						m_iStartCellIndex;
	_int						m_iWComboStack;
	_int						m_iSComboStack;
};


END
#endif // _SERVERFRAMEWORK_SERVER_PUBLIC_CPLAYERSESSION_H