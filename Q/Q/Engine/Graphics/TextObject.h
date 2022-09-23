/*
    File Name: TextObject.h
    Project Name: Q
    Author(s):
        Primary: Joonho Hwang
        Secondary:
    All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#pragma once
#include <glm/vec2.hpp>
#include <string>

#include "Color.h"
#include "GraphicsMisc.h"
#include "Engine/Types.h"

namespace q_engine
{
	enum class ETextHorizontalAlign : char
	{
		CENTER,
		LEFT,
		RIGHT
	};

	enum class ETextVerticalAlign : char
	{
		CENTER,
		TOP,
		BOTTOM
	};

	enum class ELineWrapMode : char
	{
		BY_WHITESPACE,
		// Whitespaces + Symbols
		BY_WORD,
		BREAK_WORD
	};



	class [[nodiscard]] TextObject
	{
	public:
		TextObject(FontID fontID, std::string text, ZDepth zDepth = 0);
		TextObject(FontID fontID, std::string text, float fontSize, Color color, ZDepth zDepth = 0);

		
		constexpr void SetFont(FontID fontID) noexcept { mFontID = fontID; }
		void SetText(std::string text) { mText = std::move(text); }
		
		constexpr void SetFontSize(float fontSize) noexcept { mFontSize = fontSize; }
		constexpr void SetColor(Color color) noexcept { mColor = color; }

		constexpr void SetHorizontalAlign(ETextHorizontalAlign align) { mHorizontalAlign = align; }
		constexpr void SetVerticalAlign(ETextVerticalAlign align) { mVerticalAlign = align; }
		constexpr void SetLineWrapMode(ELineWrapMode mode) { mLineWrapMode = mode; }
		
		
		void Draw(glm::vec2 translation, float rotation = 0.f, glm::vec2 scale = { 1.0, 1.0 }) const;
		


	private:
		FontID mFontID;
		std::string mText;

		float mFontSize = 30;
		Color mColor = {255, 255, 255};

		ZDepth mZDepth;

		ETextHorizontalAlign mHorizontalAlign = ETextHorizontalAlign::CENTER;
		ETextVerticalAlign mVerticalAlign = ETextVerticalAlign::CENTER;
		ELineWrapMode mLineWrapMode = ELineWrapMode::BY_WORD;
	};
}
