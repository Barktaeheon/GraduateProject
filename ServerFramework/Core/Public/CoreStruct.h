#ifndef _SERVERFRAMEWORK_CORE_PUBLIC_CORESTRUCT_H
#define _SERVERFRAMEWORK_CORE_PUBLIC_CORESTRUCT_H

namespace Core {

	/*
	@ Date: 2024-04-07, Writer: ������
	@ Explain: ������ ���� ���� 
	*/
	struct SPACEINFO {
		Vector3		vCenter;
		Vector3		vExtents;
		_int				Depths;
		_int				Index;

		SPACEINFO() : vCenter{}, vExtents{}, Depths{ 0 }, Index{ 0 } {}
		SPACEINFO(const Vector3& _vCenter, const Vector3& _vExtents, _int _Depths, _int _SpaceIndex = 0) :
			vCenter{ _vCenter }, vExtents{ _vExtents }, Depths{ _Depths }, Index{ _SpaceIndex } {}
	};
	/*
	@ Date: 2024-06-15, Writer: ������
	@ Explain: �ݶ��̴��� ���� ���� ���� 
	*/
	struct COLLIDERINFO {
		_int							iColliderType;
		Vector3					vPos;
		Vector3					vScale;
	};
	/*
	@ Date: 2024-06-16, Writer: ������
	@ Explain: ĳ���� �������ͽ��� ���� ���� 
	*/
	struct CHARSTATUS
	{
		_float fPower;
		_float fDefensive;
		_float fHp;

		CHARSTATUS() : fPower{0.0f}, fDefensive{0.0f}, fHp{0.0f}{}
		CHARSTATUS(_float _fPower, _float _fDefensive, _float _fHp) : fPower{ _fPower }, fDefensive{ _fDefensive }, fHp{ _fHp } {}
	};

#pragma region ANIMMATIONFASTSECTION 

	typedef struct tagAnimFastSection
	{
		_float fStartSpot = -1.f;
		_float fEndSpot = -1.f;
		_float fFastValue = 1.f;

		void Convert(_double& _dValue, const _double& _dTimeAcc)
		{
			if (fStartSpot <= _dTimeAcc && _dTimeAcc <= fEndSpot)
				_dValue *= fFastValue;
		}
		_bool IsPass(const _double& _dTimeValue)
		{
			if (fStartSpot <= _dTimeValue && _dTimeValue <= fEndSpot)
				return true;

			return false;
		}
	}ANIMFASTSECTION;

#pragma endregion ANIMATIONFASTSECTION


#pragma region ANIMEVENTTYPE


	/*
	@ Date: 2024-02-10, Writer: ������
	@ Explain
	- �ִϸ��̼� �̺�Ʈ ����ü�� ���� ����ü
	*/
	struct ANIMEVENTDESC abstract {
	public:
		_bool							isActiveEvent;
		_string						strEventTrigger;
		_bool							isAnimChangeActive;

		ANIMEVENTDESC() : strEventTrigger{ "" }, isActiveEvent{ false }, isAnimChangeActive{ false }
		{
			strEventTrigger.resize(MAX_BUFFER_LENGTH);
		}
	};
	/*
	@ Date: 2024-02-10, Writer: ������
	@ Explain
	- �ִϸ��̼� Event�� ���� ������ �����ϴ� ����ü
	- �ִϸ��̼� Event�� ChangeDesc�� ��� �޾Ƽ� �� ������ ����Ѵ�.
	*/
	struct ANIMEVENTSECTIONDESC : public ANIMEVENTDESC {
		_double dStartTime;
		_double dEndTime;
		_double dStopTime;

		ANIMEVENTSECTIONDESC() : dStartTime{ 0.0 }, dEndTime{ 0.0 }, dStopTime{ 0.0 } {}

		_bool IsAnimEventActive(const _double& _dTimeAcc) const
		{
			if (_dTimeAcc >= dStartTime && _dTimeAcc <= dEndTime)
			{
				return true;
			}
			return false;
		}
	};
	/*
	@ Date: 2024-02-10, Writer: ������
	@ Explain
	- �ִϸ��̼� Event�� �߻��� ������ �����ϴ� ����ü
	- �ִϸ��̼� Event�� ChangeDesc�� ��� �޾Ƽ� �� ������ ����Ѵ�.
	*/
	struct ANIMOCURRESDESC : public ANIMEVENTDESC {
		_double		dAnimOccursTime;

		ANIMOCURRESDESC() : dAnimOccursTime{ 0.0 } {}

		bool IsAnimOcurrs(const _double& _dTimeAcc) const{
			if (_dTimeAcc >= dAnimOccursTime)
				return true;
			return false;
		}
	};
	/*
	@ Date: 2024-02-10, Writer: ������
	@ Explain
	-  �ִϸ��̼��� Event Desc ���� ������ Other Event�� ��� �ִ� ����ü��
		�ش� ����ü�� ���������ν�, �ٸ� POINTER���� ������ ������ ���ֱ� ���ؼ� ������
	*/
	struct ANIMOTHEREVENTDESC abstract { };

	/*
	@ Date: 2024-02-10, Writer: ������
	@ Explain
	-  �ִϸ��̼��� �������� �Ѿ�� ���� ����ü
	*/
	struct ANIMCHANGEDESC : public ANIMOTHEREVENTDESC {
		_int				iNextAnimIndex;
		_float			fSupplyAnimValue;
		_double		dNextAnimTimeAcc;
		_float			fAnimChangeTime;
		_float			fLastProgressValue;
		_bool			isEnableLastSettingAnim;

		ANIMCHANGEDESC() : iNextAnimIndex{ 0 }, fSupplyAnimValue{ 1.f }, dNextAnimTimeAcc{ 0.0 },
			fAnimChangeTime{ 0 }, fLastProgressValue{ 1.f }, isEnableLastSettingAnim{ false }
		{}
		ANIMCHANGEDESC(const _int _NextAnimIndex, const _float& _SupplyAnimValue, const _double& _dNextAnimTimeAcc) :
			iNextAnimIndex{ _NextAnimIndex }, fSupplyAnimValue{ _SupplyAnimValue }, dNextAnimTimeAcc{ _dNextAnimTimeAcc },
			fAnimChangeTime{ 0 }, fLastProgressValue{ 1.f }, isEnableLastSettingAnim{ false }
		{}
	};
	/*
	@ Date: 2024-02-21, Writer: ������
	@ Explain
	-  �ִϸ��̼� Ư�� ������ Collider�� ���̱� ���� ����ü�̴�.
	*/
	struct ANIMCOLLIDERDESC : public ANIMOTHEREVENTDESC {
		_int												iColliderType;

		SHPTR<class ACollider>			spCollider;
		SHPTR<class ABoneNode>	spBoneNode;

		Vector3										vColliderScale;
		Vector3										vColliderTranslation;

		ANIMCOLLIDERDESC() : iColliderType{ 0 },
			spCollider{ nullptr }, spBoneNode{ nullptr }, vColliderScale{ 1, 1, 1 }, vColliderTranslation{ 1, 1, 1 } {}
		ANIMCOLLIDERDESC(const _int _iColliderType, const Vector3& _vColliderScale, const Vector3& _vColliderTranslation) :
			iColliderType{ _iColliderType }, vColliderScale{ _vColliderScale }, vColliderTranslation{ _vColliderTranslation }, spBoneNode{ nullptr }, spCollider{ nullptr }
		{}
	};
	/*
	@ Date: 2024-02-21, Writer: ������
	@ Explain
	-  �ִϸ��̼� Ư�� ������ Collider�� ���̱� ���� ����ü�̴�.
	*/
	struct ANIMSOUNDDESC : public ANIMOTHEREVENTDESC {
		_string				strSoundName;
		Vector3				vSoundVelocity;
		_float					fMinSoundDistance;
		_float					fMaxSoundDistance;

		ANIMSOUNDDESC() : strSoundName{ "" }, vSoundVelocity{}, fMinSoundDistance{ 0.f }, fMaxSoundDistance{ 1.f } {}

	};

	// Key Frame
	typedef struct tagKeyFrame
	{
		_double		dTime{ 0.0 };

		Vector3	vScale;
		Vector4	vRotation;
		Vector4	vPosition;
	}KEYFRAME;

	struct TIMEREVENT {
		_llong					llObjID;
		_llong					llTargetID;
		TIMEPOINT		WakeUpTime;
		EVENT_TYPE eEventType;

		TIMEREVENT() : llObjID{ 0 }, llTargetID{ 0 }, WakeUpTime{ TIMEPOINT::min()}, eEventType{EV_END} {}
		TIMEREVENT(const _llong _llObjID, const _llong _llTargetID, EVENT_TYPE _eEventType)
			: llObjID{ _llObjID }, llTargetID{ _llTargetID }, WakeUpTime{ std::chrono::system_clock::now() }, eEventType{_eEventType}
		{}

		constexpr bool operator < (const TIMEREVENT& L) const
		{
			return (WakeUpTime > L.WakeUpTime);
		}
	};

#pragma endregion ANIMEVENTTYPE
}
#endif // _SERVERFRAMEWORK_CORE_PUBLIC_CORESTRUCT_H