/*
	File Name: Engine.cpp
	Project Name: Q
	Author(s):
		Primary: Minjae Kyung
		Secondary:Junwoo Seo
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/


#include <chrono>

#include "Engine.h"


#include <iostream>
#include <GL/glew.h>

#include "AssetManager.h"
#include "Core/AStarPathFinding.h"
#include "Debugger/Debugger.h"
#include "ECS/InputManager.h"
#include "Core/EventManager.h"
#include "Core/RandomMachine.h"
#include "Graphics/Font.h"
#include "Scripting/ScriptManager.h"
#include "Engine/Scripting/ScriptManager.h"
#include "Core/ThreadPool.h"
#include "Graphics/PostProcessor.h"

#ifndef QEDITOR // for engine.
#include "GameStateManager.h"
#include "Core/BezierInterpolation.h"
#include "Component/SoundComponent.h"
#include "Core/RandomMachine.h"
#endif

#ifdef QEDITOR// for editor.
#include "Editor/EngineComponent/EditorComponent.h"
#endif

namespace q_engine
{
	std::unordered_map<int, std::shared_ptr<IEngineComponent>> Engine::engineComponents;
	double Engine::deltaTime = 0;
	std::chrono::time_point<std::chrono::high_resolution_clock> Engine::pastTime;
	std::chrono::time_point<std::chrono::high_resolution_clock> Engine::nowTime;

	bool Engine::isGameEnded{ false };
	bool Engine::IsFullScreen{ true };
	IDType Engine::LastEngineCompID{ 0 };
	double Engine::elapsedTime = 0.f;
	std::shared_ptr<Framebuffer> Engine::mFrameBuffer;
	std::shared_ptr<PostProcessor> Engine::mPostProcessor;
	bool Engine::IsFocused = true;
	bool Engine::IsUpdateBlocked = false;
	bool Engine::IsEngineComponentsLoaded = false;

	static inline glm::vec2 targetViewPort;
	constexpr static glm::vec2 renderingResolution = { 1600, 900 };

	std::function<void()> SwapWindowFunc = nullptr;
	
	bool Engine::IsInGame()
	{
		return IsEngineComponentsLoaded ? GetEngineComponent<GameStateManager>().HasState(GameStateType::GAME) : false;
	}

	glm::vec2 Engine::GetWindowSize()
	{
#ifdef QEDITOR
		return GetEngineComponent<EditorComponent>().GetViewportSize();
#else
		const auto winSize = get_window_size;
		return glm::vec2{ static_cast<float>(winSize().first), static_cast<float>(winSize().second) };
#endif
	}
	void Engine::Init()
	{
		//common

		Font::Initialize();
		AssetManager::Initialize("resource/resources.txt");
		MapGeneration::Initialize();
		MapGeneration::Generation();
		

#ifndef QEDITOR // for engine.

		AddEngineComponent<GameStateManager>();
		AddEngineComponent<EventManager>();

#endif
		AddEngineComponent<InputManager>();

#ifndef QEDITOR // for engine.
		    
		AddEngineComponent<SoundComponent>();
		AddEngineComponent<RandomMachine>();
		AddEngineComponent<InterpolationManager>();
		AddEngineComponent<ThreadPool>();
		AddEngineComponent<AStarPathFinding>();

#endif

#ifdef  QEDITOR // for editor.
		AddEngineComponent<EditorComponent>();
#endif

		q_engine::ScriptManager::Get().Init();
		FramebufferSpecification spec;
		spec.Width = 1600; spec.Height = 900;
		spec.Attachments = { FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::DEPTH24STENCIL8 };
		mFrameBuffer = std::make_shared<Framebuffer>(spec);
		mPostProcessor = std::make_shared<PostProcessor>(mFrameBuffer);

		if (IsFullScreen)
		{
			targetViewPort = { q_impl::monitor_size_width, q_impl::monitor_size_height };
		}
		else
		{
			targetViewPort = { renderingResolution.x,  renderingResolution.y };
		}
	}
	
	void Engine::Load()
	{
		for (auto& comp : engineComponents)
		{
			comp.second->Load();
		}
		pastTime = std::chrono::high_resolution_clock::now();
		IsEngineComponentsLoaded = true;
	}

	void Engine::Update()
	{
		namespace time = std::chrono;
		nowTime = time::high_resolution_clock::now();
		const time::duration deltaTick = nowTime - pastTime;
		deltaTime = time::duration<double>(deltaTick).count();

		pastTime = nowTime;
		q_impl::FPS = 1.f / deltaTime;

		if(deltaTime > 0.1)
		{
			deltaTime = 0.033;
		}

		if (IsUpdateBlocked)
		{
			return;
		}

		if (slow_motion.remaining_time > 0)
		{
			slow_motion.remaining_time -= deltaTime;
			deltaTime *= slow_motion.time_ratio;
		}
		mFrameBuffer->Bind();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		for (auto& comp : engineComponents)
		{
			comp.second->Update(deltaTime);
		}
		mFrameBuffer->Unbind();
		mPostProcessor->UpdateTimeCounter(deltaTime);

		if(SwapWindowFunc != nullptr)
		{
 			SwapWindowFunc();
			SwapWindowFunc = nullptr;
		}

		glViewport(0, 0, targetViewPort.x, targetViewPort.y);
		mPostProcessor->UpdateScreenSize(targetViewPort);
		DrawAllResolution();

		
#ifndef QEDITOR
		GetEngineComponent<GameStateManager>().RemoveLater();
#endif
		elapsedTime += deltaTime;
	}

	void Engine::Unload()
	{
		for (auto& comp : engineComponents)
		{
			comp.second->Unload();
		}
	}


	void Engine::CleanUp()
	{
		q_engine::Debugger::GetLogger().AddMessage("Map Generation :" + std::to_string(MapGeneration::GetTimes()) + "times", q_engine::SeverityLevel::INFORMATIONAL);
		debugger.CleanUp();
		Font::CleanUp();
		mPostProcessor->Destroy();
	}

	void Engine::DrawAllResolution()
	{
		mPostProcessor->Draw();
	}

	void Engine::SetWindowFocus(bool focus)
	{
		if(IsFocused == focus || !IsEngineComponentsLoaded)
		{
			return;
		}

		if(IsFocused == true)
		{
		    if(IsEngineComponentsLoaded && IsInGame())
		    {
				if(!(GetEngineComponent<GameStateManager>().HasState(GameStateType::IN_GAME_MENU ) || GetEngineComponent<GameStateManager>().HasState(GameStateType::IN_GAME_MENU2)))
				    GetEngineComponent<GameStateManager>().AddState("InGameMenu"_hashID);
		    }
			else
			{
				GetEngineComponent<SoundComponent>().PauseAll();
				IsUpdateBlocked = true;
			}

		}
		else
		{
			if(IsEngineComponentsLoaded)
			{
			    if(!IsInGame())
			    {
					IsUpdateBlocked = false;

					GetEngineComponent<SoundComponent>().ResumeAll();
			    }

			}

		}
		IsFocused = focus;
	}

	void Engine::ToggleFullScreen()
	{
		static std::function<void(void)> fullScreenFunc = []()
		{
			const glm::vec2 targetSize = { q_impl::monitor_size_width, q_impl::monitor_size_height };
			set_window_size(targetSize.x, targetSize.y);
			set_window_mode(EWindowMode::FULLSCREEN);
			targetViewPort = targetSize;
		};
		static std::function<void(void)> windowScreenFunc = []()
		{
			const glm::vec2 targetSize = renderingResolution;
			set_window_size(targetSize.x, targetSize.y);
			set_window_mode(EWindowMode::WINDOWED);
			targetViewPort = targetSize;
		};

		if (IsFullScreen)	//was fullscreen
		{
			SwapWindowFunc = windowScreenFunc;
		}
		else
		{
			SwapWindowFunc = fullScreenFunc;
		}
		IsFullScreen = !IsFullScreen;
	}


	bool Engine::IsGameEnded()
	{
#ifndef QEDITOR
	    return GetEngineComponent<GameStateManager>().IsGameEnded();
		#else
		return false;
#endif
	}

	std::shared_ptr<PostProcessor> Engine::GetPostProcessor()
	{
		return mPostProcessor;
	}
}

