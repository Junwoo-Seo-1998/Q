/*
    File Name: ColorComponent.h
    Project Name: Q
    Author(s):
        Primary: Yoonki Kim
        Secondary:
    All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#pragma once
#include "../ECS/Component.h"
#include "../Graphics/Color.h"

namespace  q_engine
{
    class ColorComponent : public Component<ColorComponent>
    {
    public:
        ColorComponent() : ColorComponent(Color{0, 0, 0}) {}
        ColorComponent(Color color) : m_color(color) {}
        ColorComponent(unsigned r, unsigned g, unsigned b, unsigned a = 255) : ColorComponent(Color{ r,g,b,a }) {}

        void SetChangedTo(Color target_color, float changing_time)
        {
            is_changing = true, m_elapsed_t = 0.f, m_target_t = changing_time, m_target_color = target_color;
        }

        [[nodiscard]] bool IsChanging() const { return is_changing; }
        [[nodiscard]] bool IsTimerEnded();
        [[nodiscard]] Color GetTargetColor() const { return m_target_color;  }
        [[nodiscard]] Color GetColor() const { return m_color; }
        void SetColor(Color new_color) { m_color = new_color; }
        Color GetComplementaryColor() const;
    private:
        bool is_changing = false;
        float m_elapsed_t = 0.f;
        float m_target_t = 0.f;
        Color m_target_color{0, 0, 0, 255};
        Color m_color;
    };

}
