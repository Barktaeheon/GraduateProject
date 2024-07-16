#ifndef _SERVERFRAMEWORK_CORE_PUBLIC_COREPAKCET_H
#define _SERVERFRAMEWORK_CORE_PUBLIC_COREPAKCET_H

namespace Core
{
	enum
	{
		TCP_PORT_NUM = 9998,
		UDP_PORT_NUM = 9999,
		PACKET_SIZE_CNT = 2
	};

	/*
	@ Data: 2023-12-22, Writer: ������
	@ Explain
	- Packet�� ���� �κ��� ����ϴ� Head �κ��̴�.
	*/
	struct PACKETHEAD {
		short		PacketSize;
		short		PacketType;

		PACKETHEAD() : PacketSize{ 0 }, PacketType{ 0 } {}
		PACKETHEAD(const short _size, const short _type) : PacketSize(_size), PacketType(_type) {}
	};

	constexpr static unsigned int PACKETHEAD_SIZE{ sizeof(PACKETHEAD) };
	constexpr static unsigned int PACKETSIZE_SIZE{ sizeof(short) };
}

#endif // _SERVERFRAMEWORK_CORE_PUBLIC_COREPAKCET_H