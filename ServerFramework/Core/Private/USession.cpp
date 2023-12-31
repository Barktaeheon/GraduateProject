#include "CoreDefines.h"
#include "USession.h"
#include "UBuffer.h"

namespace Core
{
	USession::USession(MOVE Asio::ip::tcp::socket&& _TcpSocket, SESSIONTYPE _SessionType, ID _ID) : 
		m_TcpSocket(std::move(_TcpSocket)), m_SessionType(_SessionType), m_ID(_ID), m_CurBuffuerLocation{0}
	{
	}

	_bool USession::Start()
	{
		ReadData();
		return true;
	}

	_bool USession::ReadData()
	{
		_bool result{ true };
		m_TcpSocket.async_read_some(Asio::buffer(m_RecvBuffer),
			[&](const boost::system::error_code& _error, std::size_t _Size)
			{
				if (!_error)
				{
					// Insert Buffer Queue
					UBuffer Buf(m_RecvBuffer.data(), (_uint)(_Size));
					// Packet Combine 
					PacketCombine(std::move(Buf));
					ZeroMemory(&m_RecvBuffer, MAX_BUFFER_LENGTH);
					// Read Data
					ReadData();
				}
				else
				{
#ifdef USE_DEBUG
					if (_error.value() == boost::asio::error::operation_aborted) return;
					std::cout << "Receive Error on Session[" << m_ID << "] EC[" << _error << "]\n";
#endif
					result = false;
				}
			});
		return result;
	}

	_bool USession::WriteData(_char* _Packet, const PACKETHEAD& _PacketHead)
	{
		_bool result{ true };
		CombineSendBuffer(m_SendBuffer, _Packet, _PacketHead);
		// Buffer�� ���̴� Packet�� ���� + PACKETHEAD�� �����̴�.
		size_t BufferLength = static_cast<size_t>(_PacketHead.PacketSize + PACKETHEAD_SIZE);
		m_TcpSocket.async_write_some(Asio::buffer(m_SendBuffer, BufferLength),
			[&](const boost::system::error_code& _error, std::size_t _Size)
			{
				// Packet
				if (!_error)
				{
#ifdef USE_DEBUG
					if (_Size == _PacketHead.PacketSize)
					{
						std::cout << "Packet Send Failed [" << m_ID << "] EC [" << _error << "]\n";
					}
#endif
					result = false;
				}
			});
		return result;
	}

	void USession::End()
	{
	}

	/*
	===========
	PUBLIC
	===========
	PROTECTED
	===========
	*/

	/*
	@ Data: 2023-12-31
	@	 Writer : ������
	@ Explain
	- Buffer Ŭ������ �޾Ƽ� Combine �� �� Packet�� ó���ϴ� �Լ��̴�.
	*/
	void USession::PacketCombine(MOVE UBuffer&& _Buffer)
	{
		// ���۸� �����Ѵ�. 
		_Buffer.CombineBuffers(&m_RecvBuffer[0], m_CurBuffuerLocation);
		short moveBuffer{ 0 };
		char* pBufferMove = &m_RecvBuffer[0];
		// ���� BufferLocation�� ������ ��
		while (m_CurBuffuerLocation > 0)
		{
			// PacketSize�� ���Ѵ�. 
			PACKETHEAD PacketHead;
			memcpy(&PacketHead, pBufferMove, PACKETHEAD_SIZE);
			short CurrPacket = PacketHead.PacketSize + PACKETHEAD_SIZE;
			// ��Ŷ�� ���� ��ġ�� ������ �Ǵ� ���� 
			if (m_CurBuffuerLocation - CurrPacket < 0)
			{
				// ��ü ���۰����� �̵��� Packet ����� �� �� 
				_uint EraseValue = MAX_PROCESSBUF_LENGTH - moveBuffer;
				// �������� ���ۿ� PacketSize��ŭ �̵��Ͽ� ���ʿ� �����ϴ� �����͵��� �����. 
				memcpy(&m_RecvBuffer[0], pBufferMove, EraseValue);
				return;
			}
			ProcessPacket(&pBufferMove[PACKETHEAD_SIZE], PacketHead);
			// Buffer�� ��ġ�� �ű��. 
			m_CurBuffuerLocation -= CurrPacket;
			pBufferMove += CurrPacket;
			moveBuffer += CurrPacket;
		}
		m_CurBuffuerLocation = 0;
		ZeroMemory(&m_RecvBuffer, MAX_PROCESSBUF_LENGTH);
	}

	/*
	@ Data: 2023-12-31
	@	 Writer : ������
	@ Explain
	- Buffer�� �����ϴ� �Լ�
	*/
	void USession::CombineSendBuffer(REF_OUT BUFFER& _SendBuffer, _char* _Packet, const PACKETHEAD& _PacketHead)
	{
		ZeroMemory(&_SendBuffer[0], MAX_BUFFER_LENGTH);
		memcpy(&_SendBuffer[0], &_PacketHead, PACKETHEAD_SIZE);
		// [0 ~ 1] : PACKETSIZE		[2 ~ 3] : PACKETTYPE		[4 ~ ] Remain... 
		memcpy(&_SendBuffer[PACKETHEAD_SIZE], _Packet, _PacketHead.PacketSize);
	}

	void USession::Free()
	{
	}

}