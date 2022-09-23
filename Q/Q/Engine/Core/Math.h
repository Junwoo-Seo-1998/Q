/*
    File Name: Math.h
    Project Name: Q
    Author(s):
        Primary: Joonho Hwang
        Secondary:
    All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#pragma once
#include <glm/glm.hpp>


namespace q_engine
{
    [[nodiscard]] constexpr float smooth_step(float start, float end, float alpha) noexcept
    {
        alpha = glm::clamp(alpha, 0.f, 1.f);

        return start + (end - start) * (alpha * alpha * alpha * (alpha * (6 * alpha - 15) + 10));
    }

	template<glm::length_t Length>
    [[nodiscard]] constexpr glm::vec<Length, float> smooth_step(glm::vec<Length, float> start, glm::vec<Length, float> end, float alpha) noexcept
    {
        glm::vec<Length, float> toReturn;
    	
	    for (glm::length_t i = 0; i < Length; ++i)
	    {
            toReturn[i] = smooth_step(start[i], end[i], alpha);
	    }

        return toReturn;
    }
}
