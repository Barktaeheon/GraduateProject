#pragma once
#include "UScene.h"
BEGIN(Engine)

END

BEGIN(Client)

class CLogoScene   final : public UScene {
public:
	CLogoScene(CSHPTRREF<UDevice> _spDevice);
	NO_COPY(CLogoScene)
	DESTRUCTOR(CLogoScene)
public:
	// UScene��(��) ���� ��ӵ�
	virtual void Free() override;
	virtual HRESULT LoadSceneData() override;
	virtual void Tick(const _double& _dTimeDelta) override;
	virtual void LateTick(const _double& _dTimeDelta) override;
private:

};

END

