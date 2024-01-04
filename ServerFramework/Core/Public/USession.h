#ifndef _SERVERFRAMEWORK_CORE_PUBLIC_USESSION_H
#define _SERVERFRAMEWORK_CORE_PUBLIC_USESSION_H

#include "UObject.h"


BEGIN(Core)
class UService;

/*
@ Date: 2023-01-04, Writer: ������
@ Explain
- Ŭ���̾�Ʈ�� ����ϱ� ���� Session Ŭ�����̰� �����Լ��� Server �ַ�ǿ��� �ش� Ŭ������ �θ�� �ڽ� Ŭ������ 
�������־�� �Ѵ�.
*/
class CACHE_ALGIN_CORE_DLL USession abstract : public UObject  {
public:
	using BUFFER = ARRAY<_char, MAX_BUFFER_LENGTH>;
	using TOTALBUFFER = ARRAY<_char, MAX_PROCESSBUF_LENGTH>;
public:
	USession(OBJCON_CONSTRUCTOR, MOVE TCPSOCKET _TcpSocket, SHPTR<UService> _spService,  SESSIONID _ID, SESSIONTYPE _SessionType);
	DESTRUCTOR(USession)
public: 
	virtual _bool Start() PURE;
	// Ŭ���̾�Ʈ���� ���۵� ���۸� �д� �Լ�
	virtual void ReadData() PURE;
	// Ŭ���̾�Ʈ���� ���۸� �����Ͽ� �����ϴ� �Լ�
	virtual _bool WriteData(_char* _pPacket, const PACKETHEAD& _PacketHead) PURE;
	virtual void Disconnect() PURE;
	virtual void ConnectTcpSocket() PURE;
public: /*Get Set */
	const SESSIONID GetID() const { return m_ID; }
	const SESSIONTYPE GetSessionType() const { return m_SessionType; }
	TCPSOCKET& GetTcpSocket(REF_RETURN) { return m_TcpSocket; }
	SHPTR<UService> GetService() const { return m_wpService.lock(); }

protected:
	void PacketCombine(_char* _pPacket, _llong _Size);
	virtual _bool ProcessPacket(_char* _pPacket, const PACKETHEAD& _PacketHead) PURE;
	void CombineSendBuffer(_char* _pPacket, const PACKETHEAD& _PacketHead);
	void Leave();

	void ClearSendBuff() { ZeroMemory(&m_SendBuffer, MAX_BUFFER_LENGTH); }
	void ClearRecvBuff() { ZeroMemory(&m_RecvBuffer, MAX_BUFFER_LENGTH); }

	template<class T>
	void CombineProto(REF_OUT UBuffer& _Buffer, REF_OUT PACKETHEAD& _PacketHead, T _data, short _tag)
	{
		_data.SerializePartialToArray((void*)&_Buffer.GetBuffer()[0], static_cast<int>(_data.ByteSizeLong()));
		short size = static_cast<short>(_data.ByteSizeLong());
		_PacketHead = PACKETHEAD{ size, _tag };
	}

	BUFFER& GetRecvBuff(REF_RETURN) { return m_RecvBuffer; }
	BUFFER& GetSendBuff(REF_RETURN) { return m_SendBuffer; }
private:
	virtual void Free() override;

private:
	SESSIONID						m_ID;
	SESSIONTYPE				m_SessionType;
	TCPSOCKET					m_TcpSocket;
	ATOMIC<_llong>			m_CurBuffuerLocation;
	// Buffer ����
	TOTALBUFFER				m_TotalBuffer;
	BUFFER							m_RecvBuffer;
	BUFFER							m_SendBuffer;
	// Service
	WKPTR<UService>		m_wpService;

	URWLock						m_BufferLocking;
};

END

#endif //  _SERVERFRAMEWORK_CORE_PUBLIC_USESSION_H
