#pragma once
#include "UBase.h"

BEGIN(Engine)
class UResource;
class UShader;
class UCommand;
class UModelMaterial;
class URenderer;
/*
@ Date: 2024-04-13, Writer: ������
@ Explain
-  ���͸����� �����ϴ� �Ŵ���
*/
class UMaterialManager final : public UBase {
public:
	UMaterialManager();
	NO_COPY(UMaterialManager)
	DESTRUCTOR(UMaterialManager)
public:
	void AddModelMaterial(const _uint _MaterialIndex, CSHPTRREF<UModelMaterial> _spModelMaterial);
	void  CopyToMaterialShaderParam(REF_IN GLOBALPARAM& _GrobalParam);
	void RemoveModelMaterial(const _uint _MaterialIndex);
public:
	const _bool IsMaterialContainerInfoChange() const { return m_isMaterialContainerInfoChange; }
private:
	virtual void Free() override;
private:
	using MATERIALCONTAINER = UNORMAP<_uint, WKPTR<UModelMaterial>>;

	MATERIALCONTAINER		m_MaterialContainer;

	_bool									m_isMaterialContainerInfoChange;
};

END