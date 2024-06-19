#include "ServerDefines.h"
#include "CPlayerSession.h"
#include "ACoreInstance.h"
#include "AServerService.h"
#include "ATransform.h"

namespace Server {

	CPlayerSession::CPlayerSession(SESSION_CONSTRUCTOR)
		: Core::ASession(SESSION_CONDATA(Core::SESSIONTYPE::PLAYER)), m_iStartCellIndex{ 741 }
	{
		::memset(&m_CopyBuffer[0], 0, sizeof(BUFFER));
		::memset(&m_CopyPacketHead, 0, sizeof(PACKETHEAD));
	}

	_bool CPlayerSession::Start()
	{
		__super::Start();

		SC_CONNECTSUCCESS scConnectSuccess;
		PROTOALLOC(CHARDATA, charData);
		PROTOALLOC(VECTOR3, scale);
		{
			PROTOFUNC::MakeVector3(OUT scale, 1, 1, 1);
			PROTOFUNC::MakeCharData(OUT charData, GetCharStatus().fPower, GetCharStatus().fDefensive, GetCharStatus().fHp);
			PROTOFUNC::MakeScConnectSuccess(OUT & scConnectSuccess, GetSessionID(), IN charData, m_iStartCellIndex, IN scale, TAG_MAINPLAYER);
		}
		CombineProto(REF_OUT m_CopyBuffer, REF_OUT m_CopyPacketHead, scConnectSuccess, TAG_SC::TAG_SC_CONNECTSUCCESS);
		SendData(&m_CopyBuffer[0], m_CopyPacketHead);
		SetGameObjectType(TAG_CHAR::TAG_MAINPLAYER);
		return true;
	}

	void CPlayerSession::RecvData()
	{
		__super::RecvData();
	}

	_bool CPlayerSession::SendData(_char* _pPacket, const Core::PACKETHEAD& _PacketHead)
	{
		RETURN_CHECK(false == IsConnected() , false);

		CombineSendBuffer( _pPacket, _PacketHead);
		_bool result{ true };

		TCPSOCKET& Socket = GetTcpSocket(REF_RETURN);

		Socket.async_write_some(Asio::buffer(GetSendBuff(), _PacketHead.PacketSize + PACKETHEAD_SIZE),
			[this](const boost::system::error_code& _error, std::size_t _Size) {
			SESSIONID SessionID = GetSessionID();
			Core::SHPTR<Core::ACoreInstance> spCoreInstance{ GetCoreInstance() };
				// Packet
				if (_error)
				{
					if (nullptr != spCoreInstance)
						spCoreInstance->LeaveService(SessionID);
				}
				else
				{
					if (false == IsConnected())
					{
						if (nullptr != spCoreInstance)
						{
							spCoreInstance->LeaveService(SessionID);
						}
					}
				}
			});
		return result;
	}

	void CPlayerSession::Disconnect()
	{
		__super::Disconnect();
	}

	void CPlayerSession::ConnectTcpSocket(){}

	_bool CPlayerSession::ProcessPacket(_char* _pPacket, const Core::PACKETHEAD& _PacketHead)
	{
		SHPTR<ACoreInstance> spCoreInstance = GetCoreInstance();

		switch (_PacketHead.PacketType)
		{
			case TAG_CS::TAG_CS_LOGIN:
			{
				// ���� �޾ƿ� ��Ŷ�� ���ؼ� �Ѵ�. 
				CS_LOGIN Login;
				Login.ParseFromArray(_pPacket, _PacketHead.PacketSize);
				{
					// Make Sc Login 
					SC_OTHERCLIENTLOGIN scOtherLogin;
					// �ٸ� Ŭ���̾�Ʈ�鿡�� �ش� �÷��̾ ���������� �˸�
					PROTOFUNC::MakeScOtherClientLogin(OUT  &scOtherLogin, Login.id(), m_iStartCellIndex, TAG_CHAR::TAG_OTHERPLAYER);
					CombineProto(REF_OUT m_CopyBuffer, REF_OUT m_CopyPacketHead, scOtherLogin, TAG_SC::TAG_SC_OTHERCLIENTLOGIN);
					spCoreInstance->BroadCastMessageExcludingSession(Login.id(), &m_CopyBuffer[0], m_CopyPacketHead);
				}
				// Login Packet�� �����ϰ� �޽����� ������. 
				//{
				//	const GAMEOBJECTCONTAINER& GameObjectContainer = spCoreInstance->GetGameObjectContainer();
				//	SC_VIEWINRANGE scViewInRange;
				//	// GameObject
				//	SET<AGameObject*> GameObjectList;
				//	{
				//		// �þ�ó�� 
				//		for (auto& iter : GameObjectContainer)
				//		{
				//			GameObjectList.insert(iter.second.get());
				//		}
				//		SC_VIEWINRANGE scViewRange;
				//		PROTOALLOC(VECTOR3, position);
				//		// �ش� �༮���� �ִٰ� ������. 
				//		for (auto& iter : GameObjectList)
				//		{
				//			if (Login.id() == iter->GetSessionID())
				//				continue;

				//			_int GameObjectID = GetGameObjectType() == TAG_CHAR::TAG_MAINPLAYER ? TAG_CHAR::TAG_OTHERPLAYER : GetGameObjectType();

				//			SHPTR<ATransform> spTransform = iter->GetTransform();
				//			Vector3 vPosition = spTransform->GetPos();
				//			PROTOFUNC::MakeVector3(OUT  position, vPosition.x, vPosition.y, vPosition.z);
				//			PROTOFUNC::MakeScViewInRange(OUT &scViewRange, iter->GetSessionID(), IN position, iter->GetCellIndex(), GameObjectID);
				//			CombineProto(REF_OUT m_CopyBuffer, REF_OUT m_CopyPacketHead, scViewInRange, TAG_SC::TAG_SC_VIEWINRANGE);
				//			spCoreInstance->DirectSendMessage(iter->GetSessionID(), &m_CopyBuffer[0], m_CopyPacketHead);
				//		}
				//	}
				//}
			}
			break;
			case TAG_CS::TAG_CS_MOVE:
			{
				CS_MOVE csMove;
				csMove.ParseFromArray(_pPacket, _PacketHead.PacketSize);


			}
			break;
		}
		return true;
	}

	void CPlayerSession::Free()
	{
	}
}
