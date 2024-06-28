#include "CoreDefines.h"
#include "ASession.h"
#include "ATransform.h"
#include "ASpace.h"
#include "ACoreInstance.h"

namespace Core
{
	ASession::ASession(OBJCON_CONSTRUCTOR, MOVE TCPSOCKET _TcpSocket, SESSIONID _ID, SESSIONTYPE _SessionType) :
		AGameObject(OBJCON_CONDATA, _ID, _SessionType),
		m_TcpSocket(std::move(_TcpSocket)), m_CurBuffuerLocation{0},	m_isConnected{true}
	{
		MemoryInitialization(m_SendBuffer.data(), MAX_BUFFER_LENGTH);
		MemoryInitialization(m_RecvBuffer.data(), MAX_BUFFER_LENGTH);
		MemoryInitialization(m_TotalBuffer.data(), MAX_PROCESSBUF_LENGTH);
	}

	_bool ASession::Start()
	{
		__super::Start();
		RecvData();
		return true;
	}

	void ASession::RecvData()
	{
		RETURN_CHECK(false == m_isConnected, ;);
		m_TcpSocket.async_read_some(Asio::buffer(m_RecvBuffer),
			[this](const boost::system::error_code& _error, std::size_t _Size)
			{	
				if (!_error)
				{
					if (_Size == 0)
					{
						Leave();
						return;
					}
					PacketCombine(&m_RecvBuffer[0], _Size);
					RecvData();
				}
				else
				{
#ifdef USE_DEBUG
					if (_error.value() == boost::asio::error::operation_aborted) return;
					std::cout << "Receive Error on Session[" << GetSessionID() << "] EC[" << _error << "]\n";
#endif
					Leave();
				}
			});
	}

	_bool ASession::SendData(_char* _pPacket, const PACKETHEAD& _PacketHead)
	{
		RETURN_CHECK(false == m_isConnected, false);
		return true;
	}

	void ASession::Disconnect()
	{
#ifdef USE_DEBUG
		std::cout << "DisConnected" << std::endl;
#endif
		m_isConnected = false;
	}

	void ASession::ConnectTcpSocket(){ }

	/*
	@ Data: 2024-01-13, Writer : ������
	@ Explain
	- Buffer Ŭ������ �޾Ƽ� Combine �� �� Packet�� ó���ϴ� �Լ��̴�.
	*/
	void ASession::PacketCombine(_char* _pPacket, _llong _Size)
	{
		// ���۸� �����Ѵ�. 
		{
			::memcpy(&m_TotalBuffer[m_CurBuffuerLocation], _pPacket, _Size);
			m_CurBuffuerLocation +=_Size;
		}
		short moveBuffer{ 0 };
		char* pBufferMove = &m_TotalBuffer[0];
		// ���� BufferLocation�� ������ ��
		while (m_CurBuffuerLocation > 0)
		{
			// PacketSize�� ���Ѵ�. 
			PACKETHEAD PacketHead;
			memcpy(&PacketHead, pBufferMove, PACKETHEAD_SIZE);
			short CurrPacket = PacketHead.PacketSize + PACKETHEAD_SIZE;
			// ��Ŷ�� ���� ��ġ�� ������ �Ǵ� ���� 
			if ((m_CurBuffuerLocation - CurrPacket) < 0)     
			{
#ifdef USE_DEBUG
		//		std::cout << m_CurBuffuerLocation << "\n";
#endif
				// �������� ���ۿ� PacketSize��ŭ �̵��Ͽ� ���ʿ� �����ϴ� �����͵��� �����. 
				memmove(&m_TotalBuffer[0], pBufferMove, MAX_PROCESSBUF_LENGTH - moveBuffer);
				return;
			}
			ProcessPacket(&pBufferMove[PACKETHEAD_SIZE], PacketHead);
			// Buffer�� ��ġ�� �ű��. 
			m_CurBuffuerLocation -= CurrPacket;
			pBufferMove += CurrPacket;
			moveBuffer += CurrPacket;
		}
		m_CurBuffuerLocation = 0;
		MemoryInitialization(m_TotalBuffer.data(), MAX_PROCESSBUF_LENGTH);
	}

	/*
	@ Data: 2023-12-31,  Writer : ������
	@ Explain
	- Buffer�� �����ϴ� �Լ�
	*/
	void ASession::CombineSendBuffer( _char* _pPacket, const PACKETHEAD& _PacketHead)
	{
		::memset(&m_SendBuffer[0], 0, MAX_BUFFER_LENGTH);
		memcpy(&m_SendBuffer[0], &_PacketHead, PACKETHEAD_SIZE);
		// [0 ~ 1] : PACKETSIZE		[2 ~ 3] : PACKETTYPE		[4 ~ ] Remain... 
		memcpy(&m_SendBuffer[PACKETHEAD_SIZE], _pPacket, _PacketHead.PacketSize);
	}

	void ASession::Leave()
	{
		SHPTR<ACoreInstance> spCoreInstance = GetCoreInstance();
		spCoreInstance->LeaveService(GetSessionID());
	}

	void ASession::Free()
	{
		m_TcpSocket.close();
	}

}