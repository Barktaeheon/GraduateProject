#pragma once
#include "UAnimationController.h"

BEGIN(Client)
class CWarriorPlayer;
class UAnimation;

class CNetworkWarriorPlayerController  final : public UAnimationController {
public:
	enum WARRIOR_STATE {
		ANIM_ROLL = 6, ANIM_RUN = 7, ANIM_JUMP = 8
	};
public:
	CNetworkWarriorPlayerController(CSHPTRREF<UDevice> _spDevice);
	CNetworkWarriorPlayerController(const CNetworkWarriorPlayerController& _rhs);
	virtual ~CNetworkWarriorPlayerController() = default;
public:
	virtual void Free() override;
	CLONE_MACRO(CNetworkWarriorPlayerController, "CNetworkWarriorPlayerController::Clone To Failed")
public:
	// Native Construct 
	virtual HRESULT NativeConstruct() override;
	// Clone 
	virtual HRESULT NativeConstructClone(const VOIDDATAS& _tDatas) override;
	// Tick
	virtual void Tick(const _double& _dTimeDelta) override;
	// ��Ʈ��ũ ������ �޾ƿ��� �Լ�
	virtual void ReceiveNetworkProcessData(void* _pData) override;
private:
	WKPTR< CWarriorPlayer>			m_wpWarriorPlayer;
	_int													m_JumpSpeed;
};

END