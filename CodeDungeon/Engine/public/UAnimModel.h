#pragma once
#include "UModel.h"

BEGIN(Engine)
class UShaderConstantBuffer;
class UAnimation;
class UTexture;
class UTransform;
class UPawn;
class UMeshFilter;
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
	CLONE_MACRO(UAnimModel, "UAnimModel::Clone To Failed")
	virtual void Free() override;
	virtual HRESULT NativeConstruct() override;
	using UModel::NativeConstruct;
	// �ش� PATH�� �� �ε�
	HRESULT NativeConstruct(const _wstring& _wstrPath, const _float4x4& _PivotMatrix);
	// �ش� ������ ���� �����ͷ� �ε�
	HRESULT NativeConstruct(CSHPTRREF<FILEGROUP> _spFileGroup, CSHPTRREF<FILEDATA> _spFileData, const _float4x4& _PivotMatrix);
	// PATH�� �Է��ϰ� ���� �̸��� �Է��ϸ� ã�Ƽ� �´�. 
	HRESULT NativeConstruct(const PATHS& _vecPaths, const _wstring& _wstrFileName, const _float4x4& _PivotMatrix);
	// ���� ���� �̸�, ���� �̸��� �Է��ϸ� ã�Ƽ� �� �ε�
	HRESULT NativeConstruct(const _wstring& _wstrModelFolder, const _wstring& _wstrFileName, const _float4x4& _PivotMatrix);

	virtual HRESULT NativeConstructClone(const VOIDDATAS& _vecDatas) override;
	// Tick Event 
	void TickEvent(UPawn* _pPawn, const _wstring& _wstrInputTrigger, const _double& _TimeDelta);
	// �ִϸ��̼��� timedelta�� �帧�� ���� �����ϴ� �Լ� (TimeDelta�� ���� ���ӿ��� �帣�� �ð�)
	void TickAnimation(const _double& _dTimeDelta);
	// �ִϸ��̼��� timeAcc�� ���� �����ϴ� �Լ� (TimAcc�� �ִϸ��̼� ���)
	void UpdateCurAnimationToTimeAcc(const _double& _dTimeAcc);
	// �ִϸ��̼��� Tick�ϸ鼭 Event�� Tick�ϴ� �Լ�
	void TickAnimChangeTransform(CSHPTRREF<UTransform> _spTransform, const _double& _dTimeDelta);
	// ���� �ִϸ��̼��� TimAcc ���� ���� �����ϴ� �Լ� Evnet�� TimeAcc�� ���� �����Ѵ�. 
	void TickAnimToTimAccChangeTransform(CSHPTRREF<UTransform> _spTransform, const _double& _dTimeDelta, const _double& _TimeAc);
	// �ִϸ��̼��� �����ϴ� �Լ�
	virtual HRESULT Render(const _uint _iMeshIndex, CSHPTRREF<UShader> _spShader, CSHPTRREF<UCommand> _spCommand) override;
	// Set Animation
	void SetAnimation(const _uint& _iAnimIndex);
	void SetAnimation(const _wstring& _wstrAnimName);
	// Change Animation
	void ChangeAnimation(const _uint& _iAnimIndex);
	void ChangeAnimation(const _wstring& _wstrAnimName);
	// Change Animation, Apply Next Time Acc 
	void ChangeAnimation(const _uint& _iAnimIndex, const _double& _dNextTimeAcc);
	void ChangeAnimation(const _wstring& _wstrAnimName, const _double& _dNextTimeAcc);
	void OnShowOriginAnimation();
	void OnAdjustTransformToAnimation();
	void ResetCurAnimEvent();
public: /* get set */
	ANIMATIONS& GetAnimations() { return m_vecAnimations; }
	const ANIMATIONPARAM& GetAnimParam() const { return m_stAnimParam; }
	const ANIMSTRINGS& GetAnimStrings() const { return m_AnimNamesGroup; }
	CSHPTRREF<UAnimation> GetCurrentAnimation() const { return m_spCurAnimation; }
	CSHPTRREF<UAnimation> GetNextAnimation() const { return m_spNextAnimation; }
	CSHPTRREF<UMeshFilter> GetMeshFilter() const { return m_spMeshFilterController; }
	const _float GetSupplyLerpValue() const { return m_fSupplyLerpValue; }
	const _int GetCurrentAnimIndex() const { return m_iCurAnimIndex; }
	const _int GetNetAnimIndex() const { return m_iNextAnimIndex; }
	_bool IsChangeAnimation() const { return m_isChangeAnim; }
	_bool IsChangedAnimation() const { return m_spNextAnimation == nullptr; }

	const _float4x4& GetPivotMatirx() const { return m_mPivotMatrix; }

	void SetSupplyLerpValue(const _float _fSupplyLerpValue) { this->m_fSupplyLerpValue = _fSupplyLerpValue; }
	void SetAnimParam(const ANIMATIONPARAM& _stAnimParam) { this->m_stAnimParam = _stAnimParam; }
private:
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
	_float4x4													m_mPivotMatrix;

	SHPTR<UMeshFilter>								m_spMeshFilterController;								
};

END
