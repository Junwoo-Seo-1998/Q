/*
    File Name: SoundComponent.cpp
    Project Name: Q
    Author(s):
        Primary: Sieun Choi
        Secondary: Yoonki Kim
    All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/

#include "SoundComponent.h"
#include <stdexcept>
#include "../ECS/InputManager.h"
#include "../Engine/Engine.h"
#include <filesystem>

#include "Engine/Core/Timer.h"

namespace q_engine {

	const float SoundComponent::MaxSound = 1.0f;
	const float SoundComponent::MinSound = 0.0f;
	const float SoundComponent::Soundvalue = 0.1f;

	SoundComponent::SoundComponent() : EffectCount(0), BGCount(0), Volume(0.5f), BGVolume(0.5),BGSound(nullptr), EffectSound(nullptr), BGChannel(nullptr)
	,EffectVolume(0.5f)
	{

	}


	SoundComponent::~SoundComponent()
	{
		FMOD_System_Close(System);
		FMOD_System_Release(System);
	}

    void SoundComponent::LoadBG(std::string&& sound_file_path)
    {
		std::string name = ExtractName(sound_file_path);
		bg_sound_paths.push_back(sound_file_path);
		bg_sounds.push_back(get_hash_id(name));
    }

    void SoundComponent::LoadEffect(std::string&& sound_file_path)
    {
		std::string name = ExtractName(sound_file_path);
		effect_sound_paths.push_back(sound_file_path);
		effect_sounds.push_back(get_hash_id(name));
    }
	void SoundComponent::LoadLoopEffect(std::string&& sound_file_path)
	{
		std::string name = ExtractName(sound_file_path);
		loop_effect_sound_paths.push_back(sound_file_path);
		loop_effect_sounds.push_back(get_hash_id(name));
	}
	std::string SoundComponent::ExtractName(const std::string& sound_file_path)
	{
		std::filesystem::path path{ sound_file_path };
		std::string extracted_name = path.filename().replace_extension().string();
		return extracted_name;
	}

	void SoundComponent::Load()
	{
		FMOD_System_Create(&System);
		FMOD_System_Init(System, 64, FMOD_INIT_NORMAL, NULL);
		FMOD_System_CreateChannelGroup(System, "Sound Channel", &ChannelGroup);

		auto& input_manager = Engine::GetEngineComponent<InputManager>();
		input_manager.LoadBindings("SoundComponent.cfg");
		input_manager.AddCallback("PlayBg", &SoundComponent::PlayBG0, this);
		input_manager.AddCallback("StopBg", &SoundComponent::StopBG0, this);
		input_manager.AddCallback("PlayEffect", &SoundComponent::PlayEffect0, this);
		input_manager.AddCallback("VolUp", &SoundComponent::VolumeUp, this);
		input_manager.AddCallback("VolDown", &SoundComponent::VolumeDown, this);
		
		LoadBG("resource/sound/bgm.mp3");
		LoadBG("resource/sound/boss_bgm.mp3");
		LoadBG("resource/sound/MyVeryOwnDeadShip.ogg");
		LoadBG("resource/sound/bgm2.mp3");
		LoadBG("resource/sound/bgm3.mp3");

		LoadLoopEffect("resource/sound/portal-open.wav");
		LoadEffect("resource/sound/portal-warp.wav");

		LoadEffect("resource/sound/button-interaction.wav");
		LoadEffect("resource/sound/splash-bgm.wav");
		LoadEffect("resource/sound/menu_transition.wav");
		LoadEffect("resource/sound/gun-shot.wav");
		LoadEffect("resource/sound/sword-swing1.wav");
		LoadEffect("resource/sound/sword-swing2.wav");
	    LoadEffect("resource/sound/sword-swing3.wav");
		LoadEffect("resource/sound/bomb-explosion.wav");
		LoadEffect("resource/sound/gun-reload.wav");
		LoadLoopEffect("resource/sound/player-walking.wav");

		LoadEffect("resource/sound/player-jumping.mp3");

		LoadEffect("resource/sound/player_hurt_die.wav");
		LoadEffect("resource/sound/player_hurt1.wav");
		LoadEffect("resource/sound/player_hurt2.wav");

		LoadEffect("resource/sound/item_swap.wav");
		LoadEffect("resource/sound/item_pickup.wav");

		LoadEffect("resource/sound/enemy_hit_dead.wav");
		LoadEffect("resource/sound/enemy_hit1.wav");
		LoadEffect("resource/sound/enemy_hit2.wav");
		LoadEffect("resource/sound/enemy_hit3.wav");
		
		LoadEffect("resource/sound/drink_sound.wav");
		LoadEffect("resource/sound/out-of-ammo.wav");

		LoadEffect("resource/sound/shotgun-reload.wav");

		LoadEffect("resource/sound/EnemyHit/EnemyHit1.wav");
		LoadEffect("resource/sound/EnemyHit/EnemyHit2.wav");
		LoadEffect("resource/sound/EnemyHit/EnemyHit3.wav");
		LoadEffect("resource/sound/EnemyHit/EnemyHit4.wav");

		LoadEffect("resource/sound/Explode/Explode1.wav");
		LoadEffect("resource/sound/Explode/Explode2.wav");
		LoadEffect("resource/sound/Explode/Explode3.wav");
		LoadEffect("resource/sound/Explode/Explode4.wav");
		LoadEffect("resource/sound/Explode/Explode5.wav");

		LoadEffect("resource/sound/shield_hit.wav");

		LoadEffect("resource/sound/rocket-launch.mp3");

		CreateEffectSound(static_cast<int>(effect_sound_paths.size()), effect_sound_paths.data());
		CreateLoopEffectSound(static_cast<int>(loop_effect_sound_paths.size()), loop_effect_sound_paths.data());
		CreateBGSound(static_cast<int>(bg_sound_paths.size()), bg_sound_paths.data());
		OffAllSound();
		FMOD_ChannelGroup_SetVolume(ChannelGroup, Volume);

		//SetBGVolume(BGVolume);
		//SetEffectVolume(EffectVolume);
	}

	void SoundComponent::CreateBGSound(int nCount, std::string* SoundFileName)
	{
		//BG SOUND
		BGCount = nCount;
		//allocate size of background list
		BGSound = new FMOD_SOUND * [nCount];
		//also make size of background music list
		BGChannel = new FMOD_CHANNEL * [nCount];
		isplaying_bg = new FMOD_BOOL[nCount]{false};

		for (int i = 0; i < nCount; i++)
		{
			FMOD_System_CreateSound(System, SoundFileName[i].c_str(), FMOD_LOOP_NORMAL| FMOD_UNIQUE, nullptr, &BGSound[i]);
		}
	}


	void SoundComponent::CreateEffectSound(int nCount, std::string* SoundFileName)
	{
		//Effect Sound 
		EffectCount = nCount;
		//allocate size of effect sound list
		EffectSound = new FMOD_SOUND * [nCount];
		//also make size of background music list
		EffectChannel = new FMOD_CHANNEL * [nCount];
		for (int i = 0; i < nCount; i++)
		{
			FMOD_System_CreateSound(System, SoundFileName[i].c_str(), FMOD_DEFAULT| FMOD_UNIQUE, 0, &EffectSound[i]);
		}
	}
	void SoundComponent::CreateLoopEffectSound(int nCount, std::string* SoundFileName)
	{
		//Effect Sound 
		LoopEffectCount = nCount;
		//allocate size of effect sound list
		EffectLoopSound = new FMOD_SOUND * [nCount];
		//also make size of background music list
		EffectLoopChannel = new FMOD_CHANNEL * [nCount];
		isplaying_effect = new FMOD_BOOL[nCount]{ false };
		for (int i = 0; i < nCount; i++)
		{
			FMOD_System_CreateSound(System, SoundFileName[i].c_str(), FMOD_LOOP_NORMAL| FMOD_UNIQUE, 0, &EffectLoopSound[i]);
		}
	}
	
	void SoundComponent::PlaySoundEffect(int nIndex, bool looping)
	{
		if(EffectVolume <= 0.f)
		{
			return;

		}

		if (nIndex < EffectCount && !looping)
		{
			int result = FMOD_System_PlaySound(System, EffectSound[nIndex], ChannelGroup, 0, &EffectChannel[nIndex]);
			FMOD_Channel_SetVolume(EffectChannel[nIndex], EffectVolume);
		}
		else if(nIndex < EffectCount && looping && isplaying_effect[nIndex] == static_cast<FMOD_BOOL>(false))
		{
			FMOD_System_PlaySound(System, EffectLoopSound[nIndex], ChannelGroup, 0, &EffectLoopChannel[nIndex]);
			FMOD_Channel_SetVolume(EffectLoopChannel[nIndex], EffectVolume);
			isplaying_effect[nIndex] = true;

		}
		else if (nIndex >= BGCount || nIndex < 0)
		{
			throw std::invalid_argument("Invalid Index value");
		}
	}

    void SoundComponent::PlaySoundEffect(std::string sound_path,bool looping)
    {
		if (!looping) 
		{
			const auto where = std::distance(effect_sounds.begin(), std::find(effect_sounds.begin(), effect_sounds.end(), get_hash_id(sound_path)));
			PlaySoundEffect(static_cast<int>(where), looping);
		}
		else
		{
			const auto where = std::distance(loop_effect_sounds.begin(), std::find(loop_effect_sounds.begin(), loop_effect_sounds.end(), get_hash_id(sound_path)));
			PlaySoundEffect(static_cast<int>(where), looping);
		}
    }

    void SoundComponent::PlaySoundBG(std::string sound_path)
    {
		const auto where = std::distance(bg_sounds.begin(), std::find(bg_sounds.begin(), bg_sounds.end(), get_hash_id(sound_path)));
		PlaySoundBG(static_cast<int>(where));
    }

    void SoundComponent::PlaySoundBG(int nIndex)
	{
		if (nIndex < BGCount && isplaying_bg[nIndex] == static_cast<FMOD_BOOL>(false))
		{
			FMOD_System_PlaySound(System, BGSound[nIndex], ChannelGroup, false, &BGChannel[nIndex]);
			FMOD_Channel_SetVolume(BGChannel[nIndex], BGVolume);
			isplaying_bg[nIndex] = true;
		}
		else if (nIndex >= BGCount || nIndex < 0)
		{
			throw std::invalid_argument("Invalid Index value");
		}
	}

	void SoundComponent::StopSoundBG(int nIndex)
	{
		if (nIndex < BGCount && isplaying_bg[nIndex] == static_cast<FMOD_BOOL>(true))
		{
			FMOD_Channel_Stop(BGChannel[nIndex]);
			isplaying_bg[nIndex] = false;
		}
		else if (nIndex >= BGCount || nIndex < 0)
		{
			throw std::invalid_argument("Invalid Index value");
		}
	}
	
	void SoundComponent::StopSoundEffect()
	{
		for(int i = 0 ; i < EffectCount ;++i)
		{ 
			FMOD_Channel_Stop(EffectChannel[i]);
		}
		for (int i = 0; i < LoopEffectCount; ++i)
		{
			FMOD_Channel_Stop(EffectLoopChannel[i]);
			isplaying_effect[i] = false;
		}
	}
	
	void SoundComponent::StopSoundLoopEffect(int nIndex)
	{
		if (nIndex < LoopEffectCount && isplaying_effect[nIndex] == static_cast<FMOD_BOOL>(true))
		{
			FMOD_Channel_Stop(EffectLoopChannel[nIndex]);
			isplaying_effect[nIndex] = false;
		}
		else if (nIndex >= LoopEffectCount || nIndex < 0)
		{
			throw std::invalid_argument("Invalid Index value");
		}
	}
	void SoundComponent::StopSoundLoopEffect(std::string sound_path)
	{
		const auto where = std::distance(loop_effect_sounds.begin(), std::find(loop_effect_sounds.begin(), loop_effect_sounds.end(), get_hash_id(sound_path)));
		StopSoundLoopEffect(where);

	}
	void SoundComponent::StopSoundAll()
	{
		if (CheckOnSound())
		{
			FMOD_ChannelGroup_Stop(ChannelGroup);
			OffAllSound();
		}
	}
	void SoundComponent::Unload()
	{

		delete[] BGChannel;
		delete[] EffectChannel;
		delete[] EffectLoopChannel;
		delete[] isplaying_bg;
		delete[] isplaying_effect;
		for (int i = 0; i < BGCount; i++)
			FMOD_Sound_Release(BGSound[i]);
		delete[] BGSound;
		for (int i = 0; i < EffectCount; i++)
			FMOD_Sound_Release(EffectSound[i]);
		delete[] EffectSound;
		for (int i = 0; i < LoopEffectCount; i++)
			FMOD_Sound_Release(EffectLoopSound[i]);
		delete[] EffectLoopSound;

	}

	void SoundComponent::Update(double)
	{
		FMOD_ChannelGroup_SetPitch(ChannelGroup, static_cast<float>(Engine::GetSlowMotionRatio()));
	    if (!System)
		{

			FMOD_System_Update(System);
		}
	}

	void SoundComponent::VolumeUp()
	{
		if (Volume < MaxSound)
		{
			Volume += Soundvalue;
		}
		
		FMOD_ChannelGroup_SetVolume(ChannelGroup, Volume);
		
	}
	void SoundComponent::VolumeDown()
	{
		if (Volume > MinSound)
		{
			Volume -= Soundvalue;
		}
		FMOD_ChannelGroup_SetVolume(ChannelGroup, Volume);

	}

	float SoundComponent::GetCurEffectVolume() const
    {
		return EffectVolume;
    }

	float SoundComponent::GetCurBGVolume() const
    {
		return BGVolume;
    }

    std::pair<float, float> SoundComponent::GetVolumeRange() const
    {
		return std::make_pair(MinSound, MaxSound);
    }

    void SoundComponent::SetBGVolume(float nVolume)
	{
		BGVolume = nVolume;
		const size_t numOfBG = bg_sounds.size();

		for(size_t i = 0; i < numOfBG; ++i)
		{
			FMOD_Channel_SetVolume(BGChannel[i], BGVolume);
		}
	}

	void SoundComponent::SetEffectVolume(float nVolume)
	{
		EffectVolume = nVolume;
		FMOD_Channel_SetVolume(*EffectLoopChannel, EffectVolume);
		FMOD_Channel_SetVolume(*EffectChannel, EffectVolume);
	}

	void SoundComponent::EffectSoundVolumeUp()
	{
		if (EffectVolume < MaxSound)
		{
			EffectVolume += Soundvalue;
		}

		FMOD_Channel_SetVolume(*EffectLoopChannel, EffectVolume);

	}
	void SoundComponent::EffectSoundVolumeDown()
	{
		if (EffectVolume > MinSound)
		{
			EffectVolume -= Soundvalue;
		}
		FMOD_Channel_SetVolume(*EffectLoopChannel, EffectVolume);

	}
	
	void SoundComponent::PauseSoundBG(int nIndex)
	{
		if (nIndex < BGCount && isplaying_bg[nIndex] == static_cast<FMOD_BOOL>(true))
		{
			FMOD_Channel_SetPaused(BGChannel[nIndex], isplaying_bg[nIndex]);
			isplaying_bg[nIndex] = false;
		}
		else if (nIndex >= BGCount || nIndex < 0)
		{
			throw std::invalid_argument("Invalid Index value");
		}
	}

	void SoundComponent::PauseAll()
	{
 		FMOD_ChannelGroup_SetPaused(ChannelGroup,true);
		//OffAllSound();
	}

	void SoundComponent::ResumeAll()
	{
		FMOD_ChannelGroup_SetPaused(ChannelGroup, false);
		//OffAllSound();
	}

	bool SoundComponent::ResumeSoundBG(int nIndex)
	{
		if (nIndex < BGCount && isplaying_bg[nIndex] == static_cast<FMOD_BOOL>(false))
		{
			FMOD_Channel_SetPaused(BGChannel[nIndex], isplaying_bg[nIndex]);
			isplaying_bg[nIndex] = true;
		}
		else if (nIndex >= BGCount || nIndex < 0)
		{
			throw std::invalid_argument("Invalid Index value");
		}

		return isplaying_bg[nIndex];
	}

	void SoundComponent::ResumeSoundBG(std::string sound_path)
	{
		ResumeSoundBG(std::distance(bg_sounds.begin(), std::find(bg_sounds.begin(), bg_sounds.end(), get_hash_id(sound_path))));
	}

	bool SoundComponent::IsPlaying()
	{
		return isplaying_bg;
	}

	void SoundComponent::PlayBG0()
	{
		PlaySoundBG(0);
	}
	void SoundComponent::StopBG0()
	{
		StopSoundBG(0);
	}
	void SoundComponent::PlayEffect0()
	{
		PlaySoundEffect(0,false);
	}

	void SoundComponent::OffAllSound()
	{
		for (int i = 0; i < BGCount; ++i)
		{
			isplaying_bg[i] = false;
		}
		for (int i = 0; i < LoopEffectCount; ++i)
		{
			isplaying_effect[i] = false;
		}
	}

	bool SoundComponent::CheckOnSound()
	{
		for (int i = 0; i < BGCount; ++i)
		{
			if(isplaying_bg[i])
			{
				return true;
			}
		}
		for (int i = 0; i < LoopEffectCount; ++i)
		{
			if (isplaying_effect[i])
			{
				return true;
			}
		}
		return false;
	}

}


