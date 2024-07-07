#include "CoreDefines.h"
#include "AAnimator.h"
#include "ABoneNode.h"
#include "ARootBoneNode.h"
#include "AAnimation.h"
#include "ACollider.h"
#include "ATransform.h"

namespace Core
{
	AAnimator::AAnimator(SHPTR<ACoreInstance> _spCoreInstance, const _string& _strFolderPath, const _string& _strFileName) :
		ACoreObject(_spCoreInstance),  m_iCurAnimIndex{0}, m_iNextAnimIndex{0}, m_PivotMatirx{_float4x4::Identity}, m_fSupplyLerpValue{0},
	m_isCanAttackSituation{false}, m_BoneContainer{}, m_BoneSelector{}, m_AnimContainer{}, m_AnimSelector{},
	m_spRootBoneNode{nullptr},	m_spCurAnimation{nullptr},	m_spNextAnimation{nullptr},	m_AnimEventColliderContainer{},
		m_spAttackCollisionCollider{nullptr}
	{
		_string RealPath = _strFolderPath +  _strFileName;
		std::ifstream read{ RealPath, std::ios::binary };

		size_t boneNodeCnt = 0;
		size_t AnimCnt = 0;
		_int AnimIndex = 0;

		read.read((_char*)&boneNodeCnt, sizeof(size_t));
		read.read((_char*)&AnimCnt, sizeof(size_t));
		read.read((_char*)&AnimIndex, sizeof(_int));
		// Root Bone �Ҵ�
		for (_int i = 0; i < boneNodeCnt; ++i)
		{
			SHPTR<ABoneNode> spBoneNode = nullptr;
			if (AnimIndex == i)
			{
				m_spRootBoneNode = CreateInitNative<ARootBoneNode>(read);
				spBoneNode = m_spRootBoneNode;
			}
			else
			{
				spBoneNode = CreateInitNative<ABoneNode>(read);
			}
			m_BoneContainer.push_back(spBoneNode);
			m_BoneSelector.emplace(MakePair(spBoneNode->GetName(), i));
		}

		for (_int i = 0; i < AnimCnt; ++i)
		{
			SHPTR<AAnimation> spAnimation = CreateInitNative<AAnimation>(this, read);
			spAnimation->LoadAnimEventData(this, _strFolderPath);
			spAnimation->LoadAnimSectionData(_strFolderPath);
			m_AnimContainer.push_back(spAnimation);
			m_AnimSelector.emplace(MakePair(spAnimation->GetAnimName(), i));
		}

		ACollider::COLLIDERDESC Desc{};
		m_AnimEventColliderContainer.emplace(MakePair(ACollider::TYPE_AABB, Create<ACollider>(ACollider::TYPE_AABB, Desc)));
		m_AnimEventColliderContainer.emplace(MakePair(ACollider::TYPE_OBB, Create<ACollider>(ACollider::TYPE_OBB, Desc)));
		m_AnimEventColliderContainer.emplace(MakePair(ACollider::TYPE_SPHERE, Create<ACollider>(ACollider::TYPE_SPHERE, Desc)));

		SetAnimation(0);
	}

	AAnimator::AAnimator(const AAnimator& _rhs) : 
		ACoreObject(_rhs), m_iCurAnimIndex{ 0 }, m_iNextAnimIndex{ 0 }, m_PivotMatirx{ _float4x4::Identity }, m_fSupplyLerpValue{ 0 },
		m_isCanAttackSituation{ false }, m_BoneContainer{}, m_BoneSelector{}, m_AnimContainer{}, m_AnimSelector{},
		m_spRootBoneNode{ nullptr }, m_spCurAnimation{ nullptr }, m_spNextAnimation{ nullptr }, m_AnimEventColliderContainer{},
		m_spAttackCollisionCollider{ nullptr }
	{
	}


	void AAnimator::TickEvent(APawn* _pPawn, const _string& _strInputTrigger, const _double& _TimeDelta)
	{
		m_spCurAnimation->TickAnimEvent(_pPawn, this, _TimeDelta, _strInputTrigger);
	}

	void AAnimator::TickEventToRatio(APawn* _pPawn, const _string& _strInputTrigger, const _double& _dRatio, const _double& _TimeDelta)
	{
		m_spCurAnimation->TickAnimEvent(_pPawn, this, _TimeDelta, _dRatio, _strInputTrigger);
	}

	void AAnimator::TickAnimation(const _double& _dTimeDelta)
	{
		if (nullptr != m_spNextAnimation)
		{
			m_spCurAnimation->UpdateNextAnimTransformMatrices(_dTimeDelta, m_fSupplyLerpValue, m_spNextAnimation);
			if (false == m_spCurAnimation->IsSupplySituation())
			{
				m_spCurAnimation = m_spNextAnimation;
				m_spNextAnimation = nullptr;
			}
		}
		else
		{
			m_spCurAnimation->UpdateBoneMatrices(_dTimeDelta);
		}

		/* �θ�κ��� �ڽĻ����� �������� �����Ѵ�.(CombinedTransformationMatrix) */
		{
			for (auto& BoneNode : m_BoneContainer)
				BoneNode->UpdateCombinedMatrix(m_PivotMatirx);
		}
	}

	void AAnimator::TickAnimChangeTransform(CSHPTRREF<ATransform> _spTransform, const _double& _dTimeDelta)
	{
		ASSERT_CRASH(nullptr != _spTransform && nullptr != m_spCurAnimation);
		TickAnimation(_dTimeDelta);

		RETURN_CHECK(true == m_spCurAnimation->IsSupplySituation(), ;);

		if (true == m_spCurAnimation->IsFinishAnim())
		{
			m_spRootBoneNode->ResetRootBoneInfo();
			return;
		}

		if (true == m_spCurAnimation->IsApplyRootBoneMove())
		{
			Vector3 Position = Vector3::TransformCoord(m_spRootBoneNode->GetMoveRootBonePos(), m_PivotMatirx * _spTransform->GetWorldMatrix());
			_spTransform->SetPos(Position);
		}
	}

	void AAnimator::TickAnimToTimeAccChangeTransform(CSHPTRREF<ATransform> _spTransform, const _double& _dTimeDelta, const _double& _TimeAcc)
	{
		ASSERT_CRASH(nullptr != _spTransform && nullptr != m_spCurAnimation);
		UpdateCurAnimationToRatio(_TimeAcc);

		if (true == m_spCurAnimation->IsApplyRootBoneMove())
		{
			Vector3 Position = Vector3::TransformCoord(m_spRootBoneNode->GetMoveRootBonePos(), m_PivotMatirx * _spTransform->GetWorldMatrix());
			_spTransform->SetPos(Position);
		}
	}

	void AAnimator::UpdateCurAnimationToRatio(const _double& _dRatio)
	{
		m_spCurAnimation->UpdateboneMatricesToRatio(_dRatio);
		/* �θ�κ��� �ڽĻ����� �������� �����Ѵ�.(CombinedTransformationMatrix) */
		{
			for (auto& BoneNode : m_BoneContainer)
				BoneNode->UpdateCombinedMatrix(m_PivotMatirx);
		}
	}

	SHPTR<ABoneNode> AAnimator::FindBoneNode(const _string& _strBoneNode) const
	{
		const auto& iter = m_BoneSelector.find(_strBoneNode);
		RETURN_CHECK(m_BoneSelector.end() == iter, nullptr);
		return m_BoneContainer[iter->second];
	}

	void AAnimator::UpdateAttackData(const _bool _isCanAttackSituation, CSHPTRREF<ACollider> _spCollider)
	{
		m_isCanAttackSituation = _isCanAttackSituation;
		m_spAttackCollisionCollider = _spCollider;
	}

	void AAnimator::ResetCurAnimEvent()
	{
		if (nullptr != m_spCurAnimation)
			m_spCurAnimation->ResetAnimChangeEventNode();
	}

	void AAnimator::SetAnimation(const _int& _iAnimIndex)
	{
		ChangeAnimIndex(_iAnimIndex, m_iCurAnimIndex);
		// ���� �ִϸ��̼��� ���õǴ� ��Ȳ�� ���� �Լ� ����
		SettingCurAnimSituation();
	}

	void AAnimator::SetAnimation(const _string& _strAnimName)
	{
		ChangeAnimIndex(_strAnimName, m_iCurAnimIndex);
		// ���� �ִϸ��̼��� ���õǴ� ��Ȳ�� ���� �Լ� ����
		SettingCurAnimSituation();
	}

	void AAnimator::SetAnimation(const _int& _iAnimIndex, const _double& _dNextTimeAcc)
	{
		SetAnimation(_iAnimIndex);
		// Change Time Acc
		m_spCurAnimation->UpdateTimeAccToChannelIndex(_dNextTimeAcc);
	}

	void AAnimator::SetAnimation(const _string& _strAnimName, const _double& _dNextTimeAcc)
	{
		SetAnimation(_strAnimName);
		// Change Time Acc
		m_spCurAnimation->UpdateTimeAccToChannelIndex(_dNextTimeAcc);
	}

	void AAnimator::ChangeAnimation(const _int& _iAnimIndex)
	{
		ChangeAnimation(_iAnimIndex, 0.0);
	}

	void AAnimator::ChangeAnimation(const _string& _strAnimName)
	{
		ChangeAnimation(_strAnimName, 0.0);
	}

	void AAnimator::ChangeAnimation(const _int& _iAnimIndex, const _double& _dNextTimeAcc)
	{
		ChangeAnimIndex(_iAnimIndex, m_iNextAnimIndex);
		// ���� �ִϸ��̼��� ���õǴ� ��Ȳ�� ���� �Լ� ����
		SettingNextAnimSituation();
		// Change Time Acc
		m_spNextAnimation->UpdateTimeAccToChannelIndex(_dNextTimeAcc);
	}

	void AAnimator::ChangeAnimation(const _string& _strAnimName, const _double& _dNextTimeAcc)
	{
		ChangeAnimIndex(_strAnimName, m_iCurAnimIndex);
		// ���� �ִϸ��̼��� ���õǴ� ��Ȳ�� ���� �Լ� ����
		SettingNextAnimSituation();
		// Change Time Acc
		m_spNextAnimation->UpdateTimeAccToChannelIndex(_dNextTimeAcc);
	}

	SHPTR<ACollider> AAnimator::BringAttackCollider(_int _iColliderType)
	{
		const auto& iter = m_AnimEventColliderContainer.find(_iColliderType);
		RETURN_CHECK(m_AnimEventColliderContainer.end() == iter, nullptr);
		m_spAttackCollisionCollider = iter->second;
		return iter->second;
	}

	_bool AAnimator::IsCollisionAttackCollider(SHPTR<ACollider> _spEnemyCollider)
	{
		RETURN_CHECK(false == m_isCanAttackSituation, false);
		RETURN_CHECK(nullptr == m_spAttackCollisionCollider, false);
		return m_spAttackCollisionCollider->IsCollision(_spEnemyCollider);
	}

	void AAnimator::ChangeAnimIndex(const _int& _iAnimIndex, _int& _iIndex)
	{
		if (m_AnimContainer.size() <= _iAnimIndex)
		{
			_iIndex = 0;
		}
		else
		{
			_iIndex = _iAnimIndex;
		}
	}

	void AAnimator::ChangeAnimIndex(const _string& _strAnimName, _int& _iIndex)
	{
		const auto& FindIterator = m_AnimSelector.find(_strAnimName);
		if (m_AnimSelector.end() == FindIterator)
		{
			_iIndex = 0;
		}
		else
		{
			_iIndex = FindIterator->second;
		}
	}

	void AAnimator::SettingCurAnimSituation()
	{
		ASSERT_CRASH(nullptr != m_AnimContainer[m_iCurAnimIndex]);
		m_spCurAnimation = m_AnimContainer[m_iCurAnimIndex];
		m_spCurAnimation->ResetData();
		m_spNextAnimation = nullptr;

		if (nullptr != m_spRootBoneNode)
			m_spRootBoneNode->ResetRootBoneInfo();
	}

	void AAnimator::SettingNextAnimSituation()
	{
	}

	void AAnimator::Free()
	{
	}
}