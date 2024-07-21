#pragma once

#include "UBase.h"

BEGIN(Engine)
class UCharacter;
class UPawn;
class UPlayer;

using COLLISIONPAWNLIST = SET<SHPTR<UPawn>>;
using PLAYERCONTAINER = SET<SHPTR<UPlayer>>;
/*
@ Date: 2024-02-25, Writer: ������
@ Explain
- Player, Monstter, NPC���� �����ϰ� �׵鳢���� �浹�� �����ϴ� Ŭ�����̴�. 
*/
class UCharacterManager final : public UBase {
public:
	UCharacterManager();
	NO_COPY(UCharacterManager)
	DESTRUCTOR(UCharacterManager)
public:
	CSHPTRREF<UCharacter> GetCurrPlayer() const { return m_spCurrentPlayer; }

	void RegisterCurrentPlayer(CSHPTRREF<UCharacter> _spCurrentPlayer);
	void AddCollisionPawnList(CSHPTRREF<UPawn> _spPawn);
	void RemoveCollisionPawn(CSHPTRREF<UPawn> _spPawn);
	void RemovePlayerInContainer(CSHPTRREF<UPlayer> _spPlayer);
	void AddPlayerInContainer(SHPTR<UPlayer> _spPlayer);

	SHPTR<UPlayer> FindPlayerToNetworkID(_int _iNetworkID);
	SHPTR<UPlayer> FindPlayerToDistance(const _float3& _vPos);

	void TickCollider(const _double& _dTimeDelta);
	void ClearData();
private:
	virtual void Free() override;
private:
	SHPTR<UCharacter>		m_spCurrentPlayer;
	COLLISIONPAWNLIST		m_CollisionPawnList;
	PLAYERCONTAINER		m_PlayerContainer;
};

END