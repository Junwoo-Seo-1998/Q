/*
    File Name: TextComponent.cpp
    Project Name: Q
    Author(s):
        Primary: Joonho Hwang
        Secondary:
    All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#include "TextComponent.h"


#include "../../Game/EObjectZDepth.h"


namespace q_engine
{
	TextComponent::TextComponent()
		: mTextObject(INVALID_ID, "", static_cast<int>(EObjectZDepth::TEXT))
	{}


	TextComponent::TextComponent(const FontID fontID, std::string text, ETextHorizontalAlign align)
        : mTextObject(fontID, std::move(text), static_cast<int>(EObjectZDepth::TEXT))
	{
		SetHorizontalAlign(align);
	}


	TextComponent::TextComponent(const FontID fontID, std::string text, const float fontSize, const Color color, ETextHorizontalAlign align)
		: mTextObject(fontID, std::move(text), fontSize, color, static_cast<int>(EObjectZDepth::TEXT))
	{
		SetHorizontalAlign(align);
	}
}
