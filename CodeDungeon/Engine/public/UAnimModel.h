#pragma once
#include "UModel.h"

BEGIN(Engine)
class UShaderConstantBuffer;
class UAnimation;
class UTexture;
class UTransform;
/*
@ Date: 2024-02-04, Writer: ������
@ Explain
- ���� �޽�, �� ����, ���� �ؽ��� �������� VECTOR �����̳ʵ�� ��� �ִ� Ŭ�����̴�.
�ִϸ��̼� ������ ��� �־ �ִϸ��̼��� ������ �� �ִ�.
*/
class  UAnimModel : public UModel{
	using ANIMATIONS = VECTOR<SHPTR<UAnimation>>;
	using SETUPBONEMATRIXES = ARRAY<_float4x4, MAX_BONE_SIZE>;
	using ANIMSTRINGS = UNORMAP<_wstring, _uint>;
public:
	UAnimModel(CSHPTRREF<UDevice> _spDevice);
	UAnimModel(const UAnimModel& _rhs);
	DESTRUCTOR(UAnimModel)
public:
	ANIMATIONS& GetAnimations() { return m_vecAnimations; }
	const ANIMATIONPARAM& GetAnimParam() const { return m_stAnimParam; }
	const ANIMSTRINGS& GetAnimStrings() const { return m_AnimNamesGroup; }
	CSHPTRREF<UAnimation> GetCurrentAnimation() const { return m_spCurAnimation; }
	CSHPTRREF<UAnimation> GetNextAnimation() const { return m_spNextAnimation; }
	const _float GetSupplyLerpValue() const { return m_fSupplyLerpValue; }
	const _int GetCurrentAnimIndex() const { return m_iCurAnimIndex; }
	const _int GetNetAnimIndex() const { return m_iNextAnimIndex; }
	_bool IsChangeAnimation() const { return m_isChangeAnim; }

	void SetSupplyLerpValue(const _float _fSupplyLerpValue) { this->m_fSupplyLerpValue = _fSupplyLerpValue; }
	void SetAnimParam(const ANIMATIONPARAM& _stAnimParam) { this->m_stAnimParam = _stAnimParam; }
public:
	CLONE_MACRO(UAnimModel, "UAnimModel::Clone To Failed")
	virtual void Free() override;
	virtual HRESULT NativeConstruct() override;
	using UModel::NativeConstruct;
	virtual HRESULT NativeConstructClone(const VOIDDATAS& _vecDatas) override;
	// �ִϸ��̼��� timedelta�� �帧�� ���� �����ϴ� �Լ� (TimeDelta�� ���� ���ӿ��� �帣�� �ð�)
	void TickAnimation(const _double& _dTimeDelta);
	// �ִϸ��̼��� timeAcc�� ���� �����ϴ� �Լ� (TimAcc�� �ִϸ��̼� ���)
	void UpdateCurAnimationToTimeAcc(const _double& _dTimeAcc);
	// �ִϸ��̼��� Tick�ϸ鼭 Event�� Tick�ϴ� �Լ�
	void TickAnimationEvent(CSHPTRREF<UTransform> _spTransform, const _double& _dTimeDelta);
	// ���� �ִϸ��̼��� TimAcc ���� ���� �����ϴ� �Լ� Evnet�� TimeAcc�� ���� �����Ѵ�. 
	void UpdateCurAnimationToTimAccEvent(CSHPTRREF<UTransform> _spTransform, const _double& _dTimeDelta, const _double& _TimeAcc);
	// �ִϸ��̼��� �����ϴ� �Լ�
	virtual HRESULT Render(const _uint _iMeshIndex, CSHPTRREF<UShader> _spShader, CSHPTRREF<UCommand> _spCommand) override;
	// Set Animation
	void SetAnimation(const _uint& _iAnimIndex);
	void SetAnimation(const _wstring& _wstrAnimName);
	// Change Animation
	void ChangeAnimation(const _uint& _iAnimIndex);
	void ChangeAnimation(const _wstring& _wstrAnimName);
private:
	void UpdateOwnerActorPos(CSHPTRREF<UTransform> _spTransform);
	// CreateAnimation
	HRESULT CreateAnimation(const VECTOR<ANIMDESC>& _convecAnimDesc, const _wstring& _wstrPath);
	// Load To Data
	virtual void LoadToData(const _wstring& _wstrPath) override;
	void LoadAnimMeshData(std::ifstream& _ifRead, VECTOR<ANIMMESHDESC>& _convecMeshes);
	void LoadAnimationData(std::ifstream& _ifRead, VECTOR<ANIMDESC>& _convecAnimes);

	void ChangeAnimIndex(const _uint& _iAnimIndex, _uint& _iIndex);
	void ChangeAnimIndex(const _wstring& _wstrAnimName, _uint& _iIndex);
	void SettingCurAnimSituation();
	void SettingNextAnimSituation();
	HRESULT CreateShaderConstantBuffer();
private:
	ANIMATIONS												m_vecAnimations;
	ANIMSTRINGS											m_AnimNamesGroup;
	VECTOR<SETUPBONEMATRIXES>		m_vecSetupBonMatrix;
	// Animations
	SHPTR< UShaderConstantBuffer>		m_spAnimShaderConstantBuffer;
	ANIMATIONPARAM									m_stAnimParam;
	// Bone, PrevBone
	SHPTR<UShaderConstantBuffer>		m_spBoneMatrixShaderConstantBuffer;
	SHPTR<UShaderConstantBuffer>		m_spPrevBoneMatrixShaderConstantBuffer;
	//UpLoadResource
	ComPtr<Dx12Resource>						m_cpUpLoadResource;

	SHPTR<UAnimation>								m_spCurAnimation;
	SHPTR<UAnimation>								m_spNextAnimation;

	_uint															m_iCurAnimIndex;
	_uint															m_iNextAnimIndex;
	_float															m_fSupplyLerpValue;
	_bool															m_isChangeAnim;
};

END
