#include "ServerDefines.h"
#include "CPlayerSession.h"
#include "ACoreInstance.h"
#include "AServerService.h"
#include "ATransform.h"
#include "ACell.h"
#include "ANavigation.h"

namespace Server {

	CPlayerSession::CPlayerSession(SESSION_CONSTRUCTOR)
		: Core::ASession(SESSION_CONDATA(Core::SESSIONTYPE::PLAYER)), m_iStartCellIndex{ 741 },
		m_iWComboStack{ 0 }, m_iSComboStack{ 0 }
	{
		::memset(&m_CopyBuffer[0], 0, sizeof(BUFFER));
		::memset(&m_CopyPacketHead, 0, sizeof(PACKETHEAD));
	}

	_bool CPlayerSession::Start()
	{
		__super::Start();

		SC_CONNECTSUCCESS scConnectSuccess;
		{
			PROTOFUNC::MakeScConnectSuccess(OUT & scConnectSuccess, GetSessionID(), m_iStartCellIndex, TAG_MAINPLAYER);
		}
		CombineProto(REF_OUT m_CopyBuffer, REF_OUT m_CopyPacketHead, scConnectSuccess, TAG_SC::TAG_SC_CONNECTSUCCESS);

		SendData(&m_CopyBuffer[0], m_CopyPacketHead);
		SetGameObjectType(TAG_CHAR::TAG_MAINPLAYER);
		SetMoveSpeed(10.f);
		SetRunSpeed(30.f);
		SetCurOnCellIndex(m_iStartCellIndex);

		SHPTR<ANavigation> spNavigation = GetCoreInstance()->GetNavigation();

		SHPTR<ACell> spCell = spNavigation->FindCell(m_iStartCellIndex);
		GetTransform()->SetPos(spCell->GetCenterPos());
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
		_int SessionID = GetSessionID();
		::memset(&m_CopyBuffer[0], 0, MAX_BUFFER_LENGTH);

		switch (_PacketHead.PacketType)
		{
			case TAG_CS::TAG_CS_LOGIN:
			{
				// ���� �޾ƿ� ��Ŷ�� ���ؼ� �Ѵ�. 
				CS_LOGIN Login;
				Login.ParseFromArray(_pPacket, _PacketHead.PacketSize);
				std::cout << "Session ID Login Success Login [" << Login.id() << "]\nCurSession [" << SessionID << "]\n";
				{
					// Make Sc Login 
					SC_OTHERCLIENTLOGIN scOtherLogin;
					// �ٸ� Ŭ���̾�Ʈ�鿡�� �ش� �÷��̾ ���������� �˸�
					PROTOFUNC::MakeScOtherClientLogin(OUT  &scOtherLogin, SessionID, m_iStartCellIndex, TAG_CHAR::TAG_OTHERPLAYER);
					CombineProto(REF_OUT m_CopyBuffer, REF_OUT m_CopyPacketHead, scOtherLogin, TAG_SC::TAG_SC_OTHERCLIENTLOGIN);
					spCoreInstance->BroadCastMessageExcludingSession(SessionID, &m_CopyBuffer[0], m_CopyPacketHead);

					for (auto& iter : spCoreInstance->GetSessionContainer())
					{
						if (iter.first == SessionID)
							continue;

						if (nullptr == iter.second)
							continue;

						PROTOFUNC::MakeScOtherClientLogin(OUT & scOtherLogin, iter.first, m_iStartCellIndex, TAG_CHAR::TAG_OTHERPLAYER);
						CombineProto(REF_OUT m_CopyBuffer, REF_OUT m_CopyPacketHead, scOtherLogin, TAG_SC::TAG_SC_OTHERCLIENTLOGIN);
						SendData(&m_CopyBuffer[0], m_CopyPacketHead);
					}
				}
				// Login Packet�� �����ϰ� �޽����� ������. 
				{
					const GAMEOBJECTCONTAINER& GameObjectContainer = spCoreInstance->GetGameObjectContainer();
					SC_VIEWINRANGE scViewInRange;
					// GameObject
					SET<AGameObject*> GameObjectList;
					{
						// �þ�ó�� 
						for (auto& iter : GameObjectContainer)
						{
							GameObjectList.insert(iter.second.get());
						}
						SC_VIEWINRANGE scViewRange;
						PROTOALLOC(VECTOR3, position);
						// �ش� �༮���� �ִٰ� ������. 
						for (auto& iter : GameObjectList)
						{
							if (Login.id() == iter->GetSessionID())
								continue;

							_int GameObjectID = GetGameObjectType();

							SHPTR<ATransform> spTransform = iter->GetTransform();
							Vector3 vPosition = spTransform->GetPos();
							PROTOFUNC::MakeVector3(OUT  position, vPosition.x, vPosition.y, vPosition.z);
							PROTOFUNC::MakeScViewInRange(OUT &scViewRange, iter->GetSessionID(), IN position, iter->GetCellIndex(), GameObjectID);
							CombineProto(REF_OUT m_CopyBuffer, REF_OUT m_CopyPacketHead, scViewInRange, TAG_SC::TAG_SC_VIEWINRANGE);
							spCoreInstance->DirectSendMessage(iter->GetSessionID(), &m_CopyBuffer[0], m_CopyPacketHead);
						}
					}
				}
			}
			break;
			case TAG_CS::TAG_CS_MOVE:
			{
				SHPTR<ANavigation> spNavigation = spCoreInstance->GetNavigation();

				VECTOR3			vSendPosition;
				VECTOR3			vSendRotate;
				Vector3				vPosition;
				Vector3				vRotate;

				CHARMOVE csMove;
				csMove.ParseFromArray(_pPacket, _PacketHead.PacketSize);
				{
					vPosition = Vector3(csMove.movex(), csMove.movey(), csMove.movez());
					vRotate = Vector3(csMove.rotatex(), csMove.rotatey(), csMove.rotatez());
				}
				_bool IsMove = false;
				SHPTR<ACell> spCurrentCell{ nullptr };
				IsMove = spNavigation->IsMove(GetCurOnCellIndex(), vPosition, REF_OUT spCurrentCell);
				if (true == IsMove)
				{
					GetTransform()->SetPos(vPosition);
					SetCurOnCellIndex(spCurrentCell->GetIndex());
				}
				else
				{
					vPosition = GetTransform()->GetPos();
				}

				PROTOFUNC::MakeVector3(OUT & vSendPosition, vPosition.x, vPosition.y, vPosition.z);
				PROTOFUNC::MakeVector3(OUT & vSendRotate, vRotate.x, vRotate.y, vRotate.z);

				if(false == IsMove)
				{
					SELFPLAYERMOVE selfPlayerMove;
					PROTOFUNC::MakeSelfPlayerMove(OUT & selfPlayerMove,  GetSessionID(), vSendPosition);
					PROTOFUNC::MakeCharMove(OUT & csMove, SessionID, vSendPosition, vSendRotate);
					SendProtoData(m_CopyBuffer, selfPlayerMove, TAG_SC::TAG_SC_SELFPLAYERMOVE);
				}
				{
					CombineProto(REF_OUT m_CopyBuffer, REF_OUT m_CopyPacketHead, csMove, TAG_SC::TAG_SC_CHARMOVE);
					spCoreInstance->BroadCastMessageExcludingSession(SessionID, &m_CopyBuffer[0], m_CopyPacketHead);
		//			spCoreInstance->BroadCastMessage(&m_CopyBuffer[0], m_CopyPacketHead);
				}
			}
			break;
			case TAG_CS::TAG_CS_PLAYERSTATE:
			{
				PLAYERSTATE PlayerState;
				PlayerState.ParseFromArray(_pPacket, _PacketHead.PacketSize);

				CombineProto(REF_OUT m_CopyBuffer, REF_OUT m_CopyPacketHead, PlayerState, TAG_SC::TAG_SC_PLAYERSTATE);
				m_CopyPacketHead.PacketSize = _PacketHead.PacketSize;
		//		std::cout << PlayerState.triggername() << "\n";
			spCoreInstance->BroadCastMessageExcludingSession(PlayerState.id(), &m_CopyBuffer[0], m_CopyPacketHead);
	//		spCoreInstance->BroadCastMessage(&m_CopyBuffer[0], m_CopyPacketHead);
			}
			break;
		}
		return true;
	}

	void CPlayerSession::Free()
	{
	}
}
