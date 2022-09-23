/*
    File Name: Texture.h
    Project Name: Q
    Author(s):
        Primary: Joonho Hwang
        Secondary:
    All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#pragma once
#include <filesystem>

#include "Engine/Types.h"


namespace q_engine
{

	class [[nodiscard]] Texture
	{
        using TextureHandle = unsigned;
        using GLenum = unsigned;

        friend class AssetManager;
	public:
        Texture(const std::filesystem::path& imageFilePath, int texelX = 0, int texelY = 0, int texelWidth = -1, int texelHeight = -1);
        Texture(TextureHandle sourceTextureHandle, GLenum sizedInternalFormat, GLenum baseInternalFormat, int texelX, int texelY, int texelWidth, int texelHeight);
        ~Texture();

        Texture(const Texture&) = delete;
        Texture(Texture&& other) noexcept = default;
        Texture& operator=(const Texture&) = delete;
        Texture& operator=(Texture&& other) noexcept = default;
        [[nodiscard]] std::string GetTextureName() const { return mTextureName; }
        [[nodiscard]] constexpr int GetWidth() const noexcept { return mWidth; }
        [[nodiscard]] constexpr int GetHeight() const noexcept { return mHeight; }
        [[nodiscard]] constexpr TextureHandle GetTextureHandle() const noexcept { return mTextureHandle; }
        [[nodiscard]] constexpr GLenum GetSizedInternalFormat() const noexcept { return mSizedInternalFormat; }
        [[nodiscard]] constexpr GLenum GetBaseInternalFormat() const noexcept { return mBaseInternalFormat; }
		
        [[nodiscard]] constexpr TextureID GetTextureID() const noexcept { return mTextureID; }
		


	private:
        TextureHandle mTextureHandle;
        int mWidth;
        int mHeight;

        GLenum mSizedInternalFormat;
        GLenum mBaseInternalFormat;

        std::string mTextureName = "";
        TextureID mTextureID = INVALID_ID;
		
	};

} // namespace q_engine
