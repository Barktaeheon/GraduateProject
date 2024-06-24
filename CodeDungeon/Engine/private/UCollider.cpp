#include "EngineDefine.h"
using namespace DirectX;
#include "UCollider.h"
#include "UTransform.h"
#include "UMethod.h"
#include "UGameInstance.h"

#ifdef _USE_DEBUGGING
#include "UDefaultDebugging.h"
#endif

UCollider::UCollider(CSHPTRREF<UDevice> _pDevice,
	const TYPE& _eType)
	: UComponent(_pDevice), m_eType(_eType),
	m_spAABB_Original{ nullptr },
	m_spOBB_Original{ nullptr },
	m_spSphere_Original{ nullptr },
	m_spAABB{ nullptr },
	m_spOBB{ nullptr },
	m_spSphere{ nullptr },
	m_isCollision{ false },
	m_mTransformMatrix{ _float4x4::Identity },
	m_vCurScale{ 0.f, 0.f, 0.f },
	m_vModelScale{ 0.f, 0.f, 0.f },
	m_vTranslate{ 0.f, 0.f, 0.f },
	m_vPos{ 0.f, 0.f, 0.f },
	m_f3ObbNormals{}
#ifdef _USE_DEBUGGING
	, m_spDebugDrawPawn{ nullptr }
#endif
{
}

UCollider::UCollider(const UCollider& _rhs) :
	UComponent(_rhs), m_eType(_rhs.m_eType),
	m_spAABB_Original(_rhs.m_spAABB_Original),
	m_spOBB_Original(_rhs.m_spOBB_Original),
	m_spSphere_Original(_rhs.m_spSphere_Original),
	m_spAABB{ nullptr },
	m_spOBB{ nullptr },
	m_spSphere{ nullptr },
	m_mTransformMatrix{ _float4x4::Identity },
	m_isCollision{ false },
	m_vCurScale{ 0.f, 0.f, 0.f },
	m_vModelScale{0.f, 0.f, 0.f},
	m_vTranslate{ 0.f, 0.f, 0.f },
	m_vPos{ 0.f, 0.f, 0.f },
	m_f3ObbNormals{}
#ifdef _USE_DEBUGGING
	, m_spDebugDrawPawn{ nullptr }
#endif
{
}

const _float3& UCollider::GetCurPos()
{
	m_vPos = _float3::Zero;
	switch (m_eType)
	{
	case TYPE_AABB:
		if (nullptr != m_spAABB)
			m_vPos = m_spAABB->Center;
		break;
	case TYPE_OBB:
		if (nullptr != m_spOBB)
			m_vPos = m_spOBB->Center;
		break;
	case TYPE_SPHERE:
		if (nullptr != m_spSphere)
			m_vPos = m_spSphere->Center;
		break;
	}
	return m_vPos;
}

const _float3& UCollider::GetScale()
{
	return m_vCurScale;
}

const _float3& UCollider::GetTranslate()
{
	return m_vTranslate;
}

void UCollider::SetScale(const _float3& _vScale)
{
	switch (m_eType)
	{
	case TYPE_AABB:
		m_spAABB_Original->Extents = _vScale;
		m_vCurScale = _vScale;
		break;
	case TYPE_OBB:
		m_spOBB_Original->Extents = _vScale;
		m_vCurScale = _vScale;
		break;
	case TYPE_SPHERE:
		m_spSphere_Original->Radius = _vScale.x;
		m_vCurScale = { _vScale.x, _vScale.x, _vScale.x };
		break;
	}
	m_vModelScale = m_vCurScale;
}

void UCollider::SetScaleToFitModel(const _float3& minVertex, const _float3& maxVertex)
{
	_float3 size = (maxVertex - minVertex) * 0.5f;
	_float3 center = (maxVertex + minVertex) * 0.5f;

	switch (m_eType)
	{
	case TYPE_AABB:
		m_spAABB_Original->Center = center;
		m_spAABB_Original->Extents = size;
		m_vModelScale = size * 2;
		break;
	case TYPE_OBB:
		m_spOBB_Original->Center = center;
		m_spOBB_Original->Extents = size;
		m_vModelScale = size * 2;
		break;
	case TYPE_SPHERE:
		m_spSphere_Original->Radius = (size).x;
		m_vModelScale = { (size* 2).x, (size * 2).x, (size * 2).x };
		break;
	}
}

void UCollider::SetTranslate(const _float3& _vTranslate)
{
	switch (m_eType)
	{
	case TYPE_AABB:
		m_spAABB_Original->Center = _vTranslate;
		break;
	case TYPE_OBB:
		m_spOBB_Original->Center = _vTranslate;
		break;
	case TYPE_SPHERE:
		m_spSphere_Original->Center = _vTranslate;
		break;
	}
	m_vTranslate = _vTranslate;
}

void UCollider::SetTransform(const _float3& _vPos, const _float4& _vQuaternion)
{
	switch (m_eType)
	{
	case TYPE_AABB:
		m_mTransformMatrix = _float4x4::Identity;
		m_mTransformMatrix.Set_Pos(_vPos);
		m_spAABB_Original->Transform(*m_spAABB.get(), m_mTransformMatrix);
		break;
	case TYPE_OBB:
		m_mTransformMatrix = XMMatrixRotationQuaternion(_vQuaternion);
		m_mTransformMatrix.Set_Pos(_vPos);
		m_spOBB_Original->Transform(*m_spOBB.get(), m_mTransformMatrix);
		SetOBBNormals(m_mTransformMatrix);
		break;
	case TYPE_SPHERE:
		m_mTransformMatrix = XMMatrixRotationQuaternion(_vQuaternion);
		m_mTransformMatrix.Set_Pos(_vPos);
		m_spSphere_Original->Transform(*m_spSphere.get(), m_mTransformMatrix);
		break;
	}
}

void UCollider::SetTransform(CSHPTRREF<UTransform> _spTransform)
{
	RETURN_CHECK(nullptr == _spTransform, ;);
	switch (m_eType)
	{
	case TYPE_AABB:
		m_mTransformMatrix = _float4x4::Identity;
		m_mTransformMatrix.Set_Pos(_spTransform->GetPos());
		m_spAABB_Original->Transform(*m_spAABB.get(), m_mTransformMatrix);
		break;
	case TYPE_OBB:
		m_mTransformMatrix = _spTransform->GetWorldMatrix();
		m_spOBB_Original->Transform(*m_spOBB.get(), m_mTransformMatrix);
		m_vModelScale = _float3(m_spOBB->Extents) * 2;
		SetOBBNormals(m_mTransformMatrix);
		break;
	case TYPE_SPHERE:
		m_mTransformMatrix = _spTransform->GetWorldMatrix();
		m_spSphere_Original->Transform(*m_spSphere.get(), m_mTransformMatrix);
		m_vModelScale = _float3((m_spSphere->Radius, m_spSphere->Radius, m_spSphere->Radius)) * 2;
		break;
	}
}

void UCollider::SetTransform(const _float4x4& _Matrix)
{
	_float4x4 Matrix = m_mTransformMatrix * _Matrix;
	switch (m_eType)
	{
	case TYPE_AABB:
		m_spAABB_Original->Transform(*m_spAABB.get(), Matrix);
		m_vModelScale = _float3(m_spAABB->Extents) * 2;
		break;
	case TYPE_OBB:
		m_spOBB_Original->Transform(*m_spOBB.get(), Matrix);
		m_vModelScale = _float3(m_spOBB->Extents) * 2;
		SetOBBNormals(Matrix);
		break;
	case TYPE_SPHERE:
		m_spSphere_Original->Transform(*m_spSphere.get(), Matrix);
		m_vModelScale = _float3(m_spSphere->Radius) * 2;
		break;
	}
}



void UCollider::Free()
{
#ifdef _USE_DEBUGGING
	m_spDebugDrawPawn.reset();
#endif
}


HRESULT UCollider::NativeConstruct()
{
	switch (m_eType)
	{
	case TYPE_AABB:
		m_spAABB_Original = std::make_shared<DirectX::BoundingBox>(_float3::Zero, _float3(0.5f, 0.5f, 0.5f));
		break;
	case TYPE_OBB:
		m_spOBB_Original = std::make_shared<DirectX::BoundingOrientedBox>(_float3::Zero, _float3(0.5f, 0.5f, 0.5f), _float4(0.f, 0.f, 0.f, 1.f));
		break;
	case TYPE_SPHERE:
		m_spSphere_Original = std::make_shared<DirectX::BoundingSphere>(_float3::Zero, 0.5f);
		break;
	}

	return S_OK;
}

HRESULT UCollider::NativeConstructClone(const VOIDDATAS& _tDatas)
{
	RETURN_CHECK(_tDatas.size() <= 0, E_FAIL);
	COLLIDERDESC ColliderDesc = UMethod::ConvertTemplate_Index<COLLIDERDESC>(_tDatas, 0);

	if (nullptr != m_spAABB_Original)
		m_spAABB_Original = std::make_shared<DirectX::BoundingBox>(*m_spAABB_Original.get());
	if (nullptr != m_spOBB_Original)
		m_spOBB_Original = std::make_shared<DirectX::BoundingOrientedBox>(*m_spOBB_Original.get());
	if (nullptr != m_spSphere_Original)
		m_spSphere_Original = std::make_shared<DirectX::BoundingSphere>(*m_spSphere_Original.get());

	switch (m_eType)
	{
	case TYPE_AABB:
		m_spAABB_Original->Center = ColliderDesc.vTranslation;
		m_spAABB_Original->Extents = _float3(ColliderDesc.vScale.x, ColliderDesc.vScale.y, ColliderDesc.vScale.z);
		m_vCurScale = ColliderDesc.vScale;
		m_spAABB = std::make_shared<DirectX::BoundingBox>(*m_spAABB_Original.get());
		break;
	case TYPE_OBB:
		m_spOBB_Original->Center = ColliderDesc.vTranslation;
		m_spOBB_Original->Extents = _float3(ColliderDesc.vScale.x, ColliderDesc.vScale.y, ColliderDesc.vScale.z);
		m_vCurScale = ColliderDesc.vScale;
		m_spOBB = std::make_shared<DirectX::BoundingOrientedBox>(*m_spOBB_Original.get());
		break;
	case TYPE_SPHERE:
		m_spSphere_Original->Center = ColliderDesc.vTranslation;
		m_spSphere_Original->Radius = ColliderDesc.vScale.x;
		m_vCurScale = { ColliderDesc.vScale.x, ColliderDesc.vScale.x, ColliderDesc.vScale.x };
		m_spSphere = std::make_shared<DirectX::BoundingSphere>(*m_spSphere_Original.get());
		break;
	}
	m_vModelScale = m_vCurScale;

#ifdef _USE_DEBUGGING
	SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);
	UDefaultDebugging::DEBUGTYPE eDebugType{ UDefaultDebugging::DEBUGTYPE::DEBUG_END };
	switch (m_eType)
	{
	case TYPE_AABB:
	case TYPE_OBB:
		eDebugType = UDefaultDebugging::DEBUGTYPE::DEBUG_CUBE;
		break;
	case TYPE_SPHERE:
		eDebugType = UDefaultDebugging::DEBUGTYPE::DEBUG_SPHERE;
		break;
	}
	// Clone Actor

	VOIDDATAS vecDatas;
	vecDatas.push_back(&eDebugType);

	m_spDebugDrawPawn = static_pointer_cast<UDefaultDebugging>(spGameInstance->CloneActorAddAndNotInLayer(
		PROTO_ACTOR_DEUBGGINGDEFAULTOBJECT, vecDatas));
#endif

	return S_OK;
}

_bool UCollider::IsCollision(CSHPTRREF<UCollider> _pCollider)
{
	switch (m_eType)
	{
	case TYPE_AABB:
	{
		TYPE		eTargetType = _pCollider->m_eType;

		if (TYPE_AABB == eTargetType)
			m_isCollision = m_spAABB->Intersects(*_pCollider->m_spAABB.get());
		if (TYPE_OBB == eTargetType)
			m_isCollision = m_spAABB->Intersects(*_pCollider->m_spOBB.get());
		if (TYPE_SPHERE == eTargetType)
			m_isCollision = m_spAABB->Intersects(*_pCollider->m_spSphere.get());
	}
	break;
	case TYPE_OBB:
	{
		TYPE		eTargetType = _pCollider->m_eType;

		if (TYPE_AABB == eTargetType)
			m_isCollision = m_spOBB->Intersects(*_pCollider->m_spAABB.get());
		if (TYPE_OBB == eTargetType)
			m_isCollision = m_spOBB->Intersects(*_pCollider->m_spOBB.get());
		if (TYPE_SPHERE == eTargetType)
			m_isCollision = m_spOBB->Intersects(*_pCollider->m_spSphere.get());
	}
	break;
	case TYPE_SPHERE:
	{
		TYPE		eTargetType = _pCollider->m_eType;

		if (TYPE_AABB == eTargetType)
			m_isCollision = m_spSphere->Intersects(*_pCollider->m_spAABB.get());
		if (TYPE_OBB == eTargetType)
			m_isCollision = m_spSphere->Intersects(*_pCollider->m_spOBB.get());
		if (TYPE_SPHERE == eTargetType)
			m_isCollision = m_spSphere->Intersects(*_pCollider->m_spSphere.get());
	}
	break;

	}
	_pCollider->m_isCollision = m_isCollision;

	return m_isCollision;
}

_bool UCollider::IsCollisionWithRay(const _float3& _vOrigin, const _float3& _vDirection, _float* _pDist)
{
	switch (m_eType)
	{
	case TYPE_AABB:
	{
		m_isCollision = m_spAABB->Intersects(_vOrigin, _vDirection, *_pDist);
	}
	break;
	case TYPE_OBB:
	{
		m_isCollision = m_spOBB->Intersects(_vOrigin, _vDirection, *_pDist);
	}
	break;
	case TYPE_SPHERE:
	{
		m_isCollision = m_spSphere->Intersects(_vOrigin, _vDirection, *_pDist);
	}
	break;
	}

	return m_isCollision;
}

//_float3 UCollider::GetAABBCollisionNormal(const BoundingBox& box1, const BoundingBox& box2) const
//{
//	XMVECTOR Center1 = XMLoadFloat3(&box1.Center);
//	XMVECTOR Extents1 = XMLoadFloat3(&box1.Extents);
//
//	XMVECTOR Center2 = XMLoadFloat3(&box2.Center);
//	XMVECTOR Extents2 = XMLoadFloat3(&box2.Extents);
//
//	XMVECTOR Min1 = XMVectorSubtract(Center1, Extents1);
//	XMVECTOR Max1 = XMVectorAdd(Center1, Extents1);
//
//	XMVECTOR Min2 = XMVectorSubtract(Center2, Extents2);
//	XMVECTOR Max2 = XMVectorAdd(Center2, Extents2);
//
//	XMVECTOR overlap = XMVectorMin(XMVectorSubtract(Max1, Min2), XMVectorSubtract(Max2, Min1));
//	overlap = XMVectorMin(overlap, XMVectorSplatOne()); // Ensure overlap is within bounds
//
//	XMFLOAT3 overlapValues;
//	XMStoreFloat3(&overlapValues, overlap);
//
//	_float3 collisionNormal = _float3::Zero;
//	if (overlapValues.x < overlapValues.y && overlapValues.x < overlapValues.z)
//	{
//		collisionNormal = { 1.0f, 0.0f, 0.0f };
//	}
//	else if (overlapValues.y < overlapValues.x && overlapValues.y < overlapValues.z)
//	{
//		collisionNormal = { 0.0f, 1.0f, 0.0f };
//	}
//	else
//	{
//		collisionNormal = { 0.0f, 0.0f, 1.0f };
//	}
//
//	return collisionNormal;
//}

void UCollider::SetOBBNormals(const _float4x4& transformMatrix)
{
	_float3 right = XMVector3TransformNormal(_float3(1.0f, 0.0f, 0.0f), transformMatrix);
	_float3 up = XMVector3TransformNormal(_float3(0.0f, 1.0f, 0.0f), transformMatrix);
	_float3 forward = XMVector3TransformNormal(_float3(0.0f, 0.0f, 1.0f), transformMatrix);

	m_f3ObbNormals[0] = right;
	m_f3ObbNormals[1] = -right;
	m_f3ObbNormals[2] = up;
	m_f3ObbNormals[3] = -up;
	m_f3ObbNormals[4] = forward;
	m_f3ObbNormals[5] = -forward;
}

_float3 UCollider::CalculateOBBCollisionNormal(const DirectX::BoundingOrientedBox& box1, const DirectX::BoundingOrientedBox& box2) const
{
	XMVECTOR Axes[15];
	XMVECTOR Center1 = XMLoadFloat3(&box1.Center);
	XMVECTOR Center2 = XMLoadFloat3(&box2.Center);
	XMVECTOR Extents1 = XMLoadFloat3(&box1.Extents);
	XMVECTOR Extents2 = XMLoadFloat3(&box2.Extents);

	XMFLOAT4 quaternion1 = box1.Orientation;
	XMFLOAT4 quaternion2 = box2.Orientation;

	XMMATRIX Rot1 = XMMatrixRotationQuaternion(XMLoadFloat4(&quaternion1));
	XMMATRIX Rot2 = XMMatrixRotationQuaternion(XMLoadFloat4(&quaternion2));

	// OBB 1�� OBB 2�� ȸ�� ��Ŀ��� ���� �����մϴ�.
	for (int i = 0; i < 3; ++i)
	{
		Axes[i] = Rot1.r[i];
		Axes[i + 3] = Rot2.r[i];
	}

	// ��� ���� ������ ����Ͽ� �浹 ������ ���� ã���ϴ�.
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			Axes[6 + i * 3 + j] = XMVector3Cross(Axes[i], Axes[3 + j]);
		}
	}

	float minPenetration = FLT_MAX;  // �ּ� ħ������ �ʱ�ȭ�մϴ�.
	_float3 bestAxis = _float3::Zero;  // ���� ���� �ʱ�ȭ�մϴ�.

	// ��� �࿡ ���� �浹 �׽�Ʈ�� �����մϴ�.
	for (int i = 0; i < 15; ++i)
	{
		XMVECTOR axis = Axes[i];  // ���� ���� �����ɴϴ�.

		// ���� ���̰� 0�� �ƴϸ� ����ȭ�մϴ�.
		if (!XMVector3Equal(axis, XMVectorZero()))
		{
			axis = XMVector3Normalize(axis);  // ���� ����ȭ�մϴ�.

			// �� OBB�� �࿵(projection)�� ����մϴ�.
			float projection1 = fabs(XMVectorGetX(XMVector3Dot(axis, Rot1.r[0])) * XMVectorGetX(Extents1)) +
				fabs(XMVectorGetX(XMVector3Dot(axis, Rot1.r[1])) * XMVectorGetY(Extents1)) +
				fabs(XMVectorGetX(XMVector3Dot(axis, Rot1.r[2])) * XMVectorGetZ(Extents1));

			float projection2 = fabs(XMVectorGetX(XMVector3Dot(axis, Rot2.r[0])) * XMVectorGetX(Extents2)) +
				fabs(XMVectorGetX(XMVector3Dot(axis, Rot2.r[1])) * XMVectorGetY(Extents2)) +
				fabs(XMVectorGetX(XMVector3Dot(axis, Rot2.r[2])) * XMVectorGetZ(Extents2));

			// �� OBB�� �߽��� ������ �Ÿ��� ����մϴ�.
			float dot1 = XMVectorGetX(XMVector3Dot(Center1, axis));
			float dot2 = XMVectorGetX(XMVector3Dot(Center2, axis));
			float distance = fabs(dot1 - dot2);

			// �࿵�� �߽��� ������ �Ÿ��� ���Ͽ� ��ħ ���θ� Ȯ���մϴ�.
			float penetration = projection1 + projection2 - distance;

			if (penetration < 0.0f)
			{
				return _float3::Zero; // �浹�� ���� ���
			}

			// �ּ� ������ ���� ã���ϴ�.
			if (penetration < minPenetration)
			{
				minPenetration = penetration;
				bestAxis = _float3(XMVectorGetX(axis), XMVectorGetY(axis), XMVectorGetZ(axis));
			}
		}
	}

	return bestAxis;  // ���� ���� ��ȯ�մϴ�.
}


_float3 UCollider::GetOBBCollisionNormal(CSHPTRREF<UCollider> _pCollider)
{
	_float3 collisionNormal = _float3::Zero;

	switch (m_eType)
	{
	case TYPE_OBB:
	{
		if (TYPE_OBB == _pCollider->m_eType)
			collisionNormal = CalculateOBBCollisionNormal(*m_spOBB, *_pCollider->m_spOBB);
	}
	break;
	}

	return collisionNormal;
}

#ifdef _USE_DEBUGGING
void UCollider::ChangeColliderColor(const _float3& _vChange)
{
	RETURN_CHECK(nullptr == m_spDebugDrawPawn, ;);
	m_spDebugDrawPawn->SetColor(_vChange);
}


void UCollider::AddRenderer(RENDERID _eID)
{
	if (nullptr != m_spDebugDrawPawn)
	{
		switch (m_eType)
		{
		case TYPE_AABB:
			m_spDebugDrawPawn->GetTransform()->SetPos(m_spAABB->Center);
			m_spDebugDrawPawn->GetTransform()->SetScale(m_vModelScale);
			break;
		case TYPE_OBB:
			m_spDebugDrawPawn->GetTransform()->SetPos(m_spOBB->Center);
			m_spDebugDrawPawn->GetTransform()->SetScale(m_vModelScale);
			m_spDebugDrawPawn->GetTransform()->RotateFix(m_spOBB->Orientation);
			break;
		case TYPE_SPHERE:
			m_spDebugDrawPawn->GetTransform()->SetPos(m_spSphere->Center);
			m_spDebugDrawPawn->GetTransform()->SetScale(m_vCurScale);
			break;
		}
		m_spDebugDrawPawn->AddRenderer(_eID);
	}
}


#endif