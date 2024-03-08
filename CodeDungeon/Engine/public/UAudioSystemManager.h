#pragma once

#include <fmod/fmod.hpp>
#include <fmod/fmod_errors.h>
#include "UBase.h"

BEGIN(Engine)
class UGameInstance;
class UAudioSystem;
class USound;
class UTransform;

using AUDIOSYSTEMCONTAINER = ARRAY <SHPTR< UAudioSystem>, SOUND_END>;
/*
@ Date: 2024-02-25, Writer: ������
@ Explain
- UAudioSystem�� �����ϴ� �Ŵ��� Ŭ����
*/
class UAudioSystemManager final : public UBase {
public:
	UAudioSystemManager();
	NO_COPY(UAudioSystemManager)
	DESTRUCTOR(UAudioSystemManager)
public:
	const AUDIOSYSTEMCONTAINER& GetAudioSystemContainer() const { return m_AudioSystemContainer; }
	SHPTR<UAudioSystem> GetAudioSystem(const SOUNDTYPE _SoundType);
public:
	HRESULT ReadyAudioSystemManager(UGameInstance* _pGameInstance);

	HRESULT CreateAudioSystemAndRegister(UGameInstance* _pGameInstance,  SOUNDTYPE _SoundType, const _wstring& _wstrSoundFolderPath);
	HRESULT CreateAudioSystemAndRegister(UGameInstance* _pGameInstance, SOUNDTYPE _SoundType, CSHPTRREF<FILEGROUP> _spSoundFileGroup);
	HRESULT CreateAudioSystemToFolderNameAndRegister(UGameInstance* _pGameInstance, SOUNDTYPE _SoundType, const _wstring& _wstrSoundFolderName);

	void Play(const _wstring& _wstrSoundName);
	void Play(const _wstring& _wstrSoundName, const _float& _fVolumeUpdate);
	void PlayBGM(const _wstring& _wstrSoundName);
	void PlayBGM(const _wstring& _wstrSoundName, const _float& _fVolumeUpdate);
	void Stop(const _wstring& _wstrSoundName);
	void VolumeUpdate(const _wstring& _wstrSoundName, const _float& _fVolumeUpdate);

	void UpdateSound3D(const _wstring& _wstrSoundName, const _float3& _vSoudPos, const _float3& _vSoundVelocity, 
		CSHPTRREF<UTransform> _spTargetTransform = nullptr);
	void UpdateSound3D(const _wstring& _wstrSoundName, CSHPTRREF<UTransform> _spSoundTransform, const _float3& _vSoundVelocity,
		CSHPTRREF<UTransform> _spTargetTransform = nullptr);
	void ChangeMinMaxDistance3D(const _wstring& _wstrSoundName, const _float _fMinDistance, const _float _fMaxDistance);

	SHPTR<USound> BringSound(const _int _Index);
	SHPTR<USound> BringSound(const _wstring& _wstrSoundName);
private:
	virtual void Free() override;
private:
	FMOD::System*						m_pSystem;
	AUDIOSYSTEMCONTAINER	m_AudioSystemContainer;
};


END