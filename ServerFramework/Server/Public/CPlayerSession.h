#ifndef _SERVERFRAMEWORK_SERVER_PUBLIC_CPLAYERSESSION_H
#define _SERVERFRAMEWORK_SERVER_PUBLIC_CPLAYERSESSION_H

#include "USession.h"

BEGIN(Core)
class UService;
END

BEGIN(Server)
/*
@ Date: 2023-01-02, Writer: ������
@ Explain
- Client�� �����ϴ� �÷��̾��� ����ϱ� ���� Ŭ����
*/
class CACHE_ALGIN CPlayerSession final : public Core::USession {
public:
	CPlayerSession(OBJCON_CONSTRUCTOR, TCPSOCKET _TcpSocket, Core::SHPTR<Core::UService> _spService, SESSIONID _ID);
	NO_COPY(CPlayerSession)
	DESTRUCTOR(CPlayerSession)
public:
	virtual _bool Start() override;
	// Ŭ���̾�Ʈ���� ���۵� ���۸� �д� �Լ�
	virtual void ReadData() override;
	// Ŭ���̾�Ʈ���� ���۸� �����Ͽ� �����ϴ� �Լ�
	virtual _bool WriteData(_char* _pPacket, const Core::PACKETHEAD& _PacketHead) override;
	virtual void Disconnect() override;
	virtual void ConnectTcpSocket() override;
protected:
	virtual _bool ProcessPacket(_char* _pPacket, const Core::PACKETHEAD& _PacketHead) override;
private:
	virtual void Free() override;

private:

};


END
#endif // _SERVERFRAMEWORK_SERVER_PUBLIC_CPLAYERSESSION_H