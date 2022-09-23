/*
    File Name: FPS_Debugger.cpp
    Project Name: Q
    Author(s):
        Primary: Yoonki Kim
        Secondary:
    All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#include "FPS_Debugger.h"

#include <numeric>
#include <gsl/gsl_narrow>
#include "../Graphics/Window.h"

namespace q_engine
{
    void FPS_Debugger::ImGuiRender(bool& is_open)
    {
        static bool update = true;
        const float curFPS = gsl::narrow_cast<float>(q_impl::FPS);

        ImGui::Begin("FPS_Status", &is_open, ImGuiWindowFlags_AlwaysAutoResize);

        ImGui::Checkbox("Update", &update);

        if(update)
            PushFPSToContainer(curFPS);

        const float min = *std::min_element(FPSContainer_.begin(), FPSContainer_.end());
        const float max = *std::max_element(FPSContainer_.begin(), FPSContainer_.end());
        const float sum = std::accumulate(FPSContainer_.begin(), FPSContainer_.end(), 0.f);
        const int average = gsl::narrow_cast<int>(sum / FPSContainerSize);

        ImGui::Separator();
        ImGui::Text(("FPS : " + std::to_string(curFPS)).c_str()); ImGui::SameLine();
        ImGui::Text(("/ Average : " + std::to_string(average)).c_str()); ImGui::SameLine();
        ImGui::Text(("/ Min : " + std::to_string(min)).c_str()); ImGui::SameLine();
        ImGui::Text(("/ Max : " + std::to_string(max)).c_str());

        ImGui::Separator();

        ImGui::PlotLines("", FPSContainer_.data(), FPSContainerSize, 
            0, "FPS", 55.f, 65.f, ImVec2(500.f, 100.f));

        ImGui::End();
    }
    void FPS_Debugger::PushFPSToContainer(const double FPS)
    {
        for(unsigned int i = 0; i < FPSContainerSize -1; ++i)
        {
            FPSContainer_[i] = FPSContainer_[i + 1];
        }
        FPSContainer_.back() = gsl::narrow_cast<float>(FPS);
    }
}
