#pragma once

#include "UBase.h"

BEGIN(Engine)
class UCharacter;

using PLAYERLIST = VECTOR<SHPTR<UCharacter>>;
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

	void ReigsterCurrentPlayer(CSHPTRREF<UCharacter> _spCurrentPlayer);
private:
	virtual void Free() override;
private:
	SHPTR<UCharacter>		m_spCurrentPlayer;

	
};

END