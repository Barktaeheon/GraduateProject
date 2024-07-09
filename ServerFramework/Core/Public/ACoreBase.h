#ifndef _SERVERFRAMEWORK_CORE_PUBLIC_ACOREBASE_H
#define _SERVERFRAMEWORK_CORE_PUBLIC_ACOREBASE_H

BEGIN(Core)
/*
@ Date: 2023-12-26
@ Writer: ������
@ Explain
- �ֻ��� �θ� Ŭ����, �������� ��ü�� ��Ȱ�� �ϴ� ��� Ŭ������ ��� �� Ŭ������ ������ ��� �޴´�.
*/
class  CORE_DLL  ACoreBase abstract : public Core::AEnableSharedFromThis<ACoreBase> {
protected:
	ACoreBase();
	ACoreBase(const ACoreBase& _rhs);
	virtual ~ACoreBase();

public:
	/*
	@ Explain
	- �ڱ� �ڽ��� WEAKPTR�� ĳ���� �ϱ� ���� �Լ����̴�. 
	*/
	template<class T>
	SHPTR<T> ThisShared() { return Core::static_shared_cast<T>(shared_from_this()); }
	template<class T>
	WKPTR<T> ThisWeak() { return Core::static_shared_cast<T>(weak_from_this()); }

	/*
	@ Explain
	- �޸𸮸� �����ؾ� �� ���� ���� �� FREE�� �����Ͽ� �����Ѵ�. 
	*/
protected:
	virtual void Free() PURE;

protected:
};

END

#endif // _SERVERFRAMEWORK_CORE_PUBLIC_ACOREBASE_H