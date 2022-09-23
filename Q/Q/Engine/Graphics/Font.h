/*
    File Name: Font.h
    Project Name: Q
    Author(s):
        Primary: Joonho Hwang
        Secondary:
    All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#pragma once
#include <filesystem>
#include <map>
#include <unordered_map>

#include "Engine/Types.h"


namespace q_engine
{
    // Will not use the Texture class in order to increase the speed

    using CharacterType = unsigned char;
    class [[nodiscard]] Font
    {
        using TextureHandle = unsigned;
    	
        friend class AssetManager;
    public:
        struct [[nodiscard]] GlyphInfo
        {
            TextureHandle textureHandle;
            unsigned int size[2];
            int bearing[2];
            long advance;
        };

        static void Initialize();
        static void CleanUp();
    	
        Font(const std::filesystem::path& fontFilePath);
        ~Font();

        Font(const Font& other) = delete;
        Font(Font&& other) noexcept = default;
        Font& operator=(const Font& other) = delete;
        Font& operator=(Font&& other) noexcept = default;

        [[nodiscard]] static unsigned int GetClosestFontSize(float desiredFontSize);
        GlyphInfo GetGlyphInfo(CharacterType character, unsigned int fontSize) const;
        [[nodiscard]] float GetKerning(unsigned int fontSize, CharacterType prev, CharacterType next) const;
        [[nodiscard]] float GetFontHeight(unsigned int fontSize) const;
    	
        [[nodiscard]] constexpr FontID GetFontID() const noexcept { return mFontID; }
    	

    	
    private:
        inline static constexpr unsigned int BASE_FONT_SIZES[] = { 10, 20, 30, 40, 50, 70, 100, 150, 200, 300 };
        inline static constexpr CharacterType MAX_CHARACTER = 127;

    	
        std::unordered_map<CharacterType, std::map<unsigned int, GlyphInfo>> mGlyphInfos;
        std::map<unsigned int, float> mFontHeights;
        std::map<unsigned int, std::unordered_map<CharacterType, std::unordered_map<CharacterType, float>>> mKerning;
    	
        FontID mFontID = INVALID_ID;
    };
}
