#include "ToolDefines.h"
#include "TParticleView.h"
#include "UParticle.h"
#include "UParticleSystem.h"
#include "UAnimatedParticle.h"
#include "UGameInstance.h"
#include "UTexGroup.h"
#include "UTexture.h"
#include "UMethod.h"

TParticleView::TParticleView(CSHPTRREF<UDevice> _spDevice) :
	TImGuiView(_spDevice, "ParticleView"),
	m_stMainDesc{},
	m_stSingleParticleView{},
	m_stMultiParticleView{},
	m_stAnimParticleView{},
	m_isInitSetting{ false }
	, m_SingleParticle{nullptr}, m_SingleParticleParam{nullptr}, m_SingleParticleType{nullptr}
	, m_MultipleParticle{nullptr}, m_MultipleParticleParam{nullptr}, m_MultipleParticleType{nullptr}
	, m_AnimParticle{nullptr}, m_AnimParticleParam{nullptr}, m_AnimParticleType{nullptr}
	, m_iSingleParticleSize{ 0 },m_iMultipleParticleSize{0}, m_iCurActiveMultipleParticle{0}, m_iAnimParticleSize{0}
{
}

void TParticleView::Free()
{
}

HRESULT TParticleView::NativeConstruct()
{
	m_stMainDesc = MAINDESC(ImGuiWindowFlags_NoBackground, ImGuiDockNodeFlags_None,
		ImVec2{ (_float)WINDOW_WIDTH, 0.f }, ImVec2{ 500.f, (_float)WINDOW_HEIGHT });
	m_stSingleParticleView = DOCKDESC("Single Particle Viewer", ImGuiWindowFlags_NoFocusOnAppearing,
		ImGuiDockNodeFlags_CentralNode);
	m_stMultiParticleView = DOCKDESC("Multiple Particle Viewer", ImGuiWindowFlags_NoFocusOnAppearing,
		ImGuiDockNodeFlags_CentralNode);
	m_stAnimParticleView = DOCKDESC("Anim Particle Viewer", ImGuiWindowFlags_NoFocusOnAppearing,
		ImGuiDockNodeFlags_CentralNode);
	return S_OK;
}

//================================================================
//**** ��ƼŬ �������� �����ϴ� �����̳�(����) ũ�� �̸� �Ҵ� ****
//================================================================
void TParticleView::ResizeSingleParticleVector(_uint _resizeAmount)
{
	m_iSingleParticleSize = _resizeAmount;
	m_SingleParticle.resize(m_iSingleParticleSize);
	m_SingleParticleParam.resize(m_iSingleParticleSize);
	m_SingleParticleType.resize(m_iSingleParticleSize);
}


void TParticleView::ResizeMultipleParticleVector(_uint _resizeAmount)
{
	m_iMultipleParticleSize = _resizeAmount;
	m_MultipleParticle.resize(m_iMultipleParticleSize);
	m_MultipleParticleParam.resize(m_iMultipleParticleSize);
	m_MultipleParticleType.resize(m_iMultipleParticleSize);
}

void TParticleView::ResizeAnimParticleVector(_uint _resizeAmount)
{
	m_iAnimParticleSize = _resizeAmount;
	m_AnimParticle.resize(m_iAnimParticleSize);
	m_AnimParticleParam.resize(m_iAnimParticleSize);
	m_AnimParticleType.resize(m_iAnimParticleSize);
}

//================================================================
//**************** ��ƼŬ �������� ���ҽ� �Ҵ� ****************
//================================================================

void TParticleView::LoadSingleParticleResource()
{
	for (int i = 0; i < m_iSingleParticleSize; i++) {
		SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);
		{
			UParticle::PARTICLEDESC tDesc;
			tDesc.wstrParticleComputeShader = PROTO_RES_COMPUTEPARTICLE2DSHADER;
			tDesc.wstrParticleShader = PROTO_RES_PARTICLE2DSHADER;


			tDesc.ParticleParam.stGlobalParticleInfo.fAccTime = 0.f;
			//tDesc.ParticleParam.stGlobalParticleInfo.fDeltaTime = 2.f;
			tDesc.ParticleParam.stGlobalParticleInfo.fEndScaleParticle = 1.f;
			tDesc.ParticleParam.stGlobalParticleInfo.fStartScaleParticle = 10.f;
			tDesc.ParticleParam.stGlobalParticleInfo.fMaxLifeTime = 0.8f;
			tDesc.ParticleParam.stGlobalParticleInfo.fMinLifeTime = 0.3f;
			tDesc.ParticleParam.stGlobalParticleInfo.fMaxSpeed = 50.f;
			tDesc.ParticleParam.stGlobalParticleInfo.fMinSpeed = 100.f;
			tDesc.ParticleParam.stGlobalParticleInfo.iMaxCount = 1000;
			tDesc.ParticleParam.stGlobalParticleInfo.fParticleThickness = 25.f;
			tDesc.ParticleParam.stGlobalParticleInfo.fParticleDirection = _float3(0.5f, 0.5f, 0.5f);
			m_SingleParticle[i] = std::static_pointer_cast<UParticle>(spGameInstance->CloneActorAdd(PROTO_ACTOR_PARTICLE, { &tDesc }));
		}
		m_SingleParticleParam[i] = m_SingleParticle[i]->GetParticleSystem()->GetParticleParam();
		m_SingleParticleType[i] = m_SingleParticle[i]->GetParticleSystem()->GetParticleTypeParam();
		m_SingleParticleType[i]->fParticleType = PARTICLE_TYPE_AUTO;
		m_SingleParticleType[i]->fParticleLifeTimeType = PARTICLE_LIFETIME_TYPE_DEFAULT; 
		
	}
}

void TParticleView::LoadMultipleParticleResource()
{
	for (int i = 0; i < m_iMultipleParticleSize; i++) {
		SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);
		{
			UParticle::PARTICLEDESC tDesc;
			tDesc.wstrParticleComputeShader = PROTO_RES_COMPUTEPARTICLE2DSHADER;
			tDesc.wstrParticleShader = PROTO_RES_PARTICLE2DSHADER;
			

			tDesc.ParticleParam.stGlobalParticleInfo.fAccTime = 0.f;
			//tDesc.ParticleParam.stGlobalParticleInfo.fDeltaTime = 2.f;
			tDesc.ParticleParam.stGlobalParticleInfo.fEndScaleParticle = 1.f * (i + 1);
			tDesc.ParticleParam.stGlobalParticleInfo.fStartScaleParticle = 5.f * (i + 1);
			tDesc.ParticleParam.stGlobalParticleInfo.fMaxLifeTime = 0.8f * (i + 1);
			tDesc.ParticleParam.stGlobalParticleInfo.fMinLifeTime = 0.3f * (i + 1);
			tDesc.ParticleParam.stGlobalParticleInfo.fMaxSpeed = 10.f * (i + 1);
			tDesc.ParticleParam.stGlobalParticleInfo.fMinSpeed = 15.f * (i + 1);
			tDesc.ParticleParam.stGlobalParticleInfo.iMaxCount = 100 * (i + 1);
			tDesc.ParticleParam.stGlobalParticleInfo.fParticleThickness = 5.f * (i + 1);
			tDesc.ParticleParam.stGlobalParticleInfo.fParticleDirection = _float3(0.f, 0.f, 0.1);
			m_MultipleParticle[i] = std::static_pointer_cast<UParticle>(spGameInstance->CloneActorAdd(PROTO_ACTOR_PARTICLE, { &tDesc }));
		}
		m_MultipleParticleParam[i] = m_MultipleParticle[i]->GetParticleSystem()->GetParticleParam();
		m_MultipleParticleType[i] = m_MultipleParticle[i]->GetParticleSystem()->GetParticleTypeParam();
		m_MultipleParticleType[i]->fParticleType = PARTICLE_TYPE_AUTO;
		m_MultipleParticleType[i]->fParticleLifeTimeType = PARTICLE_LIFETIME_TYPE_DEFAULT;
		m_MultipleParticle[i]->SetTexture(i);
		//m_MultipleParticle[i]->SetActive(true);
	}

}

void TParticleView::LoadAnimParticleResource()
{
	for (int i = 0; i < m_iAnimParticleSize; i++) {
		SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);
		{
			UParticle::PARTICLEDESC tDesc;
			tDesc.wstrParticleComputeShader = PROTO_RES_COMPUTEPARTICLE2DSHADER;
			tDesc.wstrParticleShader = PROTO_RES_2DANIMATEPARTICLESHADER;
		

			tDesc.ParticleParam.stGlobalParticleInfo.fAccTime = 0.f;
			//tDesc.ParticleParam.stGlobalParticleInfo.fDeltaTime = 2.f;
			tDesc.ParticleParam.stGlobalParticleInfo.fEndScaleParticle = 10.f;
			tDesc.ParticleParam.stGlobalParticleInfo.fStartScaleParticle = 30.f;
			tDesc.ParticleParam.stGlobalParticleInfo.fMaxLifeTime = 5.f;
			tDesc.ParticleParam.stGlobalParticleInfo.fMinLifeTime = 1.f;
			tDesc.ParticleParam.stGlobalParticleInfo.fMaxSpeed = 50.f;
			tDesc.ParticleParam.stGlobalParticleInfo.fMinSpeed = 100.f;
			tDesc.ParticleParam.stGlobalParticleInfo.iMaxCount = 1000;
			tDesc.ParticleParam.stGlobalParticleInfo.fParticleThickness = 25.f;
			tDesc.ParticleParam.stGlobalParticleInfo.fParticleDirection = _float3(0.0f, 0.0f, 0.1f);

			UAnimatedParticle::ANIMPARTICLEDESC tAnimDesc;
			tAnimDesc.vTextureSize = _float2{8.f, 8.f };
			tAnimDesc.fNextAnimTime = 0.025f;
			m_AnimParticle[i] = std::static_pointer_cast<UAnimatedParticle>(spGameInstance->CloneActorAdd(PROTO_ACTOR_ANIMATEPARTICLE, { &tDesc,&tAnimDesc }));
		}
		m_AnimParticleParam[i] = m_AnimParticle[i]->GetParticleSystem()->GetParticleParam();
		m_AnimParticleType[i] = m_AnimParticle[i]->GetParticleSystem()->GetParticleTypeParam();
	

		m_AnimParticleType[i]->fParticleType = PARTICLE_TYPE_DEFAULT;		
		m_AnimParticleType[i]->fParticleLifeTimeType = PARTICLE_LIFETIME_TYPE_DEFAULT;		
		
	}

}

HRESULT TParticleView::LoadResource()
{
	ResizeSingleParticleVector(1);
	LoadSingleParticleResource();

	ResizeMultipleParticleVector(5);
	LoadMultipleParticleResource();

	ResizeAnimParticleVector(1);
	LoadAnimParticleResource();

	return S_OK;
}

//================================================================
//**************** ��ƼŬ �������� ���ҽ� ���� ****************
//================================================================

void TParticleView::ReleaseSingleParticleResource()
{
	for (auto& _particle : m_SingleParticle) {
		_particle.reset();
		GetGameInstance()->RemoveActor(_particle);
	}//���� ���ߵ� �� �ִ� �ڵ� RemoveActor�� �� ������ ���캸�ƾ���.
	
}

void TParticleView::ReleaseMultipleParticleResource()
{
	for (auto& _particle : m_MultipleParticle) {
		_particle.reset();
		GetGameInstance()->RemoveActor(_particle);
	}//���� ���ߵ� �� �ִ� �ڵ� RemoveActor�� �� ������ ���캸�ƾ���.
}

void TParticleView::ReleaseAnimParticleResource()
{
	for (auto& _particle : m_AnimParticle) {
		_particle.reset();
		GetGameInstance()->RemoveActor(_particle);
	}//���� ���ߵ� �� �ִ� �ڵ� RemoveActor�� �� ������ ���캸�ƾ���.
}


HRESULT TParticleView::ReleaseResource()
{
	ReleaseSingleParticleResource();
	ReleaseMultipleParticleResource();
	ReleaseAnimParticleResource();

	return S_OK;
}


//================================================================
//************ ��ƼŬ �������� Rendering �ϴ� �κ� ************
//================================================================

void TParticleView::TickActive(const _double& _dTimeDelta)
{
}

void TParticleView::LateTickActive(const _double& _dTimeDetla)
{
}

void TParticleView::RenderActive()
{
	ImGui::SetNextWindowPos(m_stMainDesc.vPos, ImGuiCond_Appearing);
	ImGui::SetNextWindowSize(m_stMainDesc.vSize, ImGuiCond_Appearing);
	// Name, OpenPointer, MainDesc 
	
	ImGui::Begin(GetName().c_str(), GetOpenPointer(), m_stMainDesc.imgWindowFlags);
	{
		m_stMainDesc.iDockSpaceID = ImGui::GetID(GetName().c_str());
		DockBuildInitSetting();
		ImGui::DockSpace(m_stMainDesc.iDockSpaceID, ImVec2{}, m_stMainDesc.imgDockNodeFlags);
		//������� �� �Է�
		SingleParticleView();
		MultipleParticleView();
		AnimParticleView();
		
		
		
		
		
	}
	ImGui::End();
}

void TParticleView::RenderMenu()
{
}

void TParticleView::DockBuildInitSetting()
{
	RETURN_CHECK(true == m_isInitSetting, ;);
	{
		ImGuiID dock_main_id = m_stMainDesc.iDockSpaceID;
		ImGui::DockBuilderRemoveNode(m_stMainDesc.iDockSpaceID);
		ImGui::DockBuilderAddNode(m_stMainDesc.iDockSpaceID);
		// Docking Build 
		m_stSingleParticleView.iDockSpaceID = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Up, 0.5f, NULL, &dock_main_id);
		m_stMultiParticleView.iDockSpaceID = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Up, 0.5f, NULL, &dock_main_id);
		m_stAnimParticleView.iDockSpaceID = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Down, 0.5f, NULL, &dock_main_id);
		// DockBuild
		ImGui::DockBuilderDockWindow(m_stSingleParticleView.strName.c_str(), m_stSingleParticleView.iDockSpaceID);
		ImGui::DockBuilderDockWindow(m_stMultiParticleView.strName.c_str(), m_stMultiParticleView.iDockSpaceID);
		ImGui::DockBuilderDockWindow(m_stAnimParticleView.strName.c_str(), m_stAnimParticleView.iDockSpaceID);
		ImGui::DockBuilderFinish(m_stMainDesc.iDockSpaceID);
	}
	m_isInitSetting = true;
}

//================================================================
//************ SINGLE PARTICLE TOOL VIEW SETTING ************
//================================================================

void TParticleView::SingleParticleView()
{
	
	
		ImGui::Begin(m_stSingleParticleView.strName.c_str(), GetOpenPointer(), m_stSingleParticleView.imgWindowFlags);
		{
			if (ImGui::CollapsingHeader("Single Particle Settings", ImGuiTreeNodeFlags_DefaultOpen)) {
			ImGui::Text("Single ParticleView");
			if (true == ImGui::Button("Show Single Particle"))
			{
				m_SingleParticle[0]->SetActive(true);
			}
			if (true == ImGui::Button("Stop Single Particle"))
			{
				m_SingleParticle[0]->SetActive(false);
			}

			SingleParticleCountSetting();
			SingleParticleTimeSetting();
			SingleParticleTexSetting();
			DefaultSingleParticleSetting();
			AutomaticSingleParticleSetting();

		

		}
	}
	
	ImGui::End();
}



void TParticleView::SingleParticleCountSetting()
{
	//��ƼŬ �� ����Ŭ ���� ����.
	if (ImGui::CollapsingHeader("Single Particle Count Setting", ImGuiTreeNodeFlags_DefaultOpen)) {
		_uint increment = 1;
		ImGui::InputScalar("Enter Create Amount\n Min:1  Max :1000", ImGuiDataType_U32, m_SingleParticle[0]->GetParticleSystem()->GetAddParticleAmount(), &increment, &increment);
		if (*m_SingleParticle[0]->GetParticleSystem()->GetAddParticleAmount() < 1) {
			*m_SingleParticle[0]->GetParticleSystem()->GetAddParticleAmount() = 1;
		}
		else if (*m_SingleParticle[0]->GetParticleSystem()->GetAddParticleAmount() > 1000) {
			*m_SingleParticle[0]->GetParticleSystem()->GetAddParticleAmount() = 1000;
		}
	}
}


void TParticleView::SingleParticleTimeSetting()
{
	//��ƼŬ�� ���� �ð� ���ϱ�
	if (ImGui::CollapsingHeader("Single Particle Create Time Interval", ImGuiTreeNodeFlags_DefaultOpen)) {

		ImGui::InputFloat("Enter Time Interval\n Min:0.f  Max :6.f", m_SingleParticle[0]->GetParticleSystem()->GetCreateInterval(), 0.1f, 1.0f, "%.2f", ImGuiInputTextFlags_CharsDecimal);
		if (*m_SingleParticle[0]->GetParticleSystem()->GetCreateInterval() <= 0) {
			*m_SingleParticle[0]->GetParticleSystem()->GetCreateInterval() = 0.1;
		}
		else if (*m_SingleParticle[0]->GetParticleSystem()->GetCreateInterval() > 6) {
			*m_SingleParticle[0]->GetParticleSystem()->GetCreateInterval() = 6;
		}
	}
}

void TParticleView::SingleParticleTexSetting()
{
	 

		//��ƼŬ�� �ؽ��� ����
		ImGui::Text("Single Particle Texture Select");
		if (ImGui::BeginListBox("Texture List", ImVec2(-FLT_MIN, 5 * ImGui::GetTextLineHeightWithSpacing())))
		{

			using TEXNAMES = UNORMAP<_wstring, _uint>;
			TEXNAMES m_TextureNames = m_SingleParticle[0]->GetTextureGroup()->GetTextureNames();
			for (auto& Texture : m_TextureNames)
			{
				if (ImGui::Selectable(UMethod::ConvertWToS(Texture.first)))
				{
					m_SingleParticle[0]->SetTexture(Texture.second);
				}
			}
			ImGui::EndListBox();
		}
	
}


void TParticleView::DefaultSingleParticleSetting()
{
	_bool Default = false;
	if (ImGui::Selectable("Set Single Particle Type : Default", Default)) {
		m_SingleParticleType[0]->fParticleType = PARTICLE_TYPE_DEFAULT;
		ImGui::OpenPopup("Default Particle");
	}

	if (ImGui::BeginPopup("Default Particle")) {
		ImGui::SeparatorText("Particle Settings");
		ImGui::SeparatorText("Current Particle Type : Default");

		ImGui::SliderFloat("EndScale", &m_SingleParticleParam[0]->stGlobalParticleInfo.fEndScaleParticle, 1.f, 40.f, "%.2f");
		ImGui::SliderFloat("StartScale", &m_SingleParticleParam[0]->stGlobalParticleInfo.fStartScaleParticle, 1.f, 20.f, "%.2f");
		ImGui::SliderFloat("MaxLifeTime", &m_SingleParticleParam[0]->stGlobalParticleInfo.fMaxLifeTime, 1.f, 10.f, "%.2f");
		ImGui::SliderFloat("MinLifeTime", &m_SingleParticleParam[0]->stGlobalParticleInfo.fMinLifeTime, 0.f, 5.f, "%.2f");
		ImGui::SliderFloat("MaxSpeed", &m_SingleParticleParam[0]->stGlobalParticleInfo.fMaxSpeed, 10.f, 100.f, "%.2f");
		ImGui::SliderFloat("MinSpeed", &m_SingleParticleParam[0]->stGlobalParticleInfo.fMinSpeed, 10.f, 100.f, "%.2f");
		ImGui::SliderFloat("DirectionX", &m_SingleParticleParam[0]->stGlobalParticleInfo.fParticleDirection.x, -1.f, 1.f, "%.2f");
		ImGui::SliderFloat("DirectionY", &m_SingleParticleParam[0]->stGlobalParticleInfo.fParticleDirection.y, -1.f, 1.f, "%.2f");
		ImGui::SliderFloat("DirectionZ", &m_SingleParticleParam[0]->stGlobalParticleInfo.fParticleDirection.z, -1.f, 1.f, "%.2f");
		ImGui::SliderFloat("Thickness", &m_SingleParticleParam[0]->stGlobalParticleInfo.fParticleThickness, 0.f, 50.f, "%.2f");
		ImGui::EndPopup();
	}
}

void TParticleView::AutomaticSingleParticleSetting()
{
	_bool Automatic = false;
	if (ImGui::Selectable("Set Single Particle Type : Automatic", Automatic)) {
		m_SingleParticleType[0]->fParticleType = PARTICLE_TYPE_AUTO;
		ImGui::OpenPopup("Automatic Particle");
	}



	if (ImGui::BeginPopup("Automatic Particle")) {
		ImGui::SeparatorText("Particle Settings");
		ImGui::SeparatorText("Current Particle Type : Automatic");

		ImGui::SliderFloat("EndScale", &m_SingleParticleParam[0]->stGlobalParticleInfo.fEndScaleParticle, 1.f, 40.f, "%.2f");
		ImGui::SliderFloat("StartScale", &m_SingleParticleParam[0]->stGlobalParticleInfo.fStartScaleParticle, 1.f, 20.f, "%.2f");
		ImGui::SliderFloat("MaxSpeed", &m_SingleParticleParam[0]->stGlobalParticleInfo.fMaxSpeed, 1.f, 100.f, "%.2f");
		ImGui::SliderFloat("MinSpeed", &m_SingleParticleParam[0]->stGlobalParticleInfo.fMinSpeed, 1.f, 20.f, "%.2f");
		ImGui::SliderFloat("MaxLifeTime", &m_SingleParticleParam[0]->stGlobalParticleInfo.fMaxLifeTime, 1.f, 10.f, "%.2f");
		ImGui::SliderFloat("MinLifeTime", &m_SingleParticleParam[0]->stGlobalParticleInfo.fMinLifeTime, 0.f, 5.f, "%.2f");

		ImGui::EndPopup();
	}
}

//================================================================
//************ MULTIPLE PARTICLE TOOL VIEW SETTING ************
//================================================================

void TParticleView::MultipleParticleView()
{
	ImGui::Begin(m_stMultiParticleView.strName.c_str(), GetOpenPointer(), m_stMultiParticleView.imgWindowFlags);
	{
		if (ImGui::CollapsingHeader("Multiple Particle Settings", ImGuiTreeNodeFlags_DefaultOpen)) {
			ImGui::Text("Multiple ParticleView");
			_bool Show = false;
			if (ImGui::Selectable("Show Multiple Particle", Show))
			{
				ImGui::OpenPopup("Show Multi Particle");
				
			}
			if (ImGui::BeginPopup("Show Multi Particle")) {
				_uint increment = 1;
				ImGui::InputScalar("Enter Create Amount\n Min:0  Max :5", ImGuiDataType_U32, &m_iCurActiveMultipleParticle, &increment, &increment);
				if (m_iCurActiveMultipleParticle >= m_iMultipleParticleSize) m_iCurActiveMultipleParticle = m_iMultipleParticleSize;
				for (int i = 0; i < m_iCurActiveMultipleParticle; i++) m_MultipleParticle[i]->SetActive(true);
				ImGui::EndPopup();
			}

			if (true == ImGui::Button("Stop Multiple Particle"))
			{
				for (int i = 0; i < m_iMultipleParticleSize; i++)
				m_MultipleParticle[i]->SetActive(false);
			}

			MultipleParticleCountSetting();
			/*MultipleParticleTimeSetting();
			MultipleParticleTexSetting();
			DefaultMultipleParticleSetting();
			AutomaticMultipleParticleSetting();*/
		}
	}
	ImGui::End();
}

void TParticleView::MultipleParticleCountSetting()
{
	//��ƼŬ �� ����Ŭ ���� ����.
	if (ImGui::CollapsingHeader("Multiple Particle Count Setting", ImGuiTreeNodeFlags_DefaultOpen)) {
		
			_uint increment = 1;
			ImGui::InputScalar("Enter Create Amount\n Min:1  Max :1000", ImGuiDataType_U32, m_MultipleParticle[0]->GetParticleSystem()->GetAddParticleAmount(), &increment, &increment);
			if (*m_MultipleParticle[0]->GetParticleSystem()->GetAddParticleAmount() < 1) {
				*m_MultipleParticle[0]->GetParticleSystem()->GetAddParticleAmount() = 1;
			}
			else if (*m_MultipleParticle[0]->GetParticleSystem()->GetAddParticleAmount() > 1000) {
				*m_MultipleParticle[0]->GetParticleSystem()->GetAddParticleAmount() = 1000;
			}
		}
	
}

void TParticleView::MultipleParticleTimeSetting()
{
	//��ƼŬ�� ���� �ð� ���ϱ�
	if (ImGui::CollapsingHeader("Multiple Particle Create Time Interval", ImGuiTreeNodeFlags_DefaultOpen)) {

		ImGui::InputFloat("Enter Time Interval\n Min:0.f  Max :6.f", m_AnimParticle[0]->GetParticleSystem()->GetCreateInterval(), 0.1f, 1.0f, "%.2f", ImGuiInputTextFlags_CharsDecimal);
		if (*m_AnimParticle[0]->GetParticleSystem()->GetCreateInterval() <= 0) {
			*m_AnimParticle[0]->GetParticleSystem()->GetCreateInterval() = 0.1;
		}
		else if (*m_AnimParticle[0]->GetParticleSystem()->GetCreateInterval() > 6) {
			*m_AnimParticle[0]->GetParticleSystem()->GetCreateInterval() = 6;
		}
	}
}

void TParticleView::MultipleParticleTexSetting()
{

	//��ƼŬ�� �ؽ��� ����
	ImGui::Text("Multiple Particle Texture Select");
	if (ImGui::BeginListBox("Texture List", ImVec2(-FLT_MIN, 5 * ImGui::GetTextLineHeightWithSpacing())))
	{

		using TEXNAMES = UNORMAP<_wstring, _uint>;
		TEXNAMES m_TextureNames = m_AnimParticle[0]->GetTextureGroup()->GetTextureNames();
		for (auto& Texture : m_TextureNames)
		{
			if (ImGui::Selectable(UMethod::ConvertWToS(Texture.first)))
			{
				m_AnimParticle[0]->SetTexture(Texture.second);
				if (Texture.second == 0 || Texture.second == 1 || Texture.second == 3 || Texture.second == 4) {
					m_AnimParticle[0]->SetNextAnimTimer(0.025f);
					m_AnimParticle[0]->SetTextureRowsAndCols(8.f, 8.f);
				}
				else if (Texture.second == 5) {
					m_AnimParticle[0]->SetNextAnimTimer(0.025f);
					m_AnimParticle[0]->SetTextureRowsAndCols(6.f, 6.f);
				}
				else if (Texture.second == 2) {
					m_AnimParticle[0]->SetNextAnimTimer(0.025f);
					m_AnimParticle[0]->SetTextureRowsAndCols(8.f, 4.f);
				}

			}
		}
		ImGui::EndListBox();
	}

}

void TParticleView::DefaultMultipleParticleSetting()
{
	_bool Default = false;
	if (ImGui::Selectable("Set Multiple Particle Type : Default", Default)) {
		m_AnimParticleType[0]->fParticleType = PARTICLE_TYPE_DEFAULT;
		ImGui::OpenPopup("Default Particle");
	}

	if (ImGui::BeginPopup("Default Particle")) {
		ImGui::SeparatorText("Particle Settings");
		ImGui::SeparatorText("Current Particle Type : Default");

		ImGui::SliderFloat("EndScale", &m_AnimParticleParam[0]->stGlobalParticleInfo.fEndScaleParticle, 1.f, 40.f, "%.2f");
		ImGui::SliderFloat("StartScale", &m_AnimParticleParam[0]->stGlobalParticleInfo.fStartScaleParticle, 1.f, 20.f, "%.2f");
		ImGui::SliderFloat("MaxLifeTime", &m_AnimParticleParam[0]->stGlobalParticleInfo.fMaxLifeTime, 3.f, 10.f, "%.2f");
		ImGui::SliderFloat("MinLifeTime", &m_AnimParticleParam[0]->stGlobalParticleInfo.fMinLifeTime, 0.f, 5.f, "%.2f");
		ImGui::SliderFloat("MaxSpeed", &m_AnimParticleParam[0]->stGlobalParticleInfo.fMaxSpeed, 10.f, 100.f, "%.2f");
		ImGui::SliderFloat("MinSpeed", &m_AnimParticleParam[0]->stGlobalParticleInfo.fMinSpeed, 10.f, 100.f, "%.2f");
		ImGui::SliderFloat("DirectionX", &m_AnimParticleParam[0]->stGlobalParticleInfo.fParticleDirection.x, -1.f, 1.f, "%.2f");
		ImGui::SliderFloat("DirectionY", &m_AnimParticleParam[0]->stGlobalParticleInfo.fParticleDirection.y, -1.f, 1.f, "%.2f");
		ImGui::SliderFloat("DirectionZ", &m_AnimParticleParam[0]->stGlobalParticleInfo.fParticleDirection.z, -1.f, 1.f, "%.2f");
		ImGui::SliderFloat("Thickness", &m_AnimParticleParam[0]->stGlobalParticleInfo.fParticleThickness, 0.f, 50.f, "%.2f");
		ImGui::EndPopup();
	}
}

void TParticleView::AutomaticMultipleParticleSetting()
{
	_bool Automatic = false;
	if (ImGui::Selectable("Set Multiple Particle Type : Automatic", Automatic)) {
		m_AnimParticleType[0]->fParticleType = PARTICLE_TYPE_AUTO;
		ImGui::OpenPopup("Automatic Particle");
	}



	if (ImGui::BeginPopup("Automatic Particle")) {
		ImGui::SeparatorText("Particle Settings");
		ImGui::SeparatorText("Current Particle Type : Automatic");

		ImGui::SliderFloat("EndScale", &m_AnimParticleParam[0]->stGlobalParticleInfo.fEndScaleParticle, 1.f, 40.f, "%.2f");
		ImGui::SliderFloat("StartScale", &m_AnimParticleParam[0]->stGlobalParticleInfo.fStartScaleParticle, 1.f, 20.f, "%.2f");
		ImGui::SliderFloat("MaxSpeed", &m_AnimParticleParam[0]->stGlobalParticleInfo.fMaxSpeed, 10.f, 100.f, "%.2f");
		ImGui::SliderFloat("MinSpeed", &m_AnimParticleParam[0]->stGlobalParticleInfo.fMinSpeed, 10.f, 100.f, "%.2f");


		ImGui::EndPopup();
	}
}



//================================================================
//************ ANIM PARTICLE TOOL VIEW SETTING ************
//================================================================

void TParticleView::AnimParticleView()
{
	
		ImGui::Begin(m_stAnimParticleView.strName.c_str(), GetOpenPointer(), m_stAnimParticleView.imgWindowFlags);
		{
			if (ImGui::CollapsingHeader("Anim Particle Settings", ImGuiTreeNodeFlags_DefaultOpen)) {
			ImGui::Text("Animation ParticleView");
			if (true == ImGui::Button("Show Anim Particle"))
			{
				m_AnimParticle[0]->SetActive(true);
			}
			if (true == ImGui::Button("Stop Anim Particle"))
			{
				m_AnimParticle[0]->SetActive(false);
			}

			AnimParticleCountSetting();
			AnimParticleTimeSetting();
			AnimParticleTexSetting();
			DefaultAnimParticleSetting();
			AutomaticAnimParticleSetting();
			}
		}
		ImGui::End();
	
	

}


void TParticleView::AnimParticleCountSetting() 
{
	//��ƼŬ �� ����Ŭ ���� ����.
	if (ImGui::CollapsingHeader("Anim Particle Count Setting", ImGuiTreeNodeFlags_DefaultOpen)) {
		_uint increment = 1;
		ImGui::InputScalar("Enter Create Amount\n Min:1  Max :1000", ImGuiDataType_U32, m_AnimParticle[0]->GetParticleSystem()->GetAddParticleAmount(), &increment, &increment);
		if (*m_AnimParticle[0]->GetParticleSystem()->GetAddParticleAmount() < 1) {
			*m_AnimParticle[0]->GetParticleSystem()->GetAddParticleAmount() = 1;
		}
		else if (*m_AnimParticle[0]->GetParticleSystem()->GetAddParticleAmount() > 1000) {
			*m_AnimParticle[0]->GetParticleSystem()->GetAddParticleAmount() = 1000;
		}
	}
}

void TParticleView::AnimParticleTimeSetting() 
{
	//��ƼŬ�� ���� �ð� ���ϱ�
	if (ImGui::CollapsingHeader("Anim Particle Create Time Interval", ImGuiTreeNodeFlags_DefaultOpen)) {

		ImGui::InputFloat("Enter Time Interval\n Min:0.f  Max :6.f", m_AnimParticle[0]->GetParticleSystem()->GetCreateInterval(), 0.1f, 1.0f, "%.2f", ImGuiInputTextFlags_CharsDecimal);
		if (*m_AnimParticle[0]->GetParticleSystem()->GetCreateInterval() <= 0) {
			*m_AnimParticle[0]->GetParticleSystem()->GetCreateInterval() = 0.1;
		}
		else if (*m_AnimParticle[0]->GetParticleSystem()->GetCreateInterval() > 6) {
			*m_AnimParticle[0]->GetParticleSystem()->GetCreateInterval() = 6;
		}
	}
}

void TParticleView::AnimParticleTexSetting() 
{
	
		//��ƼŬ�� �ؽ��� ����
		ImGui::Text("Anim Particle Texture Select");
		if (ImGui::BeginListBox("Texture List", ImVec2(-FLT_MIN, 5 * ImGui::GetTextLineHeightWithSpacing())))
		{

			using TEXNAMES = UNORMAP<_wstring, _uint>;
			TEXNAMES m_TextureNames = m_AnimParticle[0]->GetTextureGroup()->GetTextureNames();
			for (auto& Texture : m_TextureNames)
			{
				if (ImGui::Selectable(UMethod::ConvertWToS(Texture.first)))
				{
					if (Texture.second == 1 || Texture.second == 2 || Texture.second == 5 || Texture.second == 6) {
						m_AnimParticle[0]->SetNextAnimTimer(0.025f);
						m_AnimParticle[0]->SetTextureRowsAndCols(8.f, 8.f);
					}
					else if (Texture.second == 3) {
						m_AnimParticle[0]->SetNextAnimTimer(0.025f);
						m_AnimParticle[0]->SetTextureRowsAndCols(6.f, 6.f);
					}
					else if (Texture.second == 4) {
						m_AnimParticle[0]->SetNextAnimTimer(0.025f);
						m_AnimParticle[0]->SetTextureRowsAndCols(8.f, 4.f);
					}
					else if (Texture.second == 0) {
						m_AnimParticle[0]->SetNextAnimTimer(0.025f);
						m_AnimParticle[0]->SetTextureRowsAndCols(3.f, 3.f);
					}

					m_AnimParticle[0]->SetTexture(Texture.second);
					
					
				}
			}
			ImGui::EndListBox();
		}
	
}

void TParticleView::DefaultAnimParticleSetting() 
{
	_bool Default = false;
	if (ImGui::Selectable("Set Anim Particle Type : Default", Default)) {
		m_AnimParticleType[0]->fParticleType = PARTICLE_TYPE_DEFAULT;
		ImGui::OpenPopup("Default Particle");
	}

	if (ImGui::BeginPopup("Default Particle")) {
		ImGui::SeparatorText("Particle Settings");
		ImGui::SeparatorText("Current Particle Type : Default");

		ImGui::SliderFloat("EndScale", &m_AnimParticleParam[0]->stGlobalParticleInfo.fEndScaleParticle, 1.f, 40.f, "%.2f");
		ImGui::SliderFloat("StartScale", &m_AnimParticleParam[0]->stGlobalParticleInfo.fStartScaleParticle, 1.f, 20.f, "%.2f");
		ImGui::SliderFloat("MaxLifeTime", &m_AnimParticleParam[0]->stGlobalParticleInfo.fMaxLifeTime, 3.f, 10.f, "%.2f");
		ImGui::SliderFloat("MinLifeTime", &m_AnimParticleParam[0]->stGlobalParticleInfo.fMinLifeTime, 0.f, 5.f, "%.2f");
		ImGui::SliderFloat("MaxSpeed", &m_AnimParticleParam[0]->stGlobalParticleInfo.fMaxSpeed, 10.f, 100.f, "%.2f");
		ImGui::SliderFloat("MinSpeed", &m_AnimParticleParam[0]->stGlobalParticleInfo.fMinSpeed, 10.f, 100.f, "%.2f");
		ImGui::SliderFloat("DirectionX", &m_AnimParticleParam[0]->stGlobalParticleInfo.fParticleDirection.x, -1.f, 1.f, "%.2f");
		ImGui::SliderFloat("DirectionY", &m_AnimParticleParam[0]->stGlobalParticleInfo.fParticleDirection.y, -1.f, 1.f, "%.2f");
		ImGui::SliderFloat("DirectionZ", &m_AnimParticleParam[0]->stGlobalParticleInfo.fParticleDirection.z, -1.f, 1.f, "%.2f");
		ImGui::SliderFloat("Thickness", &m_AnimParticleParam[0]->stGlobalParticleInfo.fParticleThickness, 0.f, 50.f, "%.2f");
		ImGui::EndPopup();
	}
}

void TParticleView::AutomaticAnimParticleSetting() 
{
	_bool Automatic = false;
	if (ImGui::Selectable("Set Anim Particle Type : Automatic", Automatic)) {
		m_AnimParticleType[0]->fParticleType = PARTICLE_TYPE_AUTO;
		ImGui::OpenPopup("Automatic Particle");
	}



	if (ImGui::BeginPopup("Automatic Particle")) {
		ImGui::SeparatorText("Particle Settings");
		ImGui::SeparatorText("Current Particle Type : Automatic");

		ImGui::SliderFloat("EndScale", &m_AnimParticleParam[0]->stGlobalParticleInfo.fEndScaleParticle, 1.f, 40.f, "%.2f");
		ImGui::SliderFloat("StartScale", &m_AnimParticleParam[0]->stGlobalParticleInfo.fStartScaleParticle, 1.f, 20.f, "%.2f");	
		ImGui::SliderFloat("MaxSpeed", &m_AnimParticleParam[0]->stGlobalParticleInfo.fMaxSpeed, 10.f, 100.f, "%.2f");
		ImGui::SliderFloat("MinSpeed", &m_AnimParticleParam[0]->stGlobalParticleInfo.fMinSpeed, 10.f, 100.f, "%.2f");


		ImGui::EndPopup();
	}
}

