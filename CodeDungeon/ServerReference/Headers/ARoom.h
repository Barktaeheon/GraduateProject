#ifndef _SERVERFRAMEWORK_CORE_PUBLIC_AROOM_H
#define _SERVERFRAMEWORK_CORE_PUBLIC_AROOM_H

#include "ACoreObject.h"

BEGIN(Core)
class ASession;
/*
@ Date: 2023-01-10, Writer: ������
@ Explain
- �� �����ӿ�ũ���� �������ϴ� Room Ŭ������ �ش� �뿡�� ���ǵ��� �޾Ƽ� �ش� ���ǵ��� �󸶳� ���Դ��� 
Ȯ���ϰ� �ش� �뿡�� ���ӿ� ���� ó���� �����Ѵ�. 
*/
class CORE_DLL ARoom abstract : public ACoreObject {
public:
	using SESSIONCONTAINER = CONHASHMAP<SESSIONID, SHPTR<ASession>>;
public:
	ARoom(OBJCON_CONSTRUCTOR, _uint _MaxSessionCount);
	DESTRUCTOR(ARoom)
public:
	virtual _bool NativeConstruct() PURE;

	_bool InsertSession(SHPTR<ASession> _spSession);
	SHPTR<ASession> FindSession(const SESSIONID _SessionID);
	void DeleteSession(const SESSIONID _SessionID);
	void BroadCastMessage(char* _pPacket, PACKETHEAD _PacketHead);
public: /* Get Set */
	ROOMID GetRoomID() const { return m_RoomID; }
	_uint GetMaxSessionCount() const { return m_MaxSessionCount; }
	_bool IsEmptyRoom() const { return m_isEmptyRoom.load(); }
private:
	virtual void Free() override;
private:
	ROOMID								m_RoomID;
	_uint									m_MaxSessionCount;
	ATOMIC<_bool>				m_isEmptyRoom;
	SESSIONCONTAINER		m_SessionContainer;
};

END

#endif // _SERVERFRAMEWORK_CORE_PUBLIC_AROOM_H

