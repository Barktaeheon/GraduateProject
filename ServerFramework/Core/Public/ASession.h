#ifndef _SERVERFRAMEWORK_CORE_PUBLIC_ASESSION_H
#define _SERVERFRAMEWORK_CORE_PUBLIC_ASESSION_H

#include "APawn.h"


BEGIN(Core)
class AService;
class ATransform;
class ACollider;
class ASpace;

/*
@ Date: 2024-01-22, Writer: ������
@ Explain
- Ŭ���̾�Ʈ�� ����ϱ� ���� Session Ŭ�����̰� �����Լ��� Server �ַ�ǿ��� �ش� Ŭ������ �θ�� �ڽ� Ŭ������ 
�������־�� �Ѵ�.
*/
class CORE_DLL ASession abstract : public APawn {
public:
	using BUFFER = ARRAY<_char, MAX_BUFFER_LENGTH>;
	using TOTALBUFFER = ARRAY<_char, MAX_PROCESSBUF_LENGTH>;
public:
	ASession(OBJCON_CONSTRUCTOR, TCPSOCKET _TcpSocket, SESSIONID _ID, SESSIONTYPE _SessionType);
	DESTRUCTOR(ASession)
public: 
	virtual _bool Start(const VOIDDATAS& _ReceiveDatas = {}) PURE;
	// Ŭ���̾�Ʈ���� ���۵� ���۸� �д� �Լ�
	virtual void RecvData() PURE;
	// Ŭ���̾�Ʈ���� ���۸� �����Ͽ� �����ϴ� �Լ�
	virtual _bool SendData(_char* _pPacket, const PACKETHEAD& _PacketHead) PURE;
	virtual void Disconnect() PURE;
	virtual void ConnectTcpSocket() PURE;
	/*
	@ Date: 2024-01-05, Writer: ������
	@ Explain
	- ProtocolBuffer�� TCP�� ����� ��뿡�� ������ ���� ���ø� �Լ��̴�. 
	*/
	template<class T>
	requires CheckProtoType<T>
	void SendProtoData(BUFFER& _buffer, const T& _data, short _tag)
	{
		Core::PACKETHEAD PacketHead;
		CombineProto<T>(REF_OUT _buffer, REF_OUT PacketHead, _data, _tag);
		SendData(&_buffer[0], PacketHead);
	}
	void ResetKeyState() { this->m_KeyState = 0; }
public: /*Get Set */
	TCPSOCKET& GetTcpSocket(REF_RETURN) { return m_TcpSocket; }
	const _int GetKeyState() const { return m_KeyState; }
protected:
	virtual _bool ProcessPacket(_char* _pPacket, const PACKETHEAD& _PacketHead) PURE;
	void PacketCombine(_char* _pPacket, _llong _Size);
	void CombineSendBuffer(_char* _pPacket, const PACKETHEAD& _PacketHead);
	void Leave();
protected: /* get set*/
	BUFFER& GetSendBuff(REF_RETURN) { return m_SendBuffer; }
	void SetKeyState(_int _KeyState) { this->m_KeyState = _KeyState; }
private:
	virtual void Free() override;

private:
	TCPSOCKET					m_TcpSocket;
	_llong								m_CurBuffuerLocation;
	// Buffer ����
	TOTALBUFFER				m_TotalBuffer;
	BUFFER							m_RecvBuffer;
	// SendBuffer
	BUFFER							m_SendBuffer;
	// KeyState
	ATOMIC<_int>				m_KeyState;
};

END

#endif //  _SERVERFRAMEWORK_CORE_PUBLIC_ASESSION_H
