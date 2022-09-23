/*
    File Name: Texture.cpp
    Project Name: Q
    Author(s):
        Primary: Joonho Hwang
        Secondary:
    All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#include "Texture.h"

#include <GL/glew.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "../Debugger/Debugger.h"


namespace q_engine
{
	Texture::Texture(const std::filesystem::path& imageFilePath, const int texelX, const int texelY, const int texelWidth, const int texelHeight)
	{
        int width, height, componentCount;
        stbi_set_flip_vertically_on_load(true);
        unsigned char* const data = stbi_load(imageFilePath.string().c_str(), &width, &height, &componentCount, 0);
        if (data == nullptr)
        {
            Debugger::GetLogger().AddMessage("Failed to load file " + imageFilePath.string(), SeverityLevel::ERROR);
        #ifdef _DEBUG
            std::exit(EXIT_FAILURE);
		#else=
			return;
		#endif
        }
		
        TextureHandle textureHandle;
        glCreateTextures(GL_TEXTURE_2D, 1, &textureHandle);

        GLenum sizedInternalFormat;
        GLenum baseInternalFormat;
		switch (componentCount)
		{
            case 3:
                sizedInternalFormat = GL_RGB8;
                baseInternalFormat = GL_RGB;
                break;

            case 4:
                sizedInternalFormat = GL_RGBA8;
                baseInternalFormat = GL_RGBA;
                break;

            default:
                Debugger::GetLogger().AddMessage("Unsupported file format of file " + imageFilePath.string(), SeverityLevel::ERROR);
            #ifdef _DEBUG
				std::exit(EXIT_FAILURE);
			#else
				return;
			#endif
		}
		
        glTextureStorage2D(textureHandle, 1, sizedInternalFormat, texelWidth > 0 ? texelWidth : width, texelHeight > 0 ? texelHeight : height);
        glTextureSubImage2D(textureHandle, 0, texelX, texelY, texelWidth > 0 ? texelWidth : width, texelHeight > 0 ? texelHeight : height, baseInternalFormat, GL_UNSIGNED_BYTE, data);

        stbi_image_free(data);

        glTextureParameteri(textureHandle, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTextureParameteri(textureHandle, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTextureParameteri(textureHandle, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTextureParameteri(textureHandle, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        mWidth = width;
        mHeight = height;
        mTextureHandle = textureHandle;
        mSizedInternalFormat = sizedInternalFormat;
        mBaseInternalFormat = baseInternalFormat;
	}


	Texture::Texture(const TextureHandle sourceTextureHandle, const GLenum sizedInternalFormat, const GLenum baseInternalFormat, const int texelX, const int texelY, const int texelWidth, const int texelHeight)
		: mWidth(texelWidth), mHeight(texelHeight), mSizedInternalFormat(sizedInternalFormat), mBaseInternalFormat(baseInternalFormat)
	{
        TextureHandle textureHandle;
        glCreateTextures(GL_TEXTURE_2D, 1, &textureHandle);

        const size_t bufSize = texelWidth * texelHeight * (baseInternalFormat == GL_RGB ? 3 : 4);
        unsigned char* data = new unsigned char[bufSize];

        glGetTextureSubImage(sourceTextureHandle, 0, texelX, texelY, 0, texelWidth, texelHeight, 1, baseInternalFormat, GL_UNSIGNED_BYTE, static_cast<GLsizei>(bufSize * sizeof(unsigned char)), data);
		
        glTextureStorage2D(textureHandle, 1, sizedInternalFormat, texelWidth, texelHeight);
        glTextureSubImage2D(textureHandle, 0, 0, 0, texelWidth, texelHeight, baseInternalFormat, GL_UNSIGNED_BYTE, data);

        delete[] data;
		
        glTextureParameteri(textureHandle, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTextureParameteri(textureHandle, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTextureParameteri(textureHandle, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTextureParameteri(textureHandle, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		
        mTextureHandle = textureHandle;
	}


	Texture::~Texture()
	{
        glDeleteTextures(1, &mTextureHandle);
	}
} // namespace q_engine
