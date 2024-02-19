#pragma once
#include "USound.h"
#include "UBase.h"

BEGIN(Engine)

/*
@ Date: 2024-02-18, Writer: ������
@ Explain
- Sound�� �����ϴ� SoundManager
*/
class USoundManager final : public UBase {
public:
	USoundManager();
	NO_COPY(USoundManager)
	DESTRUCTOR(USoundManager)
public:
	virtual void Free() override;
	HRESULT ReadySoundManager();
private:
	FMOD::System*		m_pSystem;
};


END