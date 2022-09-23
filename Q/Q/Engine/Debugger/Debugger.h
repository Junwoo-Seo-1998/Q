/*
    File Name: Debugger.h
    Project Name: Q
    Author(s):
        Primary: Yoonki Kim
        Secondary:
    All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#pragma once
#include "../ImGui/ImGuiHelper.h"
#include "../Debugger/FPS_Debugger.h"
#include "../Debugger/Logger.h"
#include "../Debugger/EntityPhysicsDebugger.h"
namespace  q_engine
{
    class Debugger : public ImGuiRenderable
    {
    public:
        Debugger()
        {
            //ent_debugger.Init();
        }
        ~Debugger()
        {
            logger_.CleanUp();
        }
        void CleanUp()
        {
            GetLogger().CleanUp();
        }
        virtual void ImGuiRender(bool& is_open) override;
        static Logger& GetLogger() { return logger_; }

        
    private:
        static Logger logger_;
        static FPS_Debugger FPS_status_;
        
#ifndef QEDITOR
    public:
        static EntityDebuggerManager& GetEntityDebuggerManager() { return ent_debugger; }
    private:
        static EntityDebuggerManager ent_debugger;
#endif
    };

    static Debugger debugger;
}

