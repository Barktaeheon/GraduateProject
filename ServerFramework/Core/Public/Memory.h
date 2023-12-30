#ifndef _SERVERFRAMEWORK_CORE_PUBLIC_MEMORY_H
#define _SERVERFRAMEWORK_CORE_PUBLIC_MEMORY_H

#include "Allocator.h"

namespace Core {

	/*
	@ Date: 2023-12-30
	@ Writer: ������
	@ Explain: �⺻���� �޸� �Ҵ��� �ϰ� �����ڸ� �θ���. 
	*/
	template<class Type, class... Args>
	requires Convertible<Type, Args...>
	Type* BaseAlloc(Args&&... _args)
	{
		Type* memory = static_cast<Type*>(BaseAllocator::Alloc(sizeof(Type)));
		new(memory)Type(std::forward<Args>(_args)...);
		return memory;
	}

	template<class Type>
	void BaseDelete(Type* _obj)
	{
		_obj->~Type();
		BaseAllocator::Release(_obj);
	}
}


#endif // _SERVERFRAMEWORK_CORE_PUBLIC_MEMORY_H