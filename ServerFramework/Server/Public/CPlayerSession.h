#ifndef _SERVERFRAMEWORK_SERVER_PUBLIC_CPLAYERSESSION_H
#define _SERVERFRAMEWORK_SERVER_PUBLIC_CPLAYERSESSION_H

#include "ASession.h"

BEGIN(Core)
class AService;
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
	_uint a{ 0 };
};


END
#endif // _SERVERFRAMEWORK_SERVER_PUBLIC_CPLAYERSESSION_H