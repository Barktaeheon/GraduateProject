#ifndef _SERVERFRAMEWORK_CORE_PUBLIC_USESSION_H
#define _SERVERFRAMEWORK_CORE_PUBLIC_USESSION_H

#include "UObject.h"


BEGIN(Core)
class UService;

/*
@ Date: 2023-01-05, Writer: ������
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

	template<class T>
	void CombineProto(REF_IN UBuffer& _Buffer, REF_IN PACKETHEAD& _PacketHead, const T& _data, short _tag)
	{
		_data.SerializePartialToArray((void*)&_Buffer.GetBuffer()[0], static_cast<int>(_data.ByteSizeLong()));
		short size = static_cast<short>(_data.ByteSizeLong());
		_PacketHead = PACKETHEAD{ size, _tag };
	}

	template<class T>
	void SendProtoData(const T& _data, short _tag)
	{
		Core::UBuffer Buffer;
		Core::PACKETHEAD PacketHead;
		CombineProto<T>(REF_OUT Buffer, REF_OUT PacketHead, _data, _tag);
		WriteData(&Buffer.GetBuffer()[0], PacketHead);
	}
public: /*Get Set */
	const SESSIONID GetID() const { return m_ID; }
	const SESSIONTYPE GetSessionType() const { return m_SessionType; }
	TCPSOCKET& GetTcpSocket(REF_RETURN) { return m_TcpSocket; }
	SHPTR<UService> GetService() const { return m_wpService.lock(); }
	const _bool IsConnect() const { return m_isConnect.load(); }

protected:
	void PacketCombine(_char* _pPacket, _llong _Size);
	virtual _bool ProcessPacket(_char* _pPacket, const PACKETHEAD& _PacketHead) PURE;
	void CombineSendBuffer(_char* _pPacket, const PACKETHEAD& _PacketHead);
	void Leave();

	void ClearSendBuff() { ZeroMemory(&m_SendBuffer, MAX_BUFFER_LENGTH); }

	BUFFER& GetSendBuff(REF_RETURN) { return m_SendBuffer; }
	MUTEX& GetLock(REF_RETURN) { return m_Lock; }
private:
	virtual void Free() override;

private:
	SESSIONID						m_ID;
	SESSIONTYPE				m_SessionType;
	TCPSOCKET					m_TcpSocket;
	ATOMIC<_llong>			m_CurBuffuerLocation;
	// Buffer ����
	TOTALBUFFER				m_TotalBuffer;
	BUFFER							m_SendBuffer;
	BUFFER							m_RecvBuffer;
	// Service
	WKPTR<UService>		m_wpService;

	MUTEX							m_Lock;
	ATOMIC<_bool>			m_isConnect;
};

END

#endif //  _SERVERFRAMEWORK_CORE_PUBLIC_USESSION_H
