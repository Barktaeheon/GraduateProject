#ifndef _SERVERFRAMEWORK_CORE_PUBLIC_UBASE_H
#define _SERVERFRAMEWORK_CORE_PUBLIC_UBASE_H

BEGIN(Core)
/*
@ Date: 2023-12-26
@ Writer: ������
@ Explain:  �ֻ��� �θ� Ŭ����, �������� ��ü�� ��Ȱ�� �ϴ� ��� Ŭ������ ��� �� Ŭ������ ������ ��� �޴´�.
*/
class  CACHE_ALGIN  UBase abstract : public Core::PTH::UEnableSharedFromThis<UBase> {
public:
	UBase();
	UBase(const UBase& _rhs);
	virtual ~UBase();

protected:
	/*
	@ Date: 2023-12-26
	@ Writer: ������
	@ Explain:  �ڱ� �ڽ��� WEAKPTR�� ĳ���� �ϱ� ���� �Լ����̴�. 
	*/
	template<class T>
	SHPTR<T> ThisShared() { return Core::PTH::static_shared_cast<T>(shared_from_this()); }
	template<class T>
	WKPTR<T> ThisWeak() { return Core::PTH::static_shared_cast<T>(weak_from_this()); }

	/*
	@ Date: 2023-12-26
	@ Writer: ������
	@ Explain:  �޸𸮸� �����ؾ� �� ���� ���� �� FREE�� �����Ͽ� �����Ѵ�. 
	*/
	virtual void Free() PURE;
};

END

#endif // _SERVERFRAMEWORK_CORE_PUBLIC_UBASE_H