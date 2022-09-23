/*
	File Name: Font.cpp
	Project Name: Q
	Author(s):
		Primary: Joonho Hwang
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#include "Font.h"

#include <string>

#include <GL/glew.h>

#include <ft2build.h>

#include "../Debugger/Debugger.h"

#include FT_FREETYPE_H


static FT_Library freetype_lib;

namespace q_engine
{
	void Font::Initialize()
	{
		if (FT_Init_FreeType(&freetype_lib))
		{
			Debugger::GetLogger().AddMessage("Failed to initialize the freetype library.", SeverityLevel::ERROR);
			std::exit(EXIT_FAILURE);
		}
	}


	void Font::CleanUp()
	{
		FT_Done_FreeType(freetype_lib);
	}


	Font::Font(const std::filesystem::path& fontFilePath)
	{
		FT_Face fontFace;
		const std::string filePath = fontFilePath.string();
		if (FT_New_Face(freetype_lib, filePath.c_str(), 0, &fontFace))
		{
			Debugger::GetLogger().AddMessage("Failed to create a new font face with file \"" + fontFilePath.string() + "\"", SeverityLevel::ERROR);
#ifdef _DEBUG
			std::exit(EXIT_FAILURE);
#else
			return;
#endif
		}

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		for (unsigned int baseFontSize : BASE_FONT_SIZES)
		{
			FT_Set_Pixel_Sizes(fontFace, 0, baseFontSize);

			mFontHeights[baseFontSize] = static_cast<float>(fontFace->size->metrics.height >> 6);

			if (FT_HAS_KERNING(fontFace))
			{
				FT_Vector kerning;

				for (CharacterType prevCharacter = 0; prevCharacter <= MAX_CHARACTER; ++prevCharacter)
				{
					for (CharacterType nextCharacter = 0; nextCharacter <= MAX_CHARACTER; ++nextCharacter)
					{
						if (!FT_Get_Kerning(fontFace, prevCharacter, nextCharacter, FT_KERNING_DEFAULT, &kerning))
						{
							mKerning[baseFontSize][prevCharacter][nextCharacter] = static_cast<float>(kerning.x >> 6);
						}
					}
				}
			}

			TextureHandle textureHandles[MAX_CHARACTER + 1];
			glCreateTextures(GL_TEXTURE_2D, MAX_CHARACTER + 1, textureHandles);

			for (CharacterType c = 0; c <= MAX_CHARACTER; ++c)
			{
				const TextureHandle textureHandle = textureHandles[c];

				if (FT_Load_Char(fontFace, c, FT_LOAD_RENDER))
				{
					Debugger::GetLogger().AddMessage("Failed to load a character '" + std::to_string(c) + "' from the font face \"" + fontFilePath.string() + "\"", SeverityLevel::WARNING);
					continue;
				}

				const auto& glyph = fontFace->glyph;
				const auto& bitmap = glyph->bitmap;
				const auto width = bitmap.width;
				const auto height = bitmap.rows;


				if (width > 0 && height > 0)
				{
					glTextureStorage2D(textureHandle, 1, GL_R8, width, height);
					glTextureSubImage2D(textureHandle, 0, 0, 0, width, height, GL_RED, GL_UNSIGNED_BYTE, bitmap.buffer);

					glTextureParameteri(textureHandle, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
					glTextureParameteri(textureHandle, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
					glTextureParameteri(textureHandle, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
					glTextureParameteri(textureHandle, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				}

				mGlyphInfos[c][baseFontSize] = { textureHandle, {width, height}, {glyph->bitmap_left, glyph->bitmap_top}, glyph->advance.x };
			}
		}

		FT_Done_Face(fontFace);
	}


	Font::~Font()
	{
		for (const auto& [_, glyphInfos] : mGlyphInfos)
		{
			for (const auto& [_, glyphInfo] : glyphInfos)
			{
				glDeleteTextures(1, &glyphInfo.textureHandle);
			}
		}
	}


	unsigned int Font::GetClosestFontSize(const float desiredFontSize)
	{
		const auto closest = std::lower_bound(std::begin(BASE_FONT_SIZES), std::end(BASE_FONT_SIZES), desiredFontSize);
		if (closest == std::end(BASE_FONT_SIZES))
		{
			return BASE_FONT_SIZES[sizeof(BASE_FONT_SIZES) / sizeof(BASE_FONT_SIZES[0]) - 1];
		}

		return *closest;
	}


	Font::GlyphInfo Font::GetGlyphInfo(const CharacterType character, const unsigned int fontSize) const
	{
		return mGlyphInfos.at(character).at(fontSize);
	}


	float Font::GetKerning(const unsigned int fontSize, const CharacterType prev, const CharacterType next) const
	{
		if (mKerning.count(fontSize) && mKerning.at(fontSize).count(prev) && mKerning.at(fontSize).at(prev).count(next))
		{
			return mKerning.at(fontSize).at(prev).at(next);
		}

		return 0;
	}


	float Font::GetFontHeight(const unsigned int fontSize) const
	{
		return mFontHeights.at(fontSize);
	}
}
