#include "CoreDefines.h"
#include "USession.h"
#include "UService.h"
#include "UAABBCollider.h"
#include "UOBBCollider.h"
#include "USphereCollider.h"
#include "UTransform.h"
#include "USpace.h"

namespace Core
{
	USession::USession(OBJCON_CONSTRUCTOR, MOVE TCPSOCKET _TcpSocket, SHPTR<UService> _spService, SESSIONID _ID, SESSIONTYPE _SessionType) :
		UObject(OBJCON_CONDATA),
		m_TcpSocket(std::move(_TcpSocket)), m_SessionType(_SessionType), m_SessionID(_ID), m_SpaceIndex{0}, m_CurBuffuerLocation{0},
		m_wpService{_spService},
		m_isConnected{true},
		 m_spTransform{nullptr},	m_spCollider{nullptr}
	{
		MemoryInitialization(m_SendBuffer.data(), MAX_BUFFER_LENGTH);
		MemoryInitialization(m_RecvBuffer.data(), MAX_BUFFER_LENGTH);
		MemoryInitialization(m_TotalBuffer.data(), MAX_PROCESSBUF_LENGTH);
	}

	_bool USession::Start()
	{
		m_spTransform = Create<UTransform>();
		ReadData();
		return true;
	}

	void USession::ReadData()
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
					ReadData();
				}
				else
				{
#ifdef USE_DEBUG
					if (_error.value() == boost::asio::error::operation_aborted) return;
					std::cout << "Receive Error on Session[" << m_SessionID << "] EC[" << _error << "]\n";
#endif
					Leave();
				}
			});
	}

	_bool USession::WriteData(_char* _pPacket, const PACKETHEAD& _PacketHead)
	{
		RETURN_CHECK(false == m_isConnected, false);
		return true;
	}

	void USession::Disconnect()
	{
		m_isConnected = false;
	}

	void USession::ConnectTcpSocket(){ }

	void USession::BringSpaceIndex(SHPTR<USpace> _spSpace)
	{
		RETURN_CHECK(nullptr == _spSpace, ;);
		m_SpaceIndex = _spSpace->GetSpaceIndex();
	}

	/*
	@ Data: 2024-01-13, Writer : ������
	@ Explain
	- Buffer Ŭ������ �޾Ƽ� Combine �� �� Packet�� ó���ϴ� �Լ��̴�.
	*/
	void USession::PacketCombine(_char* _pPacket, _llong _Size)
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
		spService->LeaveService(m_SessionID);
	}

	void USession::CreateCollider(COLLIDERTYPE _ColliderType, const Vector3& _vCenter, const Vector3& _vScale)
	{
		switch (_ColliderType) {
		case COLLIDERTYPE::OBB:
			m_spCollider = Create<UOBBCollider>(_vCenter, _vScale);
			break;
		case COLLIDERTYPE::AABB:
			m_spCollider = Create<UAABBCollider>(_vCenter, _vScale);
			break;
		case COLLIDERTYPE::SPHERE:
			m_spCollider = Create<USphereCollider>(_vCenter, _vScale);
			break;
		}
	}

	void USession::Free()
	{
		m_TcpSocket.close();
	}

}