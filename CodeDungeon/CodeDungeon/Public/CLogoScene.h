#pragma once
#include "UScene.h"
BEGIN(Engine)

END

BEGIN(Client)
class CImageUI;
class CButtonUI;
class CLoadingUI;

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
	SHPTR<CImageUI>			m_spBackgroundUI;
	SHPTR<CImageUI>			m_spMainTitleUI;
	SHPTR<CImageUI>			m_spMainTitleEffectUI;
	SHPTR<CImageUI>			m_spLineEffectUI;
	SHPTR<CImageUI>			m_spBackEffectUI;

	SHPTR<CImageUI>			m_spLoadingBackgroundUI;
	SHPTR<CLoadingUI>			m_spLoadingFillingUI;

	SHPTR<CImageUI>			m_spLoadingTextUI;
	SHPTR<CLoadingUI>			m_spLoadingDotsUI;
	SHPTR<CLoadingUI>			m_spPleaseWaitTextUI;

	SHPTR<CButtonUI>			m_spButtonUI;
};

END

