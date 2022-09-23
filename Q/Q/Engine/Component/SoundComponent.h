#pragma once
/*
    File Name: SoundComponent.h
    Project Name: Q
    Author(s):
        Primary: Sieun Choi
        Secondary: Yoonki Kim
    All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/

#include <fmod/fmod.h>
#include <sol/state.hpp>
#include <string>
#include <unordered_map>

#include "../EngineComponent.h"

namespace q_engine {

	class SoundComponent : public EngineComponent<SoundComponent>
	{

	private:
		FMOD_SYSTEM* System;
		FMOD_SOUND** BGSound;
		FMOD_SOUND** EffectSound;
		FMOD_SOUND** EffectLoopSound = nullptr;
		FMOD_CHANNEL** BGChannel;
		FMOD_CHANNEL** EffectChannel = nullptr;
		FMOD_CHANNEL** EffectLoopChannel = nullptr;
		FMOD_CHANNELGROUP* ChannelGroup = nullptr;

		int EffectCount;
		int LoopEffectCount;
		int BGCount;
		float Volume;
		float BGVolume;
		float EffectVolume;
		
		static const float MaxSound;
		static const float MinSound;
		static const float Soundvalue;

		bool IsPlaying();
		FMOD_BOOL* isplaying_bg = nullptr;
		FMOD_BOOL* isplaying_effect = nullptr;
		
		using hashed_sound_name = size_t;
		 
		std::vector<hashed_sound_name> bg_sounds;
		std::vector<hashed_sound_name> effect_sounds;
		std::vector<hashed_sound_name> loop_effect_sounds;

		std::vector<std::string> bg_sound_paths;
		std::vector<std::string> effect_sound_paths;
		std::vector<std::string> loop_effect_sound_paths;

		void LoadBG(std::string&& sound_file_path);
		void LoadEffect(std::string&& sound_file_path);
		void LoadLoopEffect(std::string&& sound_file_path);

	    void CreateEffectSound(int nCount, std::string* SoundFileName);
		void CreateLoopEffectSound(int nCount, std::string* SoundFileName);
		void CreateBGSound(int nCount, std::string* SoundFileName);



        static std::string ExtractName(const std::string& sound_file_path);
	public:

		void Load() override;
		void PlaySoundEffect(int nIndex, bool looping);
		void PlaySoundEffect(std::string sound_path, bool looping);
		void PlaySoundBG(int nIndex);
		void PlaySoundBG(std::string sound_path);
		void StopSoundBG(int nIndex);
		void StopSoundLoopEffect(int nIndex);
		void StopSoundLoopEffect(std::string sound_path);
		void StopSoundEffect();
		void StopSoundAll();
		void Unload() override;
		void VolumeUp();
		void VolumeDown();
		float  GetCurEffectVolume() const;
		float  GetCurBGVolume() const;
		std::pair<float, float> GetVolumeRange() const;
		void SetBGVolume(float nVolume);
		void SetEffectVolume(float nVolume);
		void EffectSoundVolumeUp();
		void EffectSoundVolumeDown();
		void PauseSoundBG(int nIndex);
		void PauseAll();
		void ResumeAll();
		bool ResumeSoundBG(int nIndex);
		void ResumeSoundBG(std::string sound_path);
		void Update(double dt) override;
		void PlayBG0();
		void StopBG0();
		void PlayEffect0();
		void OffAllSound();
		bool CheckOnSound();

		SoundComponent();
		~SoundComponent();
	};
}


inline void RegisterSoundComponent(sol::state& state)
{
	sol::usertype<q_engine::SoundComponent> soundComponent = state.new_usertype<q_engine::SoundComponent>("SoundComponent");
	soundComponent.set_function("PlaySoundEffect", sol::resolve<void(std::string, bool)>(&q_engine::SoundComponent::PlaySoundEffect));
	soundComponent.set_function("PlaySoundBG", sol::resolve<void(std::string)>(&q_engine::SoundComponent::PlaySoundBG));
	soundComponent.set_function("StopSoundAll", (&q_engine::SoundComponent::StopSoundAll));
	soundComponent.set_function("ResumeAll", sol::resolve<void()>(&q_engine::SoundComponent::ResumeAll));
	soundComponent.set_function("ResumeSoundBG", sol::resolve<void(std::string)>(&q_engine::SoundComponent::ResumeSoundBG));
	soundComponent.set_function("PauseAll", sol::resolve<>(&q_engine::SoundComponent::PauseAll));
}