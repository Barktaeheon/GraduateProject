#include "ServerDefines.h"
#include "CPlayerSession.h"
#include "ACoreInstance.h"
#include "AServerService.h"
#include "ATransform.h"
#include "ACell.h"
#include "ANavigation.h"
#include "AMonster.h"
#include "AAnimController.h"

namespace Server {

	CPlayerSession::CPlayerSession(SESSION_CONSTRUCTOR)
		: Core::ASession(SESSION_CONDATA(Core::SESSIONTYPE::PLAYER)), m_iStartCellIndex{ 100 }
	{
	}

	_bool CPlayerSession::Start(const VOIDDATAS& _ReceiveDatas)
	{
		__super::Start(_ReceiveDatas);

		SC_CONNECTSUCCESS scConnectSuccess;
		{
			PROTOFUNC::MakeScConnectSuccess(OUT & scConnectSuccess, GetSessionID(), m_iStartCellIndex, TAG_MAINPLAYER);
		}
		CombineProto(REF_OUT GetCopyBuffer(), REF_OUT GetPacketHead(), scConnectSuccess, TAG_SC::TAG_SC_CONNECTSUCCESS);

		SendData(GetCopyBufferPointer(), GetPacketHead());
		SetGameObjectType(TAG_CHAR::TAG_MAINPLAYER);
		SetMoveSpeed(10.f);
		SetRunSpeed(30.f);
		SetCurOnCellIndex(m_iStartCellIndex);
		SetCharStatus(CHARSTATUS{ 1, 0, 100 });

		SHPTR<ANavigation> spNavigation = GetNavigation();

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
		RETURN_CHECK(true == IsPermanentDisable() , false);

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
			});
		return result;
	}

	void CPlayerSession::Disconnect()
	{
		__super::Disconnect();
	}

	void CPlayerSession::ConnectTcpSocket(){}

	bool CPlayerSession::IsHit(APawn* _pPawn, const _double& _dTimeDelta)
	{
		return false;
	}

	void CPlayerSession::Collision(APawn* _pPawn, const _double& _dTimeDelta)
	{
	}

	_bool CPlayerSession::ProcessPacket(_char* _pPacket, const Core::PACKETHEAD& _PacketHead)
	{
		SHPTR<ACoreInstance> spCoreInstance = GetCoreInstance();
		_int SessionID = GetSessionID();
		::memset(GetCopyBufferPointer(), 0, MAX_BUFFER_LENGTH);

		switch (_PacketHead.PacketType)
		{
			case TAG_CS::TAG_CS_LOGIN:
			{
				LoginState(spCoreInstance, SessionID, _pPacket, _PacketHead);
			}
			break;
			case TAG_CS::TAG_CS_PLAYERSTATE:
			{
				PlayerState(spCoreInstance, SessionID, _pPacket, _PacketHead);
			}
			break;
			case TAG_CS::TAG_CS_CHARCOLLISION:
			{
				PlayerCollisionState(spCoreInstance, SessionID, _pPacket, _PacketHead);
			}
			break;
			case TAG_CS::TAG_CS_MONSTERCOLIISION:
			{
				MonsterCollisionState(spCoreInstance, SessionID, _pPacket, _PacketHead);
			}
			break;
			case TAG_CS::TAG_CS_MONSTERSTATE:
			{
				MonsterState(spCoreInstance, SessionID, _pPacket, _PacketHead);
			}
			break;
		}
		return true;
	}

	void CPlayerSession::LoginState(SHPTR<ACoreInstance> _spCoreInstance, SESSIONID _SessionID, _char* _pPacket, const Core::PACKETHEAD& _PacketHead)
	{
		// ���� �޾ƿ� ��Ŷ�� ���ؼ� �Ѵ�. 
		CS_LOGIN Login;
		Login.ParseFromArray(_pPacket, _PacketHead.PacketSize);
#ifdef USE_DEBUG
		std::cout << "Session ID Login Success Login [" << Login.id() << "]\nCurSession [" << _SessionID << "]\n";
#endif
		// �α��� �� �÷��̾��� ��Ŷ�� ������. 
		{
			// Make Sc Login 
			SC_OTHERCLIENTLOGIN scOtherLogin;
			// �ٸ� Ŭ���̾�Ʈ�鿡�� �ش� �÷��̾ ���������� �˸�
			PROTOFUNC::MakeScOtherClientLogin(OUT & scOtherLogin, _SessionID, m_iStartCellIndex, TAG_CHAR::TAG_OTHERPLAYER);
			CombineProto(REF_OUT GetCopyBuffer(), REF_OUT GetPacketHead(), scOtherLogin, TAG_SC::TAG_SC_OTHERCLIENTLOGIN);
			_spCoreInstance->BroadCastMessageExcludingSession(_SessionID, GetCopyBufferPointer(), GetPacketHead());

			for (auto& iter : _spCoreInstance->GetSessionContainer())
			{
				if (iter.first == _SessionID)
					continue;

				if (true == iter.second->IsPermanentDisable())
					continue;

				PROTOFUNC::MakeScOtherClientLogin(OUT & scOtherLogin, iter.first, m_iStartCellIndex, TAG_CHAR::TAG_OTHERPLAYER);
				CombineProto(REF_OUT GetCopyBuffer(), REF_OUT GetPacketHead(), scOtherLogin, TAG_SC::TAG_SC_OTHERCLIENTLOGIN);
				SendData(GetCopyBufferPointer(), GetPacketHead());
			}
			const MOBOBJCONTAINER& MobObjectContainer = _spCoreInstance->GetMobObjContainer();
			for (auto& iter : MobObjectContainer)
			{
				if (true == iter.second->IsPermanentDisable())
				{
					iter.second->RunningPermanentDisableSituation();
				}
			}
		}
	}

	void CPlayerSession::PlayerState(SHPTR<ACoreInstance> _spCoreInstance, SESSIONID _SessionID, _char* _pPacket, const Core::PACKETHEAD& _PacketHead)
	{
		SHPTR<ANavigation> spNavigation = GetNavigation();

		Vector3				vPosition;

		CHARSTATE csMove;
		csMove.ParseFromArray(_pPacket, _PacketHead.PacketSize);
		{
			vPosition = Vector3(csMove.posx(), csMove.posy(), csMove.posz());
			GetTransform()->SetPos(vPosition);
		}
		//  �ٸ� ��� Ŭ���̾�Ʈ�� �޽��� ������. 
		{
		//	PROTOFUNC::MakeCharState(OUT & csMove, _SessionID, vSendPosition, vSendRotate);
			CombineProto(REF_OUT GetCopyBuffer(), REF_OUT GetPacketHead(), csMove, TAG_SC::TAG_SC_PLAYERSTATE);
			_spCoreInstance->BroadCastMessageExcludingSession(_SessionID, GetCopyBufferPointer(), GetPacketHead());
		}
		// ���� Ȱ��ȭ 
		//{
		//	const MOBOBJCONTAINER& MobObjectContainer = _spCoreInstance->GetMobObjContainer();
		//	for (auto& iter : MobObjectContainer)
		//	{
		//		// ������ ��Ȱ��ȭ
		//		if (true == iter.second->IsPermanentDisable())
		//			continue;

		//		// �Ÿ��� �޾ƿ�
		//		_float fDistanace = iter.second->GetTransform()->ComputeDistanceSq(vPosition);
		//		if (true == iter.second->IsCanSee(iter.second->GetTransform()))
		//		{
		//			iter.second->InsertMobJobTimer(GetSessionID());
		//		}
		//		else
		//		{
		//			iter.second->SetActive(false);
		//		}
		//	}
		//}
	}

	void CPlayerSession::PlayerCollisionState(SHPTR<ACoreInstance> _spCoreInstance, SESSIONID _SessionID, _char* _pPacket, const Core::PACKETHEAD& _PacketHead)
	{
		COLLISIONDATA CollisionData;
		CollisionData.ParseFromArray(_pPacket, _PacketHead.PacketSize);
		SHPTR<ASession> spSession = _spCoreInstance->FindSession(CollisionData.id());
		SHPTR<ATransform> spPlayerTr = spSession->GetTransform();
		spSession->SetDamaged(true);
		SHPTR<AMonster> spMonster = _spCoreInstance->FindMobObject(CollisionData.enemyid());
		spSession->DamageToEnemy(spMonster->GetCharStatus().fPower);

		SC_DAMAGED scDamaged;
		PROTOFUNC::MakeScDamaged(&scDamaged, spSession->GetSessionID(), spSession->GetCharStatus().fHp);
		CombineProto<SC_DAMAGED>(GetCopyBuffer(), GetPacketHead(), scDamaged, TAG_SC_DAMAGED);
		_spCoreInstance->BroadCastMessage(GetCopyBufferPointer(), GetPacketHead());
	}

	void CPlayerSession::MonsterCollisionState(SHPTR<ACoreInstance> _spCoreInstance, SESSIONID _SessionID, _char* _pPacket, const Core::PACKETHEAD& _PacketHead)
	{
		COLLISIONDATA CollisionData;
		CollisionData.ParseFromArray(_pPacket, _PacketHead.PacketSize);
		SHPTR<AMonster> spMonster = _spCoreInstance->FindMobObject(CollisionData.id());
		spMonster->SetDamaged(true);
		SHPTR<ASession> spSession = _spCoreInstance->FindSession(CollisionData.enemyid());
		spMonster->DamageToEnemy(spSession->GetCharStatus().fPower);

		SC_DAMAGED scDamaged;
		PROTOFUNC::MakeScDamaged(&scDamaged, spSession->GetSessionID(), spSession->GetCharStatus().fHp);
		CombineProto<SC_DAMAGED>(GetCopyBuffer(), GetPacketHead(), scDamaged, TAG_SC_DAMAGED);
		_spCoreInstance->BroadCastMessage(GetCopyBufferPointer(), GetPacketHead());
	}

	void CPlayerSession::MonsterState(SHPTR<ACoreInstance> _spCoreInstance, SESSIONID _SessionID, _char* _pPacket, const Core::PACKETHEAD& _PacketHead)
	{
		MONSTERSTATE monsterState;
		monsterState.ParseFromArray(_pPacket, _PacketHead.PacketSize);
		CombineProto<MONSTERSTATE>(GetCopyBuffer(), GetPacketHead(), monsterState, TAG_SC_MONSTERSTATE);
		_spCoreInstance->BroadCastMessageExcludingSession(_SessionID, GetCopyBufferPointer(), GetPacketHead());
		_int MonsterID = monsterState.id();
		SHPTR<AMonster> spMonster = _spCoreInstance->FindMobObject(MonsterID);
		if (nullptr != spMonster)
		{
			spMonster->ProcessPacket(TAG_CS_MONSTERSTATE, &monsterState);
		}
	}

	void CPlayerSession::Free()
	{
	}
}


/*
				// Login Packet�� �����ϰ� �޽����� ������.
				{
					const MOBOBJCONTAINER& GameObjectContainer = spCoreInstance->GetMobObjContainer();
					SC_VIEWINRANGE scViewInRange;
					// GameObject
					SET<AMonster*> GameObjectList;
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
*/