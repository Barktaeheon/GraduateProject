#include "CoreDefines.h"
#include "URoom.h"
#include "USession.h"

namespace Core {

	URoom::URoom(OBJCON_CONSTRUCTOR, _uint _MaxSessionCount) :
		UObject(OBJCON_CONDATA), m_RoomID{ 0 }, m_MaxSessionCount {_MaxSessionCount}, m_isEmptyRoom{true}
	{
	}

	_bool URoom::InsertSession(SHPTR<USession> _spSession)
	{
		RETURN_CHECK(nullptr == _spSession, false);
		// �̹� ������ ����
		RETURN_CHECK(nullptr != FindSession(_spSession->GetID()), false)
		SESSIONID id = _spSession->GetID();
		m_SessionContainer.emplace(MakePair(id, _spSession));
		m_isEmptyRoom.store(false, std::memory_order_seq_cst);
		return true;
	}

	SHPTR<USession> URoom::FindSession(const SESSIONID _SessionID)
	{
		SESSIONCONTAINER::accessor access;
		m_SessionContainer.find(access, _SessionID);
		return access->second;
	}

	void URoom::DeleteSession(const SESSIONID _SessionID)
	{
		SESSIONCONTAINER::accessor access;
		m_SessionContainer.find(access, _SessionID);
		RETURN_CHECK(true == access.empty(), ;);
		m_SessionContainer.erase(access);

		if (m_SessionContainer.size() <= 0)
		{
			m_isEmptyRoom.store(true, std::memory_order_seq_cst);
		}
	}

	void URoom::BroadCastMessage(char* _pPacket, PACKETHEAD _PacketHead)
	{
		for (auto& iter : m_SessionContainer)
		{
			iter.second->WriteData(_pPacket, _PacketHead);
		}
	}

	void URoom::Free()
	{
	}
}
