/*
    File Name: Debugger.cpp
    Project Name: Q
    Author(s):
        Primary: Yoonki Kim
        Secondary:
    All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#include "Debugger.h"

namespace q_engine
{
    Logger Debugger::logger_;
    FPS_Debugger Debugger::FPS_status_;
#ifndef QEDITOR
    EntityDebuggerManager Debugger::ent_debugger;
#endif

    void Debugger::ImGuiRender(bool& is_open)
    {
        static bool show_FPS_debugger = false;
        static bool show_Logger = false;
        static bool show_entity_debugger = false;

        ImGui::BeginMainMenuBar();

        if (ImGui::BeginMenu("Debugging"))
        {
            ImGui::MenuItem("Logger", "", &show_Logger);
            ImGui::MenuItem("FPS_Status", "", &show_FPS_debugger);
            ImGui::MenuItem("Entity_Debugger", "", &show_entity_debugger);
            ImGui::EndMenu();
        }

        if (show_FPS_debugger)
            FPS_status_.ImGuiRender(show_FPS_debugger);
        if (show_Logger)
            logger_.ImGuiRender(show_Logger);
#ifndef QEDITOR
        if (show_entity_debugger)
            ent_debugger.ImGuiRender(show_entity_debugger);
#endif
        ImGui::EndMainMenuBar();
    }
}
