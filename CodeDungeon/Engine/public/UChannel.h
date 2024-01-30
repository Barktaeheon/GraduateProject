#pragma once
#include "UBase.h"

BEGIN(Engine)
class UAnimModel;
class UAnimation;
class UBoneNode;

class  UChannel : public UBase {
public:
	UChannel();
	UChannel(const UChannel& _rhs);
	DESTRUCTOR(UChannel)
public:
	SHPTR<UChannel> Clone(CSHPTRREF<UAnimModel> _spAnimModel);
	virtual void Free() override;
	HRESULT NativeConstruct(CSHPTRREF<UAnimModel> _spAnimModel, const CHANNELDESC& _stChannelDesc);
	void UpdateTransformMatrix(const _double& _dTimeAcc, UAnimation* _pAnimation);
	void UpdatSupplyToCurAndNextTransformMatrix(const _double& _dTimeAcc, const _float _fRatio,
		UAnimation* _pAnimation, CSHPTRREF<UChannel> _spChannel);

	void ComputeCurKeyFrames(const _double& _dTimeAcc, _uint& _iCurKeyFrame);
	void ResetCurKeyFrames() { this->m_iCurrentKeyFrames = 0; }
private:
	static  constexpr inline _float4	ZERO_VALUE{ 0.f, 0.f, 0.f, 1.f };

	_wstring											m_wstrBoneName;
	_uint												m_iNumKeyMaxRealSize{ 0 };
	SHPTR<UBoneNode>					m_spBoneNode;
	_uint												m_iCurrentKeyFrames;
	VECTOR<KEYFRAME>		m_vecKeyFrames;
	_double											m_dCurTimeAcc;
	// ����ϴ� �͵�
	_float3											m_vScale;
	_float4											m_vRotation;
	_float4											m_vPosition;
};

END
