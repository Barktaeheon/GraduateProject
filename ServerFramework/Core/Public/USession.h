#ifndef _SERVERFRAMEWORK_CORE_PUBLIC_USESSION_H
#define _SERVERFRAMEWORK_CORE_PUBLIC_USESSION_H

#include "UBase.h"


BEGIN(Core)

/*
@ Date: 2023-12-31
@ Writer: ������
@ Explain
- Ŭ���̾�Ʈ�� ����ϱ� ���� Session Ŭ�����̰� �����Լ��� Server �ַ�ǿ��� �ش� Ŭ������ �θ�� �ڽ� Ŭ������ 
�������־�� �Ѵ�.
*/
class CACHE_ALGIN_CORE_DLL USession abstract : public UBase  {
	using BUFFER = ARRAY<_char, MAX_BUFFER_LENGTH>;
	using TOTALBUFFER = ARRAY<_char, MAX_PROCESSBUF_LENGTH>;
	using TcpSocket = Asio::ip::tcp::socket;
public:
	USession(MOVE TcpSocket&& _TcpSocket, SESSIONTYPE _SessionType, ID _ID);
	NO_COPY(USession)
	DESTRUCTOR(USession)
public: 
	virtual _bool Start() PURE;
	// Ŭ���̾�Ʈ���� ���۵� ���۸� �д� �Լ�
	_bool ReadData();
	// Ŭ���̾�Ʈ���� ���۸� �����Ͽ� �����ϴ� �Լ�
	_bool WriteData(_char* _Packet, const PACKETHEAD& _PacketHead);
	virtual void End() PURE;
public: /*Get Set */
	const ID GetID() const { return m_ID; }
	const SESSIONTYPE GetSessionType() const { return m_SessionType; }

protected:
	void PacketCombine(MOVE UBuffer&& _Buffer);
	virtual _bool ProcessPacket(_char* _Packet, const PACKETHEAD& _PacketHead) PURE;
	void CombineSendBuffer(REF_OUT BUFFER& _SendBuffer, _char* _Packet, const PACKETHEAD& _PacketHead);
private:
	virtual void Free() override;


private:
	ID							m_ID;
	SESSIONTYPE	m_SessionType;
	TcpSocket			m_TcpSocket;
	_int						m_CurBuffuerLocation;
	// Buffer ����
	TOTALBUFFER	m_TotalBuffer;
	BUFFER				m_RecvBuffer;
	BUFFER				m_SendBuffer;
};

END

#endif //  _SERVERFRAMEWORK_CORE_PUBLIC_USESSION_H
