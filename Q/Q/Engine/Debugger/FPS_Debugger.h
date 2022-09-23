/*
    File Name: FPS_Debugger.h
    Project Name: Q
    Author(s):
        Primary: Yoonki Kim
        Secondary:
    All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#pragma once
#include "../ImGui/ImGuiHelper.h"
#include <array>
namespace q_engine
{
    static constexpr int FPSContainerSize = 60 * 5;
    class FPS_Debugger : public ImGuiRenderable
    {
    public:
        FPS_Debugger() = default;
        ~FPS_Debugger() = default;
        virtual void ImGuiRender(bool& is_open) override;

    private:
        void PushFPSToContainer(const double FPS);
        std::array<float, FPSContainerSize> FPSContainer_;
    };
}
