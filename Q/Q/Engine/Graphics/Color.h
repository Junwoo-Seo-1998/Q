/*
    File Name: Color.h
    Project Name: Q
    Author(s):
        Primary: Joonho Hwang
        Secondary:
    All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#pragma once
#include <limits>


namespace q_engine
{
	struct [[nodiscard]] Color
	{
		using HexColor = unsigned int;
		

		template<typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
		constexpr Color(T r, T g, T b, T a = 255) noexcept
			: r(static_cast<float>(r) / 255.f), g(static_cast<float>(g) / 255.f), b(static_cast<float>(b) / 255.f), a(static_cast<float>(a) / 255.f)
		{}

		template<typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
		constexpr Color(T grey, T a = 255) noexcept
			: Color(grey, grey, grey, a)
		{}
		
		explicit constexpr Color(HexColor hexColor) noexcept
			: Color((hexColor >> 6) & 0x000000FF, (hexColor >> 4) & 0x000000FF, (hexColor >> 2) & 0x000000FF, hexColor & 0x000000FF)
		{}

		constexpr Color(float r, float g, float b, float a = 1.f) noexcept
			: r(r), g(g), b(b), a(a)
		{}

		constexpr Color(float grey, float a = 1.f) noexcept
			: Color(grey, grey, grey, a)
		{}


		[[nodiscard]] constexpr bool operator==(const Color& color) const noexcept { return r == color.r && g == color.g && b == color.g && a == color.a; }
		[[nodiscard]] constexpr bool operator!=(const Color& color) const noexcept { return !(*this == color); }
		

		float r, g, b, a;
	};


	constexpr Color INVALID_COLOR{ std::numeric_limits<float>::max(), std::numeric_limits<float>::max() };
} // namespace q_graphics
