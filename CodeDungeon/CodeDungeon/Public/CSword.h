#pragma once
#include "UWeapon.h"

BEGIN(Engine)
class UGameInstance;
END

BEGIN(Client)
/*
@ Date: 2024-05-02, Writer: ������
@ Explain
-  ����ڰ� ���� �����ϴ� Ŭ�����̴�.
*/
class CSword  final : public UWeapon {
public:
	CSword(CSHPTRREF<UDevice> _spDevice, const _wstring& _wstrLayer, const CLONETYPE& _eCloneType);
	CSword(const CSword& _rhs);
	DESTRUCTOR(CSword)
public:
	// UPlayer��(��) ���� ��ӵ�
	CLONE_MACRO(CSword, "CSword::CloneToFailed")
	virtual void Free() override;
	virtual HRESULT NativeConstruct() override;
	virtual HRESULT NativeConstructClone(const VOIDDATAS& _Datas) override;
protected:
	virtual void TickActive(const _double& _dTimeDelta) override;
	virtual void LateTickActive(const _double& _dTimeDelta) override;
	virtual HRESULT RenderActive(CSHPTRREF<UCommand> _spCommand, CSHPTRREF<UTableDescriptor> _spTableDescriptor) override;
	virtual HRESULT RenderShadowActive(CSHPTRREF<UCommand> _spCommand, CSHPTRREF<UTableDescriptor> _spTableDescriptor) override;
	virtual void Collision(CSHPTRREF<UPawn> _pEnemy, const _double& _dTimeDelta) override;
private:
};

END

