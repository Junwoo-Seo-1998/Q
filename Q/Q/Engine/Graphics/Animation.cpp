/*
    File Name: Animation.cpp
    Project Name: Q
    Author(s):
        Primary: Joonho Hwang
        Secondary:
    All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#include "Animation.h"

#include <fstream>
#include <sstream>
#include <string>

#include "../AssetManager.h"


namespace q_engine
{
	Animation::Animation(const std::filesystem::path& animationFilePath)
	{
        cacheAccumulatedTimePoints.emplace_back(0.f);
		
        bool bUseTexel;
		
        std::ifstream animationFile{ animationFilePath };

		if(!animationFile.is_open())
		{
			throw std::runtime_error("No directory in file path : animation");
		}
        animationFile >> mInitialPlayRate >> mLoopPoint >> std::boolalpha >> bUseTexel >> std::noboolalpha;

		mInitialIsLooping = mLoopPoint >= 0 ? true : false;

        std::string line;
        std::string textureName;
        float playTime;
		int texelX, texelY, texelWidth, texelHeight;
		while (std::getline(animationFile, line))
		{
			if (line.empty())
			{
				continue;
			}
			
            std::stringstream sstr{ line };
			 
			if (bUseTexel)
			{
				sstr >> textureName >> texelX >> texelY >> texelWidth >> texelHeight >> playTime;
				const TextureID textureID = get_hash_id(textureName);
				const Texture& original = AssetManager::GetTexture(textureID);
				const TextureID newTextureID = AssetManager::TryRegisterTexture("OriginalTexelModified" + std::to_string(textureID) + std::to_string(texelX) + std::to_string(texelY) + std::to_string(texelWidth) + std::to_string(texelHeight),
					original.GetTextureHandle(), original.GetSizedInternalFormat(), original.GetBaseInternalFormat(),
					texelX, texelY, texelWidth, texelHeight);
				mTextureIDs.emplace_back(newTextureID);
			}
			else
			{
				sstr >> textureName >> playTime;
				mTextureIDs.emplace_back(get_hash_id(textureName));
			}

            cacheAccumulatedTimePoints.emplace_back(playTime + cacheAccumulatedTimePoints.back());
		}
	}
}
