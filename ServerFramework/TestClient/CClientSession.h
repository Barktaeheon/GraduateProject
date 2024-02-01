#include "ASession.h"

BEGIN(Core)
class AService;
END

class  CClientSession final : public ASession {
public:
	CClientSession(SESSION_CONSTRUCTOR);
	NO_COPY(CClientSession)
	DESTRUCTOR(CClientSession)
public:
	virtual _bool Start() override;
	// Ŭ���̾�Ʈ���� ���۵� ���۸� �д� �Լ�
	virtual void ReadData() override;
	// Ŭ���̾�Ʈ���� ���۸� �����Ͽ� �����ϴ� �Լ�
	virtual _bool WriteData(_char* _pPacket, const Core::PACKETHEAD& _PacketHead) override;
	virtual void Disconnect() override;
	virtual void ConnectTcpSocket() override;

	void SendMsg();
protected:
	virtual _bool ProcessPacket(_char* _pPacket, const Core::PACKETHEAD& _PacketHead) override;
private:
	virtual void Free() override;
};

