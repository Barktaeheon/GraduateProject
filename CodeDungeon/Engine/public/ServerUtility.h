#pragma once

#include "UBase.h"

BEGIN(Engine)

using BUFFER = ARRAY < _char, MAX_BUFFER_LENGTH>;

enum COMP_TYPE { OP_TCP_SEND, OP_UDP_SEND, OP_SEND_END , OP_TCP_RECV, OP_UDP_RECV};

enum
{
	SERVER_END = 99999
};

class UOverExp;
class UNetworkAddress;
/*
@ Date: 2024-02-02, Writer: ������
@ Explain
- Server�� �ʿ��� �޼ҵ�� ����
*/
class UServerMethods {
public:
	// wsa�� ���ؼ� ������ ������ �غ� �ϴ� �Լ� 
	static _bool ReadyConnectToServer(WSADATA* _pWsaData);
	static HANDLE CreateIocpHandle();
	static void RegisterIocpToSocket(const SOCKET& _Socket, const HANDLE& _IocpHandle);
	static SOCKET CreateTcpSocket();
	static SOCKET CreateUdpSocket();
	static _int RecvTcpPacket(const SOCKET& _Socket, REF_IN UOverExp& _OverExp, REF_IN size_t& _recvBytes);
	static void SendTcpPacket(const SOCKET& _Socket, REF_IN UOverExp& _OverExp);
	static void StartNonBlocking(const SOCKET& _Socket);
	static bool ServerToConnect(const SOCKET& _Socket, SOCKADDR_IN* _pSocketAddr);
	static bool ServerToConnect(const SOCKET& _Socket, CSHPTRREF<UNetworkAddress> _spNetworkAddress);
public:
	static constexpr _int ADDR_SIZE{ sizeof(SOCKADDR_IN) + 16 };
};

/*
===========================================================
SocketUtils
===========================================================
UOverExp
===========================================================
*/
/*
@ Date: 2024-02-02, Writer: ������
@ Explain
- GetQueuedCompletionStatus�� ���ؼ� �޾ƿ� OverExp
*/
class UOverExp {
public:
	UOverExp(COMP_TYPE _type = OP_TCP_RECV);
	UOverExp(_char* _pPacket, _short _PacketType, _short _PacketSize, COMP_TYPE _type = OP_TCP_SEND);
public:
	void SendBufferReady(_char* _pPacket, _short _PacketType, _short _PacketSize, COMP_TYPE _type = OP_TCP_SEND);
	void RecvReset(COMP_TYPE _type = OP_TCP_RECV);
public: /* get set */
	WSAOVERLAPPED* GetOverlappedPointer() { return &m_Over; }
	WSABUF* GetWsaBuffPointer() { return &m_wsaBuffer; }
	const BUFFER& GetBuffer() const { return m_Buffer; }
	const COMP_TYPE& GetCompType() const { return m_CompType; }

	_char* GetBufferAddress(int _start = 0) { return &m_Buffer[_start]; }
private:
	WSAOVERLAPPED		m_Over;
	WSABUF						m_wsaBuffer;
	BUFFER							m_Buffer;
	COMP_TYPE					m_CompType;
};

/*
===========================================================
UOverExp
===========================================================
UNetworkAddress
===========================================================
*/

/*
@ Date: 2024-02-02, Writer: ������
@ Explain
- Network�� ��� �޾ƿ��� ���ϴ� Ŭ���� 
*/
class UNetworkAddress final : public UBase {
public:
	UNetworkAddress(const _string& _strIPAddress, _uint _PortNumber);
	NO_COPY(UNetworkAddress)
	DESTRUCTOR(UNetworkAddress)
public:
	SOCKADDR_IN& GetSockAddr(REF_RETURN) { return m_SocketAddr; }
	SOCKADDR_IN* GetSockAddrPointer()  { return &m_SocketAddr; }
	const _uint GetPortNumber() const { return m_PortNumber; }
	const _string& GetIPAddress() const { return m_strIPAddress; }
private:
	IN_ADDR IpToAddress();
private:
	virtual void Free() override;

private:
	SOCKADDR_IN			m_SocketAddr;
	_uint							m_PortNumber;
	_string						m_strIPAddress;
};
/*
===========================================================
UNetworkAddress
===========================================================
*/

END

