#pragma once
#include "UResource.h"

BEGIN(Engine)
class UBoneNode;
class UMeshContainer;
class UShader;
class UCommand;
class URootBoneNode;
class UModelMaterial;
class UShaderConstantBuffer;
/*
@ Date: 2024-02-04, Writer: ������
@ Explain
- ���� �޽�, �� ����, ���� �ؽ��� �������� VECTOR �����̳ʵ�� ��� �ִ� Ŭ�����̴�. 
*/
class  UModel : public UResource{
public:
	enum class TYPE
	{
		NONANIM, ANIM, EMPTY
	};

	using BONENODES = VECTOR<SHPTR<UBoneNode>>;
	using MESHCONTAINERS = VECTOR<SHPTR<UMeshContainer>>;
	using MATERIERS = VECTOR<SHPTR<UModelMaterial>>;
	using MATERIALINFOS = VECTOR<MODELMATERIALINFO>;
public:
	UModel(CSHPTRREF<UDevice> _spDevice, const TYPE& _eType = TYPE::NONANIM);
	UModel(const UModel& _rhs);
	DESTRUCTOR(UModel)
public:
	MESHCONTAINERS& GetMeshContainers() { return m_MeshContainer; }
	BONENODES& GetBoneNodes() { return m_BoneNodeContainer; }
	MATERIERS& GetMaterials() { return m_MaterialContainer; }
	// Get BoneNode
	SHPTR<UBoneNode> FindBoneNode(const _wstring& _strBoneNode) const;
	const _uint& GetMeshContainerCnt() const { return m_iMeshContainerCnt; }
	CSHPTRREF<URootBoneNode> GetRootBoneNode() const { return m_spRootBoneNode; }
	const _uint& GetBoneNodesCnt() const { return m_iBoneNodeCnt; }
	const _uint& GetMaterialsCnt() const { return m_iMaterialCnt; }

	CSHPTRREF<FILEGROUP> GetFileGroup() const { return m_spFileGroup; }
	CSHPTRREF<FILEDATA> GetFileData() const { return m_spFileData; }

	const TYPE& GetType() const { return m_eType; }
	const _wstring& GetModelName() const { return m_wstrModelName; }

	_float3 GetMaxVertexPos();
	_float3 GetMinVertexPos();
	_float3 GetCenterPos();

	void SetModelName(const _wstring& _newName) { m_wstrModelName = _newName; }
public:
	CLONE_MACRO(UModel, "UModel::Clone To Failed")
	virtual void Free() override;
	virtual HRESULT NativeConstruct() override;
	// �ش� PATH�� �� �ε�
	 HRESULT NativeConstruct(const _wstring& _wstrPath);
	 // �ش� ������ ���� �����ͷ� �ε�
	 HRESULT NativeConstruct(CSHPTRREF<FILEGROUP> _spFileGroup, CSHPTRREF<FILEDATA> _spFileData);
	 // PATH�� �Է��ϰ� ���� �̸��� �Է��ϸ� ã�Ƽ� �´�. 
	 HRESULT NativeConstruct(const PATHS& _vecPaths, const _wstring& _wstrFileName);
	 // ���� ���� �̸�, ���� �̸��� �Է��ϸ� ã�Ƽ� �� �ε�
	 HRESULT NativeConstruct(const _wstring& _wstrModelFolder, const _wstring& _wstrFileName);
	virtual HRESULT NativeConstructClone(const VOIDDATAS& _vecDatas) override;
	// �𵨿� �ؽ�ó�� ���ε� �ϴ� �Լ�
	 HRESULT BindTexture(const _uint _iMeshIndex,  const SRV_REGISTER _eRegister,
		const TEXTYPE& _eTextureType,CSHPTRREF<UShader> _spShader);
	 // ���� �׸��� �Լ�
	virtual HRESULT Render(const _uint _iMeshIndex, CSHPTRREF<UShader> _spShader, CSHPTRREF<UCommand> _spCommand);
protected:
	HRESULT CreateBoneNode(void* _pData, const _wstring& _wstrBoneNodeName = L"");
	HRESULT CreateMeshContainers(void* _pData);
	HRESULT CreateMaterial(void* _pData, const MATERIALINFOS& _vecMaterialInfo);
	// Load To Data
	virtual void LoadToData(const _wstring& _wstrPath);

	void LoadMeshData(REF_IN std::ifstream& _ifRead, REF_IN VECTOR<MESHDESC>& _vecMeshes);
	void LoadBoneData(REF_IN std::ifstream& _ifRead, REF_IN VECTOR<BONENODEDESC>& _vecBones);
	void LoadMaterial(REF_IN std::ifstream& _ifRead, REF_IN UNORMAP<_uint, VECTOR<_wstring>>& _uomapMaterials, REF_IN MATERIALINFOS& _vecMaterialInfos);
	void BringModelName(const _wstring& _wstrPath);
protected: /* get set*/
	void SetMeshContainers(const _uint _iMeshContainers) { this->m_iMeshContainerCnt = _iMeshContainers; }
	void SetBoneNodesCnt(const _uint _iBoneNodesCnt) { this->m_iBoneNodeCnt = _iBoneNodesCnt; }
	void SetMaterialsCnt(const _uint _iMaterialCnt) { this->m_iMaterialCnt = _iMaterialCnt; }
private:
	void CreateRootBoneToOrder(MODELDESC* _pDesc, _int _BoneOrder = 0);
private:
	// MeshContainer
	MESHCONTAINERS									m_MeshContainer;
	_uint															m_iMeshContainerCnt;
	// BoneNodes
	BONENODES												m_BoneNodeContainer;
	_uint															m_iBoneNodeCnt;
	SHPTR<URootBoneNode>						m_spRootBoneNode;
	// Material
	MATERIERS												m_MaterialContainer;
	_uint															m_iMaterialCnt;
	// Paths 
	SHPTR<FILEGROUP>								m_spFileGroup;
	SHPTR<FILEDATA>									m_spFileData;

	TYPE															m_eType;
	_wstring														m_wstrModelName;


};

END

