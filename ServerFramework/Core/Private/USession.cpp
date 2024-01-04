#include "CoreDefines.h"
#include "USession.h"
#include "UBuffer.h"
#include "UService.h"

namespace Core
{
	USession::USession(OBJCON_CONSTRUCTOR, MOVE TCPSOCKET _TcpSocket, SHPTR<UService> _spService, SESSIONID _ID, SESSIONTYPE _SessionType) :
		UObject(OBJCON_CONDATA),
		m_TcpSocket(std::move(_TcpSocket)), m_SessionType(_SessionType), m_ID(_ID), m_CurBuffuerLocation{0},
		m_wpService{_spService}
	{
		ZeroMemory(&m_RecvBuffer, MAX_BUFFER_LENGTH);
		ZeroMemory(&m_SendBuffer, MAX_BUFFER_LENGTH);
		ZeroMemory(&m_TotalBuffer, MAX_PROCESSBUF_LENGTH);
	}

	_bool USession::Start()
	{
		ReadData();
		return true;
	}

	void USession::ReadData()
	{
	//	BUFFER recv;
		ZeroMemory(&m_RecvBuffer, MAX_BUFFER_LENGTH);
		m_TcpSocket.async_read_some(Asio::buffer(m_RecvBuffer, MAX_BUFFER_LENGTH),
			[&](const boost::system::error_code& _error, std::size_t _Size)
			{
				if (!_error)
				{
					if (_Size == 0)
					{
						Leave();
						return;
					}
					PacketCombine(&m_RecvBuffer[0], _Size);
					ReadData();
				}
				else
				{
#ifdef USE_DEBUG
					if (_error.value() == boost::asio::error::operation_aborted) return;
					std::cout << "Receive Error on Session[" << m_ID << "] EC[" << _error << "]\n";
#endif
					Leave();
				}
			});
	}

	_bool USession::WriteData(_char* _pPacket, const PACKETHEAD& _PacketHead)
	{
		return true;
	}

	void USession::Disconnect()
	{
		m_TcpSocket.close();
	}

	void USession::ConnectTcpSocket()
	{
		SESSIONID ID = m_ID;
		SHPTR<UService> spService = GetService(REF_RETURN);
		m_TcpSocket.async_connect(Asio::ip::tcp::endpoint(Asio::ip::address::from_string(IP_ADDRESS),
			Core::TCP_PORT_NUM), [&spService, &ID](const boost::system::error_code& _error) {
				// ���� ���� ���������� ����
				if (_error)
				{
					spService->RemoveService(ID);
				}
			});
	}

	/*
	@ Data: 2024-01-04, Writer : ������
	@ Explain
	- Buffer Ŭ������ �޾Ƽ� Combine �� �� Packet�� ó���ϴ� �Լ��̴�.
	*/
	void USession::PacketCombine(_char* _pPacket, _llong _Size)
	{
		// ���۸� �����Ѵ�. 
		{
			_llong value = m_CurBuffuerLocation.load();
			::memcpy(&m_TotalBuffer[value], _pPacket, _Size);
			m_CurBuffuerLocation.store(value + _Size);
		}
		short moveBuffer{ 0 };
		char* pBufferMove = &m_TotalBuffer[0];
		// ���� BufferLocation�� ������ ��
		while (m_CurBuffuerLocation > 0)
		{
			// PacketSize�� ���Ѵ�. 
			PACKETHEAD PacketHead;
			memcpy(&PacketHead, pBufferMove, PACKETHEAD_SIZE);
			short CurrPacket = PacketHead.PacketSize;
			// ��Ŷ�� ���� ��ġ�� ������ �Ǵ� ���� 
			if ((m_CurBuffuerLocation - CurrPacket ) < 0)
			{
				// ��ü ���۰����� �̵��� Packet ����� �� �� 
				_uint EraseValue = MAX_PROCESSBUF_LENGTH - moveBuffer;
				// �������� ���ۿ� PacketSize��ŭ �̵��Ͽ� ���ʿ� �����ϴ� �����͵��� �����. 
				memmove(&m_TotalBuffer[0], pBufferMove, EraseValue);
				ZeroMemory(&m_TotalBuffer[EraseValue], moveBuffer);
				return;
			}
			
			ProcessPacket(pBufferMove + PACKETHEAD_SIZE, PacketHead);
			// Buffer�� ��ġ�� �ű��. 
			m_CurBuffuerLocation -= CurrPacket;
			pBufferMove += CurrPacket;
			moveBuffer += CurrPacket;
		}
		m_CurBuffuerLocation = 0;
		ZeroMemory(&m_TotalBuffer, MAX_PROCESSBUF_LENGTH);
	}

	/*
	@ Data: 2023-12-31,  Writer : ������
	@ Explain
	- Buffer�� �����ϴ� �Լ�
	*/
	void USession::CombineSendBuffer( _char* _pPacket, const PACKETHEAD& _PacketHead)
	{
		ZeroMemory(&m_SendBuffer[0], MAX_BUFFER_LENGTH);
		memcpy(&m_SendBuffer[0], &_PacketHead, PACKETHEAD_SIZE);
		// [0 ~ 1] : PACKETSIZE		[2 ~ 3] : PACKETTYPE		[4 ~ ] Remain... 
		memcpy(&m_SendBuffer[PACKETHEAD_SIZE], _pPacket, _PacketHead.PacketSize);
	}

	void USession::Leave()
	{
		SHPTR<UService> spService =m_wpService.lock();
		spService->LeaveService(m_ID);
	}

	void USession::Free()
	{
		m_TcpSocket.close();
	}

}