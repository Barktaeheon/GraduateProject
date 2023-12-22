
#ifndef _PTH_FRAMEWORK_ENGIEN_PUBLIC_UBASE_H
#define _PTH_FRAMEWORK_ENGIEN_PUBLIC_UBASE_H

BEGIN(Engine)

/*
�ֻ��� Ŭ���� ��� Ŭ������ �ݵ�� �� Ŭ������ ���� Ŭ�������� �Ѵ�. 
���ܶ� �������� �ʴ´�. 
*/

class ENGINE_DLL UBase abstract : public std::enable_shared_from_this<UBase> {
protected:
	UBase();
	UBase(const UBase& _rhs) : enable_shared_from_this<UBase>(_rhs) {}
	virtual  ~UBase() { Free(); }
public:
	// ����
	virtual void Free() PURE;
protected:
	// �ڱ� �ڽ��� ĳ���� �ؼ� ����Ѵ�. 
	template<class T>
	std::shared_ptr<T> ThisShared() { return std::static_pointer_cast<T>(shared_from_this()); }
	template<class T>
	std::weak_ptr<T> ThisWeak() { return std::static_pointer_cast<T>(weak_from_this()); }
};

END

#endif // _PTH_FRAMEWORK_ENGIEN_PUBLIC_UBASE_H

