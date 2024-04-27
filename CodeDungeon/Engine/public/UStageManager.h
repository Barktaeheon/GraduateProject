#pragma once 

#include "UBase.h"

BEGIN(Engine)
class UStage;

class UStageManager final : public UBase
{
public:
	UStageManager();
	NO_COPY(UStageManager)
	DESTRUCTOR(UStageManager)
	// UBase��(��) ���� ��ӵ�
	void Free() override;

	SHPTR<UStage> GetStage() { return m_spStage; }
	void Reset() { m_spStage.reset(); }

	_bool Load();
	_bool Save(const _wstring& _wstrPath);

private:
	SHPTR<UStage> m_spStage;
};

END