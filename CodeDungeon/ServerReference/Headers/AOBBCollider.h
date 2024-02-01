#ifndef _SERVERFRAMEWORK_CORE_PUBLIC_AOBBCOLLIDER_H
#define _SERVERFRAMEWORK_CORE_PUBLIC_AOBBCOLLIDER_H

#include "ACollider.h"

BEGIN(Core)

/*
@ Date: 2023-01-19, Writer: ������
@ Explain
- ��ü�� OBB�浹�� �ϱ� ���� Ŭ�����̴�.
*/
class  CORE_DLL  AOBBCollider final : public ACollider {
public:
	enum { PROJ_AXIS_CNT = 15};
public:
	AOBBCollider(const Vector3 _vOriPos, const Vector3 _vOriScale);
	NO_COPY(AOBBCollider)
	DESTRUCTOR(AOBBCollider)
public:
	virtual void Tick(const _matrix	_WorldMatrix) override;
	virtual _bool IsCollision(SHPTR<ACollider> _spCollider) override;
protected:
	// OBB�� �࿡ �����Ͽ� ������ ����ϴ� �Լ��̴�. 
	_float ProjectionOBBToAxis(const Vector3& _vPos, const Vector3& _vScale,
		const Vector3& _vAxis);
private:
	virtual void Free() override;

};

END

#endif  // _SERVERFRAMEWORK_CORE_PUBLIC_AOBBCOLLIDER_H