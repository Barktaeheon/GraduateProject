#include "ToolDefines.h"
#include "TAnimControlView.h"
#include "TAnimControlModel.h"
#include "UGameInstance.h"
#include "UMethod.h"
#include "TAssimpModel.h"
#include "UAnimModel.h"
#include "UAnimation.h"
#include "AnimOccursEvents.h"
#include "AnimSectionEvents.h"
#include "UBoneNode.h"
#include "UCollider.h"
#include "UAudioSystem.h"
#include "USound.h"


const _char* TAnimControlView::s_AnimTags[1000]{};

TAnimControlView::TAnimControlView(CSHPTRREF<UDevice> _spDevice) :
    TImGuiView(_spDevice, "AnimControlView"), 
	m_stMainDesc{},
	m_isInitSetting{false}, 
	m_spSelectAnimFileData{nullptr},
	m_iSelectAnimEvent{1},
	m_spSelectAnim{nullptr},
	m_AnimMaxTagCount{0},
	m_strSelectAnimationName{"NO_SELECTED"}
{
	
}

void TAnimControlView::Free()
{
}

HRESULT TAnimControlView::NativeConstruct()
{
    m_stMainDesc = MAINDESC(ImGuiWindowFlags_NoBackground , ImGuiDockNodeFlags_None,
        ImVec2{ (_float)WINDOW_WIDTH, 0.f }, ImVec2{ 500.f, (_float)WINDOW_HEIGHT });

	m_stAnimModelSelectDesc = DOCKDESC("AnimModelSelect", ImGuiWindowFlags_NoFocusOnAppearing ,
		ImGuiDockNodeFlags_CentralNode);
	m_stAnimModifyDesc = DOCKDESC("AnimModifyViewer", ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_HorizontalScrollbar | 
		ImGuiWindowFlags_AlwaysVerticalScrollbar,
		ImGuiDockNodeFlags_CentralNode);

	m_spAnimControlModel = std::static_pointer_cast<TAnimControlModel>(GetGameInstance()->CloneActorAdd(PROTO_ACTOR_ANIMCONTROLMODELOBJECT));


    return S_OK;
}

HRESULT TAnimControlView::LoadResource()
{
	SHPTR<FILEGROUP> AnimModels  = GetGameInstance()->FindFolder(L"AnimModel");

	for (auto& iter : AnimModels->UnderFileGroupList)
	{
		// FBX���� Convert ��� ���� ������ ã�´�. 
		SHPTR<FILEGROUP> spConvert = iter.second->FindGroup(L"Convert");
		if (nullptr == spConvert)
			continue;
		m_spSelectAnimFileFolder = spConvert;
		for (auto& FileData : spConvert->FileDataList)
		{
			m_AnimFileContainer.insert(MakePair(UMethod::ConvertWToS(FileData.first), FileData.second));
		}
	}

	SHPTR<UGameInstance> spGameInstance = GetGameInstance();
	SHPTR<UAudioSystem> spAudioSystem = spGameInstance->GetAudioSystem(SOUND_GAME);
	m_FindSoundNames.clear();
	// �̸� �־�α� 
	for (auto& Sound : spAudioSystem->GetSoundOrders())
	{
		m_FindSoundNames.push_back(UMethod::ConvertWToS(Sound.first));
	}
    return S_OK;
}

HRESULT TAnimControlView::ReleaseResource()
{
	m_spSelectAnim = nullptr;
	m_AnimFileContainer.clear();
	m_spAnimControlModel->SetActive(false);
	m_FindSoundNames.clear();
    return S_OK;
}

void TAnimControlView::TickActive(const _double& _dTimeDelta)
{
}

void TAnimControlView::LateTickActive(const _double& _dTimeDetla)
{
}

void TAnimControlView::RenderActive()
{
	ImGui::SetNextWindowPos(m_stMainDesc.vPos, ImGuiCond_Appearing);
	ImGui::SetNextWindowSize(m_stMainDesc.vSize, ImGuiCond_Appearing);
	// Name, OpenPointer, MainDesc 
	ImGui::Begin(GetName().c_str(), GetOpenPointer(), m_stMainDesc.imgWindowFlags);
	{
		m_stMainDesc.iDockSpaceID = ImGui::GetID(GetName().c_str());
		DockBuildInitSetting();
		ImGui::DockSpace(m_stMainDesc.iDockSpaceID, ImVec2{}, m_stMainDesc.imgDockNodeFlags);
		AnimModelSelectView();
		AnimModifyView();
	}
	ImGui::End();
}

void TAnimControlView::DockBuildInitSetting()
{
	RETURN_CHECK(true == m_isInitSetting, ;);
	{
		ImGuiID dock_main_id = m_stMainDesc.iDockSpaceID;
		ImGui::DockBuilderRemoveNode(m_stMainDesc.iDockSpaceID);
		ImGui::DockBuilderAddNode(m_stMainDesc.iDockSpaceID);
		// Docking Build 
	    m_stAnimModelSelectDesc.iDockSpaceID = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Up, 0.2f, NULL, &dock_main_id);
		m_stAnimModifyDesc.iDockSpaceID = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Down, 0.8f, NULL, &dock_main_id);
		// DockBuild
		ImGui::DockBuilderDockWindow(m_stAnimModelSelectDesc.strName.c_str(), m_stAnimModelSelectDesc.iDockSpaceID);
		ImGui::DockBuilderDockWindow(m_stAnimModifyDesc.strName.c_str(), m_stAnimModifyDesc.iDockSpaceID);
		ImGui::DockBuilderFinish(m_stMainDesc.iDockSpaceID);
	}
	m_isInitSetting = true;
}

void TAnimControlView::AnimModelSelectView()
{
	ImGui::Begin(m_stAnimModelSelectDesc.strName.c_str(), GetOpenPointer(), m_stAnimModelSelectDesc.imgWindowFlags);
	{
		ImGui::Text("AnimModelSelect");
		static _wstring wstrSelectFolderName{ L"" };
		if (ImGui::BeginListBox("AnimModel List", ImVec2(-FLT_MIN, 5 * ImGui::GetTextLineHeightWithSpacing())))
		{
			using ANIMFILEDATA = std::pair < _string, SHPTR<FILEDATA>>;
			for (const ANIMFILEDATA& AnimModel : m_AnimFileContainer)
			{
				if (ImGui::Selectable(AnimModel.first.c_str()))
				{
					m_spSelectAnimFileData = AnimModel.second;
					wstrSelectFolderName = UMethod::ConvertSToW(AnimModel.first);
				 }
			}
			ImGui::EndListBox();
		}
		if (true == ImGui::Button("Load AnimationModel"))
		{
			// Select Folder 
			if (nullptr != m_spSelectAnimFileData)
			{

				//m_spSelectAnimFileFolder = m_spSelectAnimFileData->wpFolder.lock();
				m_spShowAnimModel = CreateConstructorNative<UAnimModel>(GetDevice(), m_spSelectAnimFileData->wstrfilePath);
				m_spAnimControlModel->SetShowModel(m_spShowAnimModel, m_spSelectAnimFileFolder);

				for (auto& iter : m_spAnimControlModel->GetAnimationClips())
				{
					s_AnimTags[iter.second] = iter.first.c_str();
				}
				m_AnimMaxTagCount = static_cast<_int>(m_spAnimControlModel->GetAnimationClips().size());
			}
		}
		if (nullptr == m_spSelectAnimFileData)
		{
			ImGui::Text("Plz Select AnimationModel");
		}
	}
	ImGui::End();
}

void TAnimControlView::AnimModifyView()
{
	ImGui::Begin(m_stAnimModifyDesc.strName.c_str(), GetOpenPointer(), m_stAnimModifyDesc.imgWindowFlags);
	{
		// select Animation
		m_spAnimControlModel->ShowAnimModify();
		MakeAnimEvent();
	}
	ImGui::End();
}

void TAnimControlView::MakeAnimEvent()
{
	RETURN_CHECK(nullptr == m_spShowAnimModel, ;);

	static const _char* ANIMTYPETAG[]{ "EFFECT", "SOUND", 
		"COLLIDER", "CAMERA", "OBJACTIVE", "ANIMCAHNGEBETWEEN", "ANIMOCCURSTIMEPASS"};

	SHPTR<UAnimation> spCurAnimation = m_spShowAnimModel->GetCurrentAnimation();

	if (ImGui::TreeNodeEx("MakeAnimEvent",ImGuiTreeNodeFlags_Bullet))
	{
		ImGui::Text(m_strSelectAnimationName);


		ImGui::Combo("AnimEvent", &m_iSelectAnimEvent, ANIMTYPETAG, ANIMEVENTTYPE::ANIMEVENT_END);
		if (true == ImGui::Button("SelectAnim"))
		{
			m_spSelectAnim = spCurAnimation;
			m_strSelectAnimationName = UMethod::ConvertWToS(m_spSelectAnim->GetAnimName());
		}
		ImGui::SameLine();
		if (true == ImGui::Button("MakeEvent"))
		{
			ANIMEVENTTYPE Event = static_cast<ANIMEVENTTYPE>(m_iSelectAnimEvent);
			if (nullptr != m_spSelectAnim)
			{
				SHPTR<UAnimEvent> spAnimEvent{ nullptr };
				switch (Event)
				{
				case ANIMEVENT_CAMERA:
					break;
				case ANIMEVENT_ANIMCHANGESBETWEEN:
					spAnimEvent = Create< UAnimChangeBetweenEvent>();
					break;
				case ANIMEVENT_COLLIDER:
					spAnimEvent = Create<UAnimColliderEvent>();
					break;
				case ANIMEVENT_EFFECT:

					break;
				case ANIMEVENT_OBJACTIVE:

					break;
				case ANIMEVENT_ANIMOCCURSTIMEPASS:
					spAnimEvent = Create< UAnimOccursTimePassEvent>();
					break;
				case ANIMEVENT_SOUND:
					spAnimEvent = Create<UAnimSoundEvent>();
					break;
				}
				m_spSelectAnim->InsertAnimEvent(Event, spAnimEvent);
			}
		}
		ImGui::SameLine();
		if (ImGui::Button("AnimEventSave"))
		{
			if (nullptr != spCurAnimation)
				spCurAnimation->SaveAnimEventPathIsFolder(m_spSelectAnimFileFolder->wstrPath);
		}

		if (nullptr != m_spSelectAnim)
		{
			// Options
			static ImGuiTableFlags flags = ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable | ImGuiTableFlags_Sortable | ImGuiTableFlags_SortMulti
				| ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV | ImGuiTableFlags_NoBordersInBody | ImGuiTableFlags_ScrollY;

			// Event Container 
			for (auto& iter : m_spSelectAnim->GetAnimEventContainer())
			{
				switch (iter.first)
				{
				case ANIMEVENT_CAMERA:
					break;
				case ANIMEVENT_ANIMCHANGESBETWEEN:
					AnimSectionShow(m_spSelectAnim, flags, iter.second);
					break;
				case ANIMEVENT_COLLIDER:
					AnimColliderShow(m_spSelectAnim, flags, iter.second);
					break;
				case ANIMEVENT_EFFECT:
					break;
				case ANIMEVENT_OBJACTIVE:
					break;
				case ANIMEVENT_ANIMOCCURSTIMEPASS:
					AnimOccursShow(m_spSelectAnim, flags, iter.second);
					break;
				case ANIMEVENT_SOUND:
					AnimSoundShow(m_spSelectAnim, flags, iter.second);
					break;
				}
			}
		}
		else
		{
			ImGui::Text("Plz select Animation"); 
		}
		ImGui::TreePop();
	}
}

void TAnimControlView::AnimSectionShow(CSHPTRREF<UAnimation> _spAnim, ImGuiTableFlags _flags,  const VECTOR<SHPTR<UAnimEvent>>& _AnimEvent)
{
	if (ImGui::TreeNodeEx("AnimChangesBetweenShow", ImGuiTreeNodeFlags_Bullet))
	{
		static _int SelectRemoveItem{ -1 };

		if (true == ImGui::Button("Remove##1"))
		{
			if (-1 != SelectRemoveItem)
			{
				_spAnim->RemoveAnimEvent(ANIMEVENTTYPE::ANIMEVENT_ANIMCHANGESBETWEEN, SelectRemoveItem);
				SelectRemoveItem = -1;
			}
		}

		ImGui::SetNextItemWidth(-FLT_MIN);
		if (ImGui::BeginTable("AnimChangesBetween", 8, _flags, ImVec2(0.0f, ImGui::GetTextLineHeightWithSpacing() * 20), 0.0f))
		{
			ImGui::TableSetupColumn("InputTrigger", ImGuiTableColumnFlags_WidthStretch);
			ImGui::TableSetupColumn("StartT", ImGuiTableColumnFlags_WidthStretch);
			ImGui::TableSetupColumn("EndT", ImGuiTableColumnFlags_WidthStretch);
			ImGui::TableSetupColumn("NextAnim", ImGuiTableColumnFlags_WidthStretch);
			ImGui::TableSetupColumn("SupV", ImGuiTableColumnFlags_WidthStretch);
			ImGui::TableSetupColumn("NextTA", ImGuiTableColumnFlags_WidthStretch);
			ImGui::TableSetupColumn("ChangeT", ImGuiTableColumnFlags_WidthStretch);
			ImGui::TableSetupScrollFreeze(0, 1); // Make row always visible
			ImGui::TableHeadersRow();

			static _float fNextTimeAcc{ 0.f };
			static _string InputTrigger = "##InputTrigger";
			static _string StartT = "##StartT";
			static _string EndT = "##EndT";
			static _string NextAnim = "##NextAnim";
			static _string SupV = "##SupV";
			static _string NextAnimTimeAcc = "##NextAnimTimeAcc";
			static _string ChangeT = "##ChangeT";
			_int iIndex{ 0 };
			_float Duration = static_cast<_float>(_spAnim->GetDuration());
			for (auto& iter : _AnimEvent)
			{
				 _string Index = _string::to_string(iIndex);
				 ANIMEVENTSECTIONDESC* SectionDesc = remove_const<ANIMEVENTSECTIONDESC*, ANIMEVENTDESC*>(iter->OutAnimEventDesc());
				 ANIMCHANGEDESC* ChangeDesc = remove_const<ANIMCHANGEDESC*, ANIMOTHEREVENTDESC*>(iter->OutOtherEventDesc());

				 _int iSelectAnim = ChangeDesc->iNextAnimIndex;
				 {
					 ImGui::TableNextColumn();
					 ImGui::SetNextItemWidth(-FLT_MIN);
					 _string str = UMethod::ConvertWToS(SectionDesc->wstrEventTrigger);
					 if (true == ImGui::InputText(InputTrigger + Index, &str[0], MAX_BUFFER_LENGTH))
					 {
						 SectionDesc->wstrEventTrigger = UMethod::ConvertSToW(str);
					 }
				 }
				{
					ImGui::TableNextColumn();
					ImGui::SetNextItemWidth(-FLT_MIN);
					_float StartTime = static_cast<_float>(SectionDesc->dStartTime);
					ImGui::InputFloat(StartT + Index, &StartTime, 0.0f, Duration);
					SectionDesc->dStartTime = static_cast<_double>(StartTime);
				}
				{
					ImGui::TableNextColumn();
					ImGui::SetNextItemWidth(-FLT_MIN);
					_float EndTime = static_cast<_float>(SectionDesc->dEndTime);
					ImGui::InputFloat(EndT + Index, &EndTime, 0.0f, Duration);
					SectionDesc->dEndTime = static_cast<_double>(EndTime);
				}
				{
					ImGui::TableNextColumn();
					ImGui::SetNextItemWidth(-FLT_MIN);
					if (true == ImGui::Combo(NextAnim + Index, &iSelectAnim, &s_AnimTags[0], m_AnimMaxTagCount))
					{
						ChangeDesc->iNextAnimIndex = m_spAnimControlModel->GetAnimationClips().find(s_AnimTags[iSelectAnim])->second;
						fNextTimeAcc = static_cast<_float>(m_spShowAnimModel->GetAnimations()[ChangeDesc->iNextAnimIndex]->GetTimeAcc());
					}
				}
				{
					ImGui::TableNextColumn();
					ImGui::SetNextItemWidth(-FLT_MIN);
					ImGui::InputFloat(SupV + Index, &ChangeDesc->fSupplyAnimValue);
				}
				{
					ImGui::TableNextColumn();
					ImGui::SetNextItemWidth(-FLT_MIN);
					_float Time = static_cast<_float>(ChangeDesc->dNextAnimTimeAcc);
					ImGui::InputFloat(NextAnimTimeAcc + Index, &Time);
					ChangeDesc->dNextAnimTimeAcc = static_cast<_double>(Time);
				}					

			/*	{
					ImGui::TableNextColumn();
					ImGui::SetNextItemWidth(-FLT_MIN);
					ImGui::InputFloat(ChangeT + Index, &SectionDesc->fAnimChangeTime);
				}*/
				if (ImGui::IsItemClicked())
					SelectRemoveItem = iIndex;
				++iIndex;
				ImGui::TableNextRow();
			}
			ImGui::EndTable();
		}
		ImGui::TreePop();
	}
}

void TAnimControlView::AnimOccursShow(CSHPTRREF<UAnimation> _spAnim, ImGuiTableFlags _flags, const VECTOR<SHPTR<UAnimEvent>>& _AnimEvent)
{
	if (ImGui::TreeNodeEx("AnimOccursTimePassShow", ImGuiTreeNodeFlags_Bullet))
	{
		ImGui::SetNextItemWidth(-FLT_MIN);
		if (ImGui::BeginTable("AnimOccursTimePass", 6, _flags, ImVec2(0.0f, ImGui::GetTextLineHeightWithSpacing() * 20), 0.0f))
		{
			ImGui::TableSetupColumn("InputTrigger", ImGuiTableColumnFlags_WidthStretch);
			ImGui::TableSetupColumn("Over", ImGuiTableColumnFlags_WidthStretch);
			ImGui::TableSetupColumn("NextAnim", ImGuiTableColumnFlags_WidthStretch);
			ImGui::TableSetupColumn("SupV", ImGuiTableColumnFlags_WidthStretch);
			ImGui::TableSetupColumn("NextTA", ImGuiTableColumnFlags_WidthStretch);
			ImGui::TableSetupScrollFreeze(0, 1); // Make row always visible
			ImGui::TableHeadersRow();

			static _float fNextTimeAcc{ 0.f };

			static _string InputTrigger = "##InputTrigger2";
			static _string OverT = "##OverT2";
			static _string NextAnim = "##NextAnim2";
			static _string SupV = "##SupV2";
			static _string NextAnimTimeAcc = "##NextAnimTimeAcc2";
			static _string ChangeT = "##ChangeT2";
			static _int SelectRemoveItem{ -1 };

			if (true == ImGui::Button("Remove##2"))
			{
				if (-1 != SelectRemoveItem)
				{
					_spAnim->RemoveAnimEvent(ANIMEVENTTYPE::ANIMEVENT_ANIMOCCURSTIMEPASS, SelectRemoveItem);
					SelectRemoveItem = -1;
				}
			}

			_int iIndex{ 0 };
			_float Duration = static_cast<_float>(_spAnim->GetDuration());
			for (auto& iter : _AnimEvent)
			{
				 _string Index = _string::to_string(iIndex);

				ANIMOCURRESDESC* OccursDesc = remove_const<ANIMOCURRESDESC*, ANIMEVENTDESC*>(iter->OutAnimEventDesc());
				ANIMCHANGEDESC* ChangeDesc = remove_const<ANIMCHANGEDESC*, ANIMOTHEREVENTDESC*>(iter->OutOtherEventDesc());

				_int iSelectAnim = ChangeDesc->iNextAnimIndex;
				{
					ImGui::TableNextColumn();
					ImGui::SetNextItemWidth(-FLT_MIN);
					_string str = UMethod::ConvertWToS(OccursDesc->wstrEventTrigger);
					if (true == ImGui::InputText(InputTrigger + Index, &str[0], MAX_BUFFER_LENGTH))
					{
						OccursDesc->wstrEventTrigger = UMethod::ConvertSToW(str);
					}
				}
				{
					ImGui::TableNextColumn();
					ImGui::SetNextItemWidth(-FLT_MIN);
					_float Time = static_cast<_float>(OccursDesc->dAnimOccursTime);
					ImGui::InputFloat(OverT + Index, &Time, 0.0f, Duration);
					OccursDesc->dAnimOccursTime = static_cast<_double>(Time);
				}
				{
					ImGui::TableNextColumn();
					ImGui::SetNextItemWidth(-FLT_MIN);
					if (true == ImGui::Combo(NextAnim + Index, &iSelectAnim, &s_AnimTags[0], m_AnimMaxTagCount))
					{
						ChangeDesc->iNextAnimIndex = m_spAnimControlModel->GetAnimationClips().find(s_AnimTags[iSelectAnim])->second;
						fNextTimeAcc = static_cast<_float>(m_spShowAnimModel->GetAnimations()[ChangeDesc->iNextAnimIndex]->GetTimeAcc());
					}
				}
				{
					ImGui::TableNextColumn();
					ImGui::SetNextItemWidth(-FLT_MIN);
					ImGui::InputFloat(SupV + Index, &ChangeDesc->fSupplyAnimValue);
				}
				{
					ImGui::TableNextColumn();
					ImGui::SetNextItemWidth(-FLT_MIN);
					_float Time = static_cast<_float>(ChangeDesc->dNextAnimTimeAcc);
					ImGui::InputFloat(NextAnimTimeAcc + Index, &Time);
					ChangeDesc->dNextAnimTimeAcc = static_cast<_double>(Time);
				}

				{
					ImGui::TableNextColumn();
					ImGui::SetNextItemWidth(-FLT_MIN);
					_float Time = static_cast<_float>(ChangeDesc->dNextAnimTimeAcc);
					ImGui::InputFloat(NextAnimTimeAcc + Index, &Time);
					ChangeDesc->dNextAnimTimeAcc = static_cast<_double>(Time);
				}
				{
					ImGui::TableNextColumn();
					ImGui::SetNextItemWidth(-FLT_MIN);
					//ImGui::InputFloat(ChangeT + Index, &OccursDesc->fAnimChangeTime);
				}
				if (ImGui::IsItemClicked())
					SelectRemoveItem = iIndex;
				++iIndex;
				ImGui::TableNextRow();
			}
			ImGui::EndTable();
		}
		ImGui::TreePop();
	}
}

void TAnimControlView::AnimColliderShow(CSHPTRREF<UAnimation> _spAnim, ImGuiTableFlags _flags, const VECTOR<SHPTR<UAnimEvent>>& _AnimEvent)
{
	static const _char* COLLIDERTAG[]{ "AABB", "OBB", "SPHERE"};
	static _string COLLIDER_NAME{ "MakeCollider##" };

	static _string InputTrigger = "InputTrigger";
	static _string StartT = "Start";
	static _string EndT = "End";
	static _string Collider = "##Collider3";
	static _string BoneNodeStr = "##BoneNode3";
	static _string CreateColliderButton = "Create";
	static _string ReadColliderPos = "CollPos";
	static _string TranslateCollider  = "Translater";
	static _string ScaleCollider = "Scale";
	static _string Remove{ "Remove" };
	static _int SelectRemoveItem{ -1 };
	if (ImGui::TreeNodeEx("AnimColliderShow", ImGuiTreeNodeFlags_Bullet))
	{
		_int iIndex{ 0 };
		_float Duration = static_cast<_float>(_spAnim->GetDuration());
		for (auto& iter : _AnimEvent)
		{
			_string Index = "##T3" + _string::to_string(iIndex);
			if (ImGui::TreeNodeEx(COLLIDER_NAME + Index))
			{
				ANIMEVENTSECTIONDESC* SectionDesc = remove_const<ANIMEVENTSECTIONDESC*, ANIMEVENTDESC*>(iter->OutAnimEventDesc());
				ANIMCOLLIDERDESC* ChangeDesc = remove_const<ANIMCOLLIDERDESC*, ANIMOTHEREVENTDESC*>(iter->OutOtherEventDesc());
				// 1
				{
					ImGui::SetNextItemWidth(300);
					_string str = UMethod::ConvertWToS(SectionDesc->wstrEventTrigger);
					if (true == ImGui::InputText(InputTrigger + Index, &str[0], MAX_BUFFER_LENGTH))
					{
						SectionDesc->wstrEventTrigger = UMethod::ConvertSToW(str);
					}
				}
				// 2
				{
					ImGui::SetNextItemWidth(100);
					_float StartTime = static_cast<_float>(SectionDesc->dStartTime);
					ImGui::InputFloat(StartT + Index, &StartTime, 0.0f, Duration);
					SectionDesc->dStartTime = static_cast<_double>(StartTime);
				}
				// 3
				{
					ImGui::SameLine();
					ImGui::SetNextItemWidth(100);
					_float EndTime = static_cast<_float>(SectionDesc->dEndTime);
					ImGui::InputFloat(EndT + Index, &EndTime, 0.0f, Duration);
					SectionDesc->dEndTime = static_cast<_double>(EndTime);
				}
				// 4
				{
					ImGui::SetNextItemWidth(100);
					_string BoneStr{ "NO_SEL" };
					if (nullptr != ChangeDesc->spBoneNode) {
						BoneStr = UMethod::ConvertWToS(ChangeDesc->spBoneNode->GetName());
					}
					if (ImGui::BeginCombo(BoneNodeStr + Index, BoneStr.c_str())) {

						_bool isTrue;
						static _string NO_SEL{ "NO_SEL" };
						if (ImGui::Selectable(NO_SEL + Index , &isTrue)) {
							ChangeDesc->spBoneNode = nullptr;
							ImGui::SetItemDefaultFocus();
						}
						else
						{
							for (auto& BoneNode : m_spShowAnimModel->GetBoneNodes())
							{
								_string ConvertBoneStr = UMethod::ConvertWToS(BoneNode->GetName());
								if (ImGui::Selectable(ConvertBoneStr.c_str(), &isTrue)) {
									ChangeDesc->spBoneNode = BoneNode;
									ImGui::SetItemDefaultFocus();
								}
							}
						}
						ImGui::EndCombo();
					}
				}
				// 5
				{
					ImGui::SameLine();
					ImGui::SetNextItemWidth(100);
					ImGui::Combo(Collider + Index, &ChangeDesc->iColliderType, COLLIDERTAG, 3);
				}
				// 6
				{
					ImGui::SameLine();
					ImGui::SetNextItemWidth(100);
					if (true == ImGui::Button(CreateColliderButton + Index))
					{
						UCollider::COLLIDERDESC Desc{ _float3{1.f, 1.f, 1.f}, _float3{0.f, 0.f, 0.f} };
						switch (ChangeDesc->iColliderType)
						{
						case UCollider::TYPE_AABB:
							ChangeDesc->spCollider = std::static_pointer_cast<UCollider>(GetGameInstance()->CloneComp(PROTO_COMP_ABBCOLLIDER, VOIDDATAS{ &Desc }));
							break;
						case UCollider::TYPE_OBB:
							ChangeDesc->spCollider = std::static_pointer_cast<UCollider>(GetGameInstance()->CloneComp(PROTO_COMP_OBBCOLLIDER, VOIDDATAS{ &Desc }));
							break;
						case UCollider::TYPE_SPHERE:
							ChangeDesc->spCollider = std::static_pointer_cast<UCollider>(GetGameInstance()->CloneComp(PROTO_COMP_SPHERECOLLIDER, VOIDDATAS{ &Desc }));
							break;
						}
					}
				}
				if (nullptr != ChangeDesc->spCollider)
				{
					// 7 Show Pos
					{
						_float3 vCurPos = ChangeDesc->spCollider->GetCurPos();
						ImGui::InputFloat3(ReadColliderPos + Index, &vCurPos.x, "%.3f", ImGuiInputTextFlags_ReadOnly);
					}
					// 8 
					{
						_float3 vTranslate = ChangeDesc->spCollider->GetTranslate();
						if (true == ImGui::InputFloat3(TranslateCollider + Index, &vTranslate.x)) {
							ChangeDesc->spCollider->SetTranslate(vTranslate);
						}
					}
					// 9
					{
						_float3 vScale = ChangeDesc->spCollider->GetScale();
						switch (ChangeDesc->iColliderType)
						{
						case UCollider::TYPE_SPHERE:
							if (true == ImGui::InputFloat(ScaleCollider + Index, &vScale.x)){
								ChangeDesc->spCollider->SetScale(vScale);
							}
							break;
						case UCollider::TYPE_OBB:
						case UCollider::TYPE_AABB:
							if (true == ImGui::InputFloat3(ScaleCollider + Index, &vScale.x)) {
								ChangeDesc->spCollider->SetScale(vScale);
							}
							break;
						}
					}
				}
				if (ImGui::Button(Remove + Index))
				{
					SelectRemoveItem = iIndex;
				}
				++iIndex;
				ImGui::TreePop();
			}
		}
		if (-1 != SelectRemoveItem)
		{
			_spAnim->RemoveAnimEvent(ANIMEVENTTYPE::ANIMEVENT_COLLIDER, SelectRemoveItem);
			SelectRemoveItem = -1;
		}
		ImGui::TreePop();
	}
}

void TAnimControlView::AnimSoundShow(CSHPTRREF<UAnimation> _spAnim, ImGuiTableFlags _flags, const VECTOR<SHPTR<UAnimEvent>>& _AnimEvent)
{
	if (ImGui::TreeNodeEx("SoundEventList", ImGuiTreeNodeFlags_Bullet))
	{
		static _string InputTrigger = "InputTrigger";
		static _string OverT = "OverT";
		static _string RemoveButton{ "Remove" };
		static _string InputSoundName;
		static 	ANIMSOUNDDESC* SelectSoundDesc{ nullptr };
		static _int SelectRemoveItem{ -1 };
		// Find Sounds 
		{
			SHPTR<UGameInstance> spGameInstance = GetGameInstance();
			SHPTR<UAudioSystem> spAudioSystem = spGameInstance->GetAudioSystem(SOUND_GAME);

			if (true == ImGui::InputText("FindSName", &InputSoundName[0], MAX_BUFFER_LENGTH))
			{
				m_FindSoundNames.clear();
				// AudioSystem ����Ʈ ���� 
				_wstring wstrConvertInputSoundName = UMethod::ConvertSToW(InputSoundName);
				for (auto& Sound : spAudioSystem->GetSoundOrders())
				{
					if (true == UMethod::Is_Same_Text(Sound.first, wstrConvertInputSoundName))
					{
						m_FindSoundNames.push_back(UMethod::ConvertWToS(Sound.first));
					}
				}
			}
			if (ImGui::BeginListBox("Sound List", ImVec2(-FLT_MIN, 5 * ImGui::GetTextLineHeightWithSpacing())))
			{
				for (auto& iter : m_FindSoundNames)
				{
					if (true == ImGui::Selectable(iter))
					{
						if (nullptr != SelectSoundDesc)
						{
							SelectSoundDesc->wstrSoundName = UMethod::ConvertSToW(iter);
						}
					}
				}
				ImGui::EndListBox();
			}

		}
		// Modify
		{
			_int iIndex{ 0 };
			_float Duration = static_cast<_float>(_spAnim->GetDuration());
			static  _string ImGuiSoundNodes = "SoundEvent";
			for (auto& iter : _AnimEvent)
			{
				_string Index = "##T4" + _string::to_string(iIndex);

				if (true == ImGui::TreeNodeEx(ImGuiSoundNodes + Index, ImGuiTreeNodeFlags_Bullet))
				{
					ANIMOCURRESDESC* OccursDesc = remove_const<ANIMOCURRESDESC*, ANIMEVENTDESC*>(iter->OutAnimEventDesc());
					ANIMSOUNDDESC* ChangeDesc = remove_const<ANIMSOUNDDESC*, ANIMOTHEREVENTDESC*>(iter->OutOtherEventDesc());
					if (true == ImGui::Button("Selectable Sound"))
					{
						SelectSoundDesc = ChangeDesc;
					}
					// 1
					{
						ImGui::SetNextItemWidth(100);
						_string str = UMethod::ConvertWToS(OccursDesc->wstrEventTrigger);
						if (true == ImGui::InputText(InputTrigger + Index, &str[0], MAX_BUFFER_LENGTH))
						{
							OccursDesc->wstrEventTrigger = UMethod::ConvertSToW(str);
						}
					}
					ImGui::SameLine();
					// 2
					{
						ImGui::SetNextItemWidth(100);
						_float OccursTime = static_cast<_float>(OccursDesc->dAnimOccursTime);
						ImGui::DragFloat(OverT + Index, &OccursTime, 0.0f, Duration);
						OccursDesc->dAnimOccursTime = static_cast<_double>(OccursTime);
					}
					// 3
					{
						ImGui::SetNextItemWidth(100);
						ImGui::Text("Select Sound Name: %s", UMethod::ConvertWToS(ChangeDesc->wstrSoundName).c_str());
					}
					// 4
					{
						ImGui::SetNextItemWidth(300);
						ImGui::InputFloat3("SoundVelocity", &ChangeDesc->vSoundVelocity.x);
					}

					if (ImGui::Button(RemoveButton + Index))
					{
						SelectRemoveItem = iIndex;
					}
					++iIndex;
					ImGui::TreePop();
				}
			}

			if (-1 != SelectRemoveItem)
			{
				_spAnim->RemoveAnimEvent(ANIMEVENTTYPE::ANIMEVENT_SOUND, SelectRemoveItem);
				SelectRemoveItem = -1;
			}
		}
		ImGui::TreePop();
	}
}