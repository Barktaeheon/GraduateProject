#pragma once

#include "UClientService.h"

class CACHE_ALGIN CClientAdiminstor final : public UClientService {
public:
	CClientAdiminstor(OBJCON_CONSTRUCTOR);
	NO_COPY(CClientAdiminstor)
	DESTRUCTOR(CClientAdiminstor)
public:
	virtual bool NativeConstruct() override;
	virtual _bool Start() override;
protected:
	// UServerService��(��) ���� ��ӵ�
	virtual void Connect() override;
	virtual void Tick() override;

	static void ThreadFunc(void* _Service);
private:
	void Free() override;

private:
	_uint		m_iMaxSessionCount;

	ARRAY< VECTOR<SHPTR<class CClientSession>>, TLS::MAX_THREAD> vecSessions;
};

