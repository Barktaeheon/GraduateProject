#ifndef _SERVERFRAMEWORK_CORE_PUBLIC_USPHERECOLLIDER_H
#define _SERVERFRAMEWORK_CORE_PUBLIC_USPHERECOLLIDER_H

#include "UCollider.h"

BEGIN(Core)
/*
@ Date: 2023-01-19, Writer: ������
@ Explain
- ��ü�� SphereCollider�浹�� �ϱ� ���� Ŭ�����̴�.
*/
class CORE_DLL USphereCollider final : public UCollider {
public:
	USphereCollider(const Vector3 _vOriPos, const Vector3 _vOriScale);
	NO_COPY(USphereCollider)
	DESTRUCTOR(USphereCollider)
public:
	virtual void Tick(const _matrix	_WorldMatrix) override;
	virtual _bool IsCollision(SHPTR<UCollider> _spCollider) override;
private:
	virtual void Free() override;
};

END

#endif // _SERVERFRAMEWORK_CORE_PUBLIC_USPHERECOLLIDER_H