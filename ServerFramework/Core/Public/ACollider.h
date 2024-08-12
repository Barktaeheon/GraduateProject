#ifndef _SERVERFRAMEWORK_CORE_PUBLIC_ACOLLIDER_H
#define _SERVERFRAMEWORK_CORE_PUBLIC_ACOLLIDER_H

#include "ACoreBase.h"

BEGIN(Core)
class ATransform;
/*
@ Date: 2023-01-18, Writer: ������
@ Explain
- ��ü�� �浹ó���� �ϱ� ���� Ŭ�����̴�. 
*/
class CORE_DLL ACollider final : public ACoreBase {
public:
	enum TYPE { TYPE_AABB, TYPE_OBB, TYPE_SPHERE, TYPE_END };
	ACollider(const TYPE& _eType, const COLLIDERDESC& _CollisionDesc);
	DESTRUCTOR(ACollider)
public:
	const Vector3& GetCurPos();
	const Vector3& GetScale();
	const Vector3& GetTranslate();
	const _float4x4& GetTransformMatrix() const { return m_mTransformMatrix; }

	void SetScale(const Vector3& _vScale);
	void SetScaleToFitModel(const Vector3& minVertex, const Vector3& maxVertex);
	void SetTranslate(const Vector3& _vTranslate);
	void SetTransform(const Vector3& _vPos, const Vector4& _vQuaternion);
	void SetTransform(CSHPTRREF< ATransform> _spTransform);
	void SetTransform(const _float4x4& _Matrix);
	// UComponent��(��) ���� ��ӵ�
	virtual void Free() override;
	_bool IsCollision(CSHPTRREF<ACollider> _pCollider);
	/*
	@ Date: 2024-03-11, Writer: �̼���
	@ Explain
	- collider�� ray�� �浹ó���� ���� �Լ�
	*/
	_bool IsCollisionWithRay(const Vector3& _vOrigin, const Vector3& _vDirection, _float* _pDist);

	/*
	@ Date: 2024-06-11, Writer: �̼���
	@ Explain
	- �ݶ��̴� �븻 �߰�
	*/
	void SetOBBNormals(const _float4x4& transformMatrix);
	Vector3 CalculateOBBCollisionNormal(const DirectX::BoundingOrientedBox& box1, const DirectX::BoundingOrientedBox& box2) const;
	Vector3 CalculateOBBSphereCollisionNormal(const DirectX::BoundingOrientedBox& box, const DirectX::BoundingSphere& sphere) const;
	Vector3 GetCollisionNormal(CSHPTRREF<ACollider> _pCollider);

	void GetBoundingOrientedBoxCorners(const CSHPTRREF<DirectX::BoundingOrientedBox> box, Vector3* Corners);
	Vector3 GetFurthestPointFromBoundingOrientedBoxCenter(const CSHPTRREF<DirectX::BoundingOrientedBox> box);
	Vector3 GetHeightAdjustedPointFromCenter(const CSHPTRREF<DirectX::BoundingOrientedBox> box, _bool minus);

	const SHPTR<DirectX::BoundingOrientedBox>& GetOBB() { return m_spOBB; }
private:
	SHPTR<DirectX::BoundingBox>						m_spAABB_Original;
	SHPTR<DirectX::BoundingOrientedBox>		m_spOBB_Original;
	SHPTR<DirectX::BoundingSphere>					m_spSphere_Original;

	SHPTR<DirectX::BoundingBox>						m_spAABB;
	SHPTR<DirectX::BoundingOrientedBox>		m_spOBB;
	SHPTR<DirectX::BoundingSphere>					m_spSphere;

	TYPE																		m_eType;
	_bool																		m_isCollision;
	_float4x4																m_mTransformMatrix;

	Vector3																	m_vModelScale;
	Vector3																	m_vCurScale;
	Vector3																	m_vTranslate;
	Vector3																	m_vPos;

	ARRAY<Vector3, 6>												m_vObbNormals;
};

END

#endif // _SERVERFRAMEWORK_CORE_PUBLIC_ACOLLIDER_H