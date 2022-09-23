/*
    File Name: TextComponent.h
    Project Name: Q
    Author(s):
        Primary: Joonho Hwang
        Secondary:
    All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#pragma once
#include "../ECS/Component.h"

#include "../Graphics/TextObject.h"


namespace q_engine
{
	class TextComponent final : public Component<TextComponent>
	{
	public:
		TextComponent();
		TextComponent(FontID fontID, std::string text, ETextHorizontalAlign align = ETextHorizontalAlign::CENTER);
		TextComponent(FontID fontID, std::string text, float fontSize, Color color, ETextHorizontalAlign align = ETextHorizontalAlign::CENTER);

		constexpr void SetFont(FontID fontID) noexcept { mTextObject.SetFont(fontID); }
		void SetText(std::string text) { mTextObject.SetText(std::move(text)); }

		constexpr void SetFontSize(float fontSize) noexcept { mTextObject.SetFontSize(fontSize); }
		constexpr void SetColor(Color color) noexcept { mTextObject.SetColor(color); }

		constexpr void SetHorizontalAlign(ETextHorizontalAlign align) { mTextObject.SetHorizontalAlign(align); }
		constexpr void SetVerticalAlign(ETextVerticalAlign align) { mTextObject.SetVerticalAlign(align); }
		constexpr void SetLineWrapMode(ELineWrapMode mode) { mTextObject.SetLineWrapMode(mode); }

		constexpr const TextObject& GetTextObject() const noexcept { return mTextObject; }
		constexpr TextObject& GetTextObject() noexcept { return mTextObject; }


	private:
		TextObject mTextObject;
	};
}
