#ifndef _SERVERFRAMEWORK_CORE_PUBLIC_UASIOCORE_H
#define _SERVERFRAMEWORK_CORE_PUBLIC_UASIOCORE_H

#include "UBase.h"

BEGIN(Core)
class UCoreInstance;

/*
@ Date: 2024-01-01
@ Writer: ������
@ Explain
- Socket Util���� �����ϴ� ����ü
�ش� ����ü�� ������Ʈ�� ������ �� ���� ��������� �Ѵ�.
*/
struct CACHE_ALGIN SOCKETUTILS {

	DECLARE_ONCEDATA

	SOCKETUTILS() :
		TcpAcceptor{ IoContext,Asio::ip::tcp::endpoint(Asio::ip::make_address(IP_ADDRESS),
			TCP_PORT_NUM) },
		TcpSocket{ IoContext },
		UdpSocket{ IoContext, Asio::ip::udp::endpoint(Asio::ip::make_address(IP_ADDRESS),
			UDP_PORT_NUM) }
	{
		CREATEINSTANCE_ONCEDATA;
	}

	// Boost/Asio Context
	IOContext							IoContext;
	// Tcps
	Asio::ip::tcp::acceptor	TcpAcceptor;
	Asio::ip::tcp::socket		TcpSocket;
	// UdpSocket
	Asio::ip::udp::socket		UdpSocket;

};
IMPLEMENT_ONCEDATA(SOCKETUTILS)



/*
@ Date: 2024-01-01
@ Writer: ������
@ Explain
- ��� ������� ������ AsioCore
*/
class CACHE_ALGIN_CORE_DLL UAsioCore final : public UBase {
	DECLARE_ONCEDATA
public:
	UAsioCore();
	NO_COPY(UAsioCore)
	DESTRUCTOR(UAsioCore)
		
public:
	_bool NativeConstruct();

private:
	virtual void Free() override;
private:
	SHPTR<UCoreInstance>		m_spCoreInstance;
	SOCKETUTILS						m_SocketUtils;
};

END

#endif // _SERVERFRAMEWORK_CORE_PUBLIC_UASIOCORE_H