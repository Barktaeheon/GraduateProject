#include "ServerDefines.h"
#include "CPlayerSession.h"
#include "ACoreInstance.h"
#include "AServerService.h"
#include "ATransform.h"
#include "ACell.h"
#include "ANavigation.h"
#include "AMonster.h"
#include "AAnimController.h"
#include "ACollider.h"

namespace Server {

	CPlayerSession::CPlayerSession(SESSION_CONSTRUCTOR)
		: Core::ASession(SESSION_CONDATA(Core::SESSIONTYPE::PLAYER)), m_iStartCellIndex{ 100 },
		m_iWComboStack{ 0 }, m_iSComboStack{ 0 }
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

		COLLIDERINFO Info{ ACollider::TYPE_OBB, Vector3(0.f, 0.f, 0.f), Vector3(0.f, 0.f, 0.f) };
		InsertColliderContainer(Info);
		for (auto& Colliders : GetColliderContainer())
		{
			Colliders->SetScale(Vector3{ 4, 10, 4 });
			Colliders->SetTranslate(Vector3{ 0, 10, 0 });
		}

		SHPTR<ANavigation> spNavigation = GetNavigation();
		SHPTR<ACell> spCell = spNavigation->FindCell(m_iStartCellIndex);
		GetTransform()->SetPos(spCell->GetCenterPos());
		SetCharStatus(CHARSTATUS{ 1, 1, 100 });
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
		return __super::IsHit(_pPawn, _dTimeDelta);
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
			case TAG_CS::TAG_CS_RESOURCE_RECEIVE_SUCCESS:
			{

			}
				break;
			case TAG_CS::TAG_CS_MOVE:
			{
				MoveState(spCoreInstance, SessionID, _pPacket, _PacketHead);
			}
			break;
			case TAG_CS::TAG_CS_PLAYERSTATE:
			{
				PlayerState(spCoreInstance, SessionID, _pPacket, _PacketHead);
			}
			break;
			case TAG_CS::TAG_CS_PLAYERCOLLISION:
			{
				PlayerCollisionState(spCoreInstance, SessionID, _pPacket, _PacketHead);
			}
			break;
			case TAG_CS::TAG_CS_MONSTERCOLIISION:
			{
				MonsterCollisionState(spCoreInstance, SessionID, _pPacket, _PacketHead);
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
		}
		//// Monster�� ��Ŷ���� �̸� ����������. 
		//{
		//	SC_MONSTERRESOURCEDATA scMonsterResourceData;
		//	VECTOR3 vPos;
		//	const MOBOBJCONTAINER& GameObjectContainer = _spCoreInstance->GetMobObjContainer();
		//	for (auto& iter : GameObjectContainer)
		//	{
		//		SHPTR<AAnimController> spAnimController = iter.second->GetAnimController();
		//		Vector3 vPosVector = iter.second->GetTransform()->GetPos();
		//		PROTOFUNC::MakeVector3(&vPos, vPosVector.x, vPosVector.y, vPosVector.z);
		//		PROTOFUNC::MakeScMonsterResourceData(&scMonsterResourceData, iter.first, vPos, 
		//			spAnimController->GetCurAnimIndex(), iter.second->GetCellIndex(), iter.second->GetMonsterType());

		//		CombineProto(REF_OUT GetCopyBuffer(), REF_OUT GetPacketHead(), scMonsterResourceData, TAG_SC::TAG_SC_MONSTERRESOURCEDATA);
		//		SendData(GetCopyBufferPointer(), GetPacketHead());
		//	}
		//	// ��� �ڿ����� �����µ� ���������� 
		//	{
		//		SC_START_INFORMATION_SUCCESS		scStartInfomationSuccess;
		//		PROTOFUNC::MakeScStartInformationSucess(&scStartInfomationSuccess, GetSessionID(), (_int)GameObjectContainer.size());
		//		CombineProto(REF_OUT GetCopyBuffer(), REF_OUT GetPacketHead(), scStartInfomationSuccess, TAG_SC::TAG_SC_START_INFORMATION_SUCCESS);
		//		SendData(GetCopyBufferPointer(), GetPacketHead());
		//	}
		//}
	}

	void CPlayerSession::MoveState(SHPTR<ACoreInstance> _spCoreInstance, SESSIONID _SessionID, _char* _pPacket, const Core::PACKETHEAD& _PacketHead)
	{
		SHPTR<ANavigation> spNavigation = GetNavigation();
		SHPTR<ATransform> spTransform = GetTransform();

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
		// �׺���̼��� ���� �̵��� �� ���� ��ġ�� ǥ���Ѵ�. 
		{
			IsMove = spNavigation->IsMove(vPosition, REF_OUT spCurrentCell);
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
		}
		{
			_float4x4 worldMatrix = spTransform->GetWorldMatrix();
			for (auto& iter : GetColliderContainer())
			{
				iter->SetTransform(worldMatrix);
			}
		}
		// ���� Ȱ��ȭ 
		{
			const MOBOBJCONTAINER& MobObjectContainer = _spCoreInstance->GetMobObjContainer();
			for (auto& iter : MobObjectContainer)
			{
				// ������ ��Ȱ��ȭ
				if (true == iter.second->IsPermanentDisable())
					continue;

				// �Ÿ��� �޾ƿ�
				_float fDistanace =  iter.second->GetTransform()->ComputeDistanceSq(vPosition);
				if (true == iter.second->IsCanSee(iter.second->GetTransform()))
				{
					if (true == iter.second->IsChangeTargetPlayer(GetSessionID(), fDistanace))
					{
						iter.second->InsertMobJobTimer(GetSessionID());
					}
				}
				else
				{
					iter.second->SetActive(false);
				}
			}
		}
		// ���� ������ �� ������
		if (false == IsMove)
		{
			// ���� ��ġ�� �̵��� �� �ֵ��� �Ѵ�. 
			SELFPLAYERMOVE selfPlayerMove;
			PROTOFUNC::MakeSelfPlayerMove(OUT & selfPlayerMove, GetSessionID(), vSendPosition);
			PROTOFUNC::MakeCharMove(OUT & csMove, _SessionID, vSendPosition, vSendRotate);
			SendProtoData(REF_OUT GetCopyBuffer(), selfPlayerMove, TAG_SC::TAG_SC_SELFPLAYERMOVE);
		}
		//  �ٸ� ��� Ŭ���̾�Ʈ�� �޽��� ������. 
		{
			CombineProto(REF_OUT GetCopyBuffer(), REF_OUT GetPacketHead(), csMove, TAG_SC::TAG_SC_CHARMOVE);
			_spCoreInstance->BroadCastMessageExcludingSession(_SessionID, GetCopyBufferPointer(), GetPacketHead());
		}
	}

	void CPlayerSession::PlayerState(SHPTR<ACoreInstance> _spCoreInstance, SESSIONID _SessionID, _char* _pPacket, const Core::PACKETHEAD& _PacketHead)
	{
#ifdef USE_DEBUG
		std::cout << "PlayerState [" << _SessionID << "]\n";
#endif
		PLAYERSTATE PlayerState;
		PlayerState.ParseFromArray(_pPacket, _PacketHead.PacketSize);
		CombineProto(REF_OUT GetCopyBuffer(), REF_OUT GetPacketHead(), PlayerState, TAG_SC::TAG_SC_PLAYERSTATE);
		_spCoreInstance->BroadCastMessageExcludingSession(PlayerState.id(), GetCopyBufferPointer(), GetPacketHead());
	}

	void CPlayerSession::PlayerCollisionState(SHPTR<ACoreInstance> _spCoreInstance, SESSIONID _SessionID, _char* _pPacket, const Core::PACKETHEAD& _PacketHead)
	{
		COLLISIONDATA CollisionData;
		CollisionData.ParseFromArray(_pPacket, _PacketHead.PacketSize);
		SHPTR<ASession> spSession = _spCoreInstance->FindSession(CollisionData.id());
		SHPTR<ATransform> spTransform = spSession->GetTransform();
		spTransform->SetPos(Vector3{ CollisionData.posx(), CollisionData.posy(), CollisionData.posz() });

		if (1 == CollisionData.damageenable())
		{
			SHPTR<AMonster> spMonster = _spCoreInstance->FindMobObject(CollisionData.id());
			spSession->Damaged(spMonster->GetCharStatus().fPower);
		}
	}

	void CPlayerSession::MonsterCollisionState(SHPTR<ACoreInstance> _spCoreInstance, SESSIONID _SessionID, _char* _pPacket, const Core::PACKETHEAD& _PacketHead)
	{
		COLLISIONDATA CollisionData;
		CollisionData.ParseFromArray(_pPacket, _PacketHead.PacketSize);
		SHPTR<AMonster> spMonster = _spCoreInstance->FindMobObject(CollisionData.id());
		SHPTR<ATransform> spTransform = spMonster->GetTransform();
		spTransform->SetPos(Vector3{CollisionData.posx(), CollisionData.posy(), CollisionData.posz() });

		if (1 == CollisionData.damageenable())
		{
			SHPTR<ASession> spSession = _spCoreInstance->FindSession(CollisionData.enemyid());
			spMonster->Damaged(spSession->GetCharStatus().fPower);
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