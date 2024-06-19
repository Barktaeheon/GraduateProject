#ifndef _SERVERFRAMEWORK_CORE_PUBLIC_AAABBCOLLIDER_H
#define _SERVERFRAMEWORK_CORE_PUBLIC_AAABBCOLLIDER_H

#include "ACollider.h"

BEGIN(Core)
/*
@ Date: 2023-01-18, Writer: ������
@ Explain
- ��ü�� AABB�浹�� �ϱ� ���� Ŭ�����̴�.
*/
class CORE_DLL AAABBCollider final : public ACollider {
public:
	AAABBCollider(const Vector3 _vOriPos, const Vector3 _vOriScale);
	NO_COPY(AAABBCollider)
	DESTRUCTOR(AAABBCollider)
public:
	virtual void Tick(const _matrix	_WorldMatrix) override;
	virtual _bool IsCollision(SHPTR<ACollider> _spCollider) override;

private:
	virtual void Free() override;
};

END

#endif // _SERVERFRAMEWORK_CORE_PUBLIC_AAABBCOLLIDER_H