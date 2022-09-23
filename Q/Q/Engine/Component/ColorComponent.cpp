/*
    File Name: ColorComponent.cpp
    Project Name: Q
    Author(s):
        Primary: Yoonki Kim
        Secondary:
    All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#include "ColorComponent.h"

namespace  q_engine
{
    bool ColorComponent::IsTimerEnded()
    {
        if (m_target_t < m_elapsed_t)
        {
            is_changing = false;
            m_target_t = 0.f;
            m_elapsed_t = 0.f;
            return true;
        }
        return false;
    }

    Color ColorComponent::GetComplementaryColor() const
    {
        const Color white(1.f, 1.f, 1.f, 1.f);
        return Color(white.r - m_color.r, white.g - m_color.g, white.b - m_color.b, m_color.a);
    }
}

