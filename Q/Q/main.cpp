/*
    File Name: main.cpp
    Project Name: Q
    Author(s):
        Primary: Yoonki Kim / Minjae Kyung / Joonho Hwang
        Secondary:
    All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#include <iostream>

#include "Engine/Graphics/Graphics.h"
#include "Engine/AssetManager.h"

#include "Engine/Debugger/Debugger.h"
#include "Engine/ECS/InputManager.h"
#include "Engine/ImGui/ImGuiHelper.h"
#include "Engine/GameStateManager.h"

#include "Engine/Engine.h"
#include "Engine/Graphics/Window.h"

#include "Engine/Profiler.h"

#define WIN_NAME "Q"

#ifdef _DEBUG

int main(void)
#else
#include "windows.h"
int WinMain(HINSTANCE/* hInstance*/, HINSTANCE/* hPrevInstance*/, LPSTR/* lpCmdLin*/e, int/*	nCmdShow*/)
#endif
{
    using namespace q_engine;
    //try
    {
        create_window(1600, 900, "Q", EWindowMode::FULLSCREEN);


#ifdef _DEBUG
        ImGuiHelper ImGui;
        ImGui.Setup(q_impl::p_window);
#endif
        Engine::Init(); //add engine components
        Engine::Load(); //setup engine components

        Binding* test = nullptr;

        //test for StateManager - y.kimD
        auto& stateMgr = Engine::GetEngineComponent<GameStateManager>();


        //end

        test = new Binding("ReloadLvl");
        test->BindInput(InputType::KEY_DOWN, Key::L_ALT);
        test->BindInput(InputType::KEY_PRESS, Key::R);
        Engine::GetEngineComponent<InputManager>().AddBinding(test);
        Engine::GetEngineComponent<InputManager>().AddCallback("ReloadLvl", &GameStateManager::ReloadState, &stateMgr);

        BEGIN_PROFILING("Update", "Update_Profiling.json");
        while (!is_window_closed() && !Engine::IsGameEnded())
        {
            set_cursor_visible(false);
            clear_background(Color{ 0.f, 0.f,0.f,1.f });
#ifdef _DEBUG
            ImGui.StartRender();
#endif
            Engine::Update();
            update_window();

#ifdef _DEBUG
            static bool on_debug = true;
            debugger.ImGuiRender(on_debug);
            ImGui.EndRender();
#endif
            swap_buffers();
        }
        END_PROFILING();
        Engine::CleanUp();
        Engine::Unload();

#ifdef _DEBUG
        ImGui.CleanUp();
#endif

        return 0;
    }

}