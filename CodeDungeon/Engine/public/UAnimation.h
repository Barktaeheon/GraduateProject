#pragma once
#include "UBase.h"

BEGIN(Engine)
class UAnimModel;
class UAnimChannel;
class UAnimEvent;
class UPawn;
/*
@ Date: 2024-02-17, Writer: ������
@ Explain
- Animation�� ��� Ŭ���� Channel���� VECTOR �����̳ʸ� ��� �ִ�. 
*/
class   UAnimation : public UBase{
	using CHANNELS = VECTOR<SHPTR<UAnimChannel>>;
	using ANIMFASTSECTIONS = VECTOR<ANIMFASTSECTION>;
	using ANIMEVENTCONTAINER = UNORMAP<ANIMEVENTTYPE, VECTOR<SHPTR<UAnimEvent>>>;
public:
	UAnimation();
	UAnimation(const UAnimation& _rhs);
	DESTRUCTOR(UAnimation)
public:
	const _wstring& GetAnimName() const { return m_wstrName; }
	const _bool IsFinishAnim() const { return m_isFinishAnimation; }
	const _bool IsSupplySituation() const { return m_isSupplySituation; }
	const _double& GetDuration() const { return m_dDuration; }
	const _double& GetTimeAcc() const { return m_dTimeAcc; }
	const _float& GetTotalAnimFastValue() const { return m_fTotalAnimationFastValue; }
	const VECTOR<ANIMFASTSECTION>& GetAnimFastSection() const { return m_AnimFastSections; }
	const ANIMEVENTCONTAINER& GetAnimEventContainer() const { return m_AnimEventContainer; }
	// Set Finish
	void SetSupplySituation(const _bool _isSupplySituation) { this->m_isSupplySituation = _isSupplySituation; }
	void SetAnimTimeAcc(const _double& _dTimeAcc) { this->m_dTimeAcc = _dTimeAcc; }
	void UpdateAnimFastSections(const _float _fTotalAnimFastValue, const VECTOR<ANIMFASTSECTION>& _AnimFastSection);

	void UpdateTimeAccToChannelIndex(const _double& _dTimeAcc);
public:
	SHPTR<UAnimation> Clone(CSHPTRREF<UAnimModel> _spAnimModel);
	virtual void Free() override;
	HRESULT NativeConstruct(CSHPTRREF<UAnimModel> _spAnimModel, const ANIMDESC& _stAnimDesc);
	// �ִϸ��̼ǰ� ����� �� �������� ������Ʈ
	void UpdateBoneMatrices(const _double& _dTimeDelta);
	// TimeAcc�� �ִϸ��̼ǰ� ����� �� �������� ������Ʈ
	void UpdateboneMatricesToTimeAcc( const _double& _TimeAcc);
	// ���� �ִϸ��̼����� ����
	void UpdateNextAnimTransformMatrices(const _double& _dTimeDelta, const _float _fSupplyValue, CSHPTRREF<UAnimation> _spAnimation);
	// Animation Event Tick 
	void TickAnimEvent(UPawn* _pPawn, UAnimModel* _pAnimModel, const _double& _TimeDelta, const _wstring& _wstrInputTrigger);
	void ResetData();
	// �ִϸ��̼� �̺�Ʈ�� ����ִ� �Լ�
	void InsertAnimEvent(ANIMEVENTTYPE _AnimEventType, CSHPTRREF<UAnimEvent> _spAnimEvent);
	// �ִϸ��̼� �̺�Ʈ�� �����ϴ� �Լ�
	void RemoveAnimEvent(CSHPTRREF<UAnimEvent> _spAnimEvent);
	// Save Sections
	void SaveAnimSectionData(const _wstring& _wstrPath);
	void SaveAnimSectionPathIsFolder(const _wstring& _wstrPath);
	void LoadAnimSectionData(const _wstring& _wstrPath);
	void LoadAnimSectionDataPathIsFolder(const _wstring& _wstrPath);
	// Save AnimEvent 
	void SaveAnimEventData(const _wstring& _wstrPath);
	void SaveAnimEventPathIsFolder(const _wstring& _wstrPath);
	void LoadAnimEventData(CSHPTRREF<UAnimModel> _spAnimModel, const _wstring& _wstrPath);
	void LoadAnimEventDataPathIsFolder(CSHPTRREF<UAnimModel> _spAnimModel, const _wstring& _wstrPath);
private:
	SHPTR<UAnimEvent> CreateAnimEvent(CSHPTRREF<UAnimModel> _spAnimModel, ANIMEVENTTYPE _AnimEventType, std::ifstream& _read);
private:

	static constexpr _float	MAX_SUPPLY_VALUE{1.f};

	CHANNELS							m_Channels;
	_wstring									m_wstrName;
	_uint										m_iNumChannels;
	_double									m_dTickPerSeconds;
	_double									m_dDuration;
	_double									m_dTimeAcc;
	_bool										m_isFinishAnimation;
	_bool										m_isSupplySituation;
	_float										m_fSupplySituationValue;

	ANIMFASTSECTIONS			m_AnimFastSections;
	_float										m_fTotalAnimationFastValue;
	// Animation Event 
	ANIMEVENTCONTAINER		m_AnimEventContainer;
};

END