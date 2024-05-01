#pragma once 
#include "TImGuiView.h"

BEGIN(Engine)
class UModel;
class UAnimModel;
class UPawn;
class UMapLayout;
END

BEGIN(Tool)
class TShowModelObject;
class TShowAnimModelObject;
class TGuizmoManager;
/*
@ Date: 2024-02-04, Writer: ������
@ Explain
- Model���� �ε��ؼ� �����ֱ� ���� Ŭ����
*/
class TModelView final : public TImGuiView {
public:
	TModelView(CSHPTRREF<UDevice> _spDevice);
	NO_COPY(TModelView)
	DESTRUCTOR(TModelView)
public:
	virtual void Free() override;

	virtual HRESULT NativeConstruct() override;
protected:
	virtual HRESULT LoadResource() override;
	virtual HRESULT ReleaseResource() override;
	virtual void TickActive(const _double& _dTimeDelta) override;
	virtual void LateTickActive(const _double& _dTimeDetla) override;
	virtual void RenderActive() override;

private:
	void DockBuildInitSetting();

	void ShowModels();
	void ShowAnimMoldels();
	// Show Model List
	void ShowModelList();
	void ShowAnimModelList();

	void ConvertModels();
	void ConvertAnimModels();

	void LoadAssimpModelDatas(CSHPTRREF<FILEGROUP> _spFolder);
	void LoadAnimModelData(CSHPTRREF<FILEGROUP> _spFolder);
	void ResetModels();
	void ResetAnimModels();

	void EditModel();
	void ClearCurrentModel();
	void ClearCurrentAnimModel();

	void MouseInput();

	HRESULT CopyCurrentModel();
	HRESULT PasteCopiedModel();
private:
	using MODELS = UNORMAP<_string, SHPTR<UModel>>;
	using ANIMMODEL = UNORMAP<_string, SHPTR<UAnimModel>>;
	using SHOWMODELS = UNORMAP<_string, SHPTR<TShowModelObject>>;
	using SHOWANIMEMODELS = UNORMAP<_string, SHPTR<TShowAnimModelObject>>;

	MAINDESC											m_stMainDesc;
	DOCKDESC											m_stModelDockDesc;
	DOCKDESC											m_stAnimModelDockDesc;
	DOCKDESC											m_stTransformEditorDesc;

	MODELS												m_ModelsContainer;
	ANIMMODEL											m_AnimModelContainer;
	SHOWMODELS											m_ShowModelsContainer;
	SHOWANIMEMODELS										m_ShowAnimModelsContainer;

	SHPTR<FILEGROUP>									m_spModelFileFolder;
	SHPTR<FILEGROUP>									m_spAnimModelFileFolder;

	SHPTR<UPawn>										m_spSelectedModel;
	_string												m_SelectedModelName;
	_string												m_CopiedModelName;

	SHPTR<UPawn>										m_spCopiedModel;

	SHPTR<TShowAnimModelObject>							m_spShowAnimModelObject;
	SHPTR<TShowModelObject>								m_spShowModelObject;

	_uint												m_iFileIndex;

	_bool												m_isAllConverter;
	_float3												m_vModelScale;
	_float3												m_vAnimModelScale;

	_bool												m_isInitSetting;
	_bool												m_isResetModel;
	_bool												m_isResetAnimModel;

	SHPTR<TGuizmoManager>								m_spGuizmoManager;

	_bool												m_bSelectedhasAnim;
	_bool												m_bColliderActive;

	_uint												m_iModelSuffix;
	_uint												m_iAnimModelSuffix;
	_uint												m_iCopiedModelSuffix;

	SHPTR<UMapLayout>									m_spMapLayout;
};

END
