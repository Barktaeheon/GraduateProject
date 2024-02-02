#pragma once

#include "UBase.h"


BEGIN(Engine)

using THREAD = std::thread;
/*
@ Date: 2024-02-02, Writer: ������
@ Explain
- Background �����峪 ��Ÿ �ٸ� �� �׶��� �۾��� �ʿ��� Ŭ������ ����
*/
class UWorkThread final : public UBase {
public:
	UWorkThread(const THREADFUNC& _threadFunc, void* _pReigsterPointer);
	NO_COPY(UWorkThread)
	DESTRUCTOR(UWorkThread)
public:
	void StartFunc();
private:
	virtual void Free() override;
private:
	SHPTR<THREAD>	m_spThread;
};

END

