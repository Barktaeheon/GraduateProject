#pragma once

#include "TImGuiView.h"

BEGIN(Engine)
class UParticle;
class UAnimatedParticle;
class UFire;
END

BEGIN(Tool)

class TFireView : public TImGuiView {
public:
	TFireView(CSHPTRREF<UDevice> _spDevice);
	NO_COPY(TFireView)
		DESTRUCTOR(TFireView)
public:
	// TImGuiView��(��) ���� ��ӵ�
	virtual void Free() override;

	virtual HRESULT NativeConstruct() override;
protected:
	virtual HRESULT LoadResource() override;
	virtual HRESULT ReleaseResource() override;
	virtual void TickActive(const _double& _dTimeDelta) override;
	virtual void LateTickActive(const _double& _dTimeDetla) override;
	virtual void RenderActive() override;
private:
	void RenderMenu();
	void DockBuildInitSetting();
	void FireView();
	void FireColorTextureSetting();
	void FireNoiseTextureSetting();
	void FireAlphaTextureSetting();
	void FireScalingSetting();
	void FirePosSetting();
//private:
//	//Multiple Particle Setting Functions
//	void MultipleParticleView();
//	void ResizeMultipleParticleVector(_uint _resizeAmount);
//
//	void MultipleParticleCountSetting();
//	void MultipleParticleTimeSetting();
//	void MultipleParticleTexSetting();
//	void DefaultMultipleParticleSetting();
//	void AutomaticMultipleParticleSetting();
//
//	void LoadMultipleParticleResource();
//	void ReleaseMultipleParticleResource();
//
//	void SetMultipleParticle(SHPTR<UParticle> _spParticle, int index) { m_MultipleParticle[index] = _spParticle; }
//	_uint GetMultipleParticleSize() { return m_iMultipleParticleSize; }
//	_uint GetMultipleParticleCurActiveSize() { return m_iCurActiveMultipleParticle; }

//private:
//	using PARTICLES = VECTOR<SHPTR<UParticle>>;
//	using ANIMPARTICLES = VECTOR<SHPTR<UAnimatedParticle>>;
//	using PARTICLEPARAMS = VECTOR<PARTICLEPARAM*>;
//	using PARTICLEANIMPARAMS = VECTOR< ANIMATEDPARTICLEPARAM*>;
//	using PARTICLETYPES = VECTOR<ComputeParticleType*>;

private:
	MAINDESC													m_stMainDesc;
	DOCKDESC												m_stFireView;


	VECTOR<SHPTR<TImGuiView>>			m_CloseImGuies;
	SET<SHPTR<TImGuiView>>					m_OpenImGuies;

	_double														m_dShowDeltaTime;
	_bool															m_isInitSetting;


	/*_bool														m_isResetParticle;

	_int m_iMultipleParticleSize;
	PARTICLES m_MultipleParticle;
	PARTICLEPARAMS m_MultipleParticleParam;
	PARTICLETYPES m_MultipleParticleType;
	_int m_iCurActiveMultipleParticle;*/
	
	
	SHPTR <UFire> m_stFire;



};


END