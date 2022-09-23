/*
	File Name: Engine.h
	Project Name: Q
	Author(s):
		Primary: Minjae Kyung
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/

#pragma once
#include <unordered_map>
#include <chrono>
#include <glm/vec2.hpp>
#include <memory>


#include "EngineComponent.h"
#include "Engine/Types.h"
#include "RandomMap/MapGeneration.h"
#include "../Engine/Graphics/Framebuffer.h"

//let's reduce build time...
class PostProcessor;

namespace q_engine
{
	
	class Engine
	{
	private:
		static std::unordered_map<int, std::shared_ptr<IEngineComponent>> engineComponents;

		static bool isGameEnded;
		static IDType LastEngineCompID;
		static double elapsedTime;

		static std::chrono::time_point<std::chrono::high_resolution_clock> pastTime;
		static std::chrono::time_point<std::chrono::high_resolution_clock> nowTime;
		static double deltaTime;
		static std::shared_ptr<Framebuffer> mFrameBuffer;
		static std::shared_ptr<PostProcessor> mPostProcessor;
		static bool IsFullScreen;
		template <typename T>
		static void AddEngineComponent();
		static bool IsFocused;
		static bool IsEngineComponentsLoaded;
		static bool IsUpdateBlocked;
	public:
		static bool IsInGame();
		static glm::vec2 GetWindowSize();
		static double GetElapsedTime()
		{
			return elapsedTime;
		}
		static void Init();
		static void Load();
		static void Update();
		static void Unload();
		static void CleanUp();
		static void DrawAllResolution();

		static void SetWindowFocus(bool focus);

		static void ToggleFullScreen();

		static bool IsGameEnded();
		template <typename T>
		static T& GetEngineComponent();
		static std::shared_ptr<PostProcessor> GetPostProcessor();
		static void MakeGameEnded()
		{
			isGameEnded = true;
		}

		struct SlowMotion
		{
			double remaining_time = 0.f;
			double time_ratio = 1.f;
		}static inline slow_motion;

		static double GetSlowMotionRatio()
		{
			return slow_motion.remaining_time > 0.f ? slow_motion.time_ratio : 1.f;
		}
		
		static void ApplySlowMotion(double time, double ratio)
		{
			if(slow_motion.remaining_time > 0)
			{
				slow_motion.remaining_time += time;
				slow_motion.time_ratio = std::min(ratio, slow_motion.time_ratio);
				return;
			}
			slow_motion.remaining_time = time;
			slow_motion.time_ratio = ratio;
		}

		static void StopSlowMotion()
		{
			slow_motion.remaining_time = 0.f;
			//slow_motion.time_ratio = ratio;
		}

		static bool GetIsFullScreen()
		{
			return IsFullScreen;
		}
	};
}

#include "Engine.inl"
