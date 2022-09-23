/*
    File Name: GraphicsMisc.h
    Project Name: Q
    Author(s):
        Primary: Joonho Hwang
        Secondary:
    All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#pragma once
#include <stdexcept>
#include <string>
#include "sol/sol.hpp"

namespace q_engine
{
    // Lower = Closer to the screen
    // If two or more objects' depth values are the same, the first one called to be drawn will be drawn
    struct [[nodiscard]] ZDepth
    {
        ZDepth() = default;
    	
        template<typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
        ZDepth(T value);
    	//expose to lua
        ZDepth(float value);

        constexpr operator float() const { return value; }


        static constexpr int MAX_VALUE = 2796202;
        float value;
    };



    struct [[nodiscard]] LineOrPointSize
    {
        LineOrPointSize(float value = 0.f);

        constexpr operator float() const { return value; }


        float value;
    };



	enum class [[nodiscard]] EFrameOfReference
	{
        LEFT_BOTTOM = 0,
        BOTTOM_LEFT = 0,

        LEFT_TOP = 1,
        TOP_LEFT = 1,

        CENTER_BOTTOM = 2,
        BOTTOM_CENTER = 2,
		
		CENTER = 3,
		MIDDLE_CENTER = 3,
		CENTER_MIDDLE = 3,


		UNCHANGED
	};




	
    template <typename T, typename>
    ZDepth::ZDepth(T value)
		: value((static_cast<float>(value) + static_cast<float>(MAX_VALUE)) / static_cast<float>(2 * MAX_VALUE))
    {
        if (value > MAX_VALUE || value < -MAX_VALUE)
        {
            throw std::runtime_error("zDepth's value should be between " + std::to_string(-MAX_VALUE) +
                " and " + std::to_string(MAX_VALUE) + " (inclusive), got " + std::to_string(value));
        }
    }



    inline LineOrPointSize::LineOrPointSize(const float value)
        : value(value)
    {
    	if (value <= 0)
    	{
            throw std::runtime_error("line or point size should be greater than 0, got " + std::to_string(value));
    	}
    }


    void RegisterZDepthType(sol::state& state);


    void RegisterEFrameOfReferenceEnum(sol::state& state);
}
