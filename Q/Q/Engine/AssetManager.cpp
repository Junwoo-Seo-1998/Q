/*
    File Name: AssetManager.cpp
    Project Name: Q
    Author(s):
        Primary: Joonho Hwang
        Secondary:
    All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#include "AssetManager.h"

#include <fstream>
#include <sstream>

#include "Debugger/Debugger.h"


namespace q_engine
{
	void AssetManager::Initialize(const std::filesystem::path& resourceListFilePath)
	{
		cacheResourceFilePath = resourceListFilePath;
		
		std::ifstream resourceListFile{ resourceListFilePath };

		std::string line;
		std::string resourceType;
		std::string resourceName;
		EShaderType shaderType;
		std::filesystem::path filePath;
		while (std::getline(resourceListFile, line))
		{
			if (line.empty() || line.front() == '#')
			{
				continue;
			}

			std::stringstream sstr{ line };

			sstr >> resourceType;

			switch (get_hash_id(resourceType))
			{
				case "mesh"_hashID:
					sstr >> resourceName >> filePath;
					TryRegisterMesh(resourceName, filePath);
					break;

				case "shader"_hashID:
					sstr >> resourceName >> shaderType >> filePath;
					TryRegisterShader(resourceName, shaderType, filePath);
					break;

				case "shaderprogram"_hashID:
				{
					sstr >> resourceName;
					const ShaderProgramID shaderProgramID = TryRegisterShaderProgram(resourceName);
					ShaderProgram& shaderProgram = GetShaderProgram(shaderProgramID);
					while (sstr >> line)  // reusing the string
					{
						shaderProgram.AttachLinkValidate(GetShader(line));
					}

					break;
				}

				case "texture"_hashID:
					sstr >> resourceName >> filePath;
					TryRegisterTexture(resourceName, filePath);
					break;

				case "animation"_hashID:
					sstr >> resourceName >> filePath;
					TryRegisterAnimation(resourceName, filePath);
					break;

				case "font"_hashID:
					sstr >> resourceName >> filePath;
					TryRegisterFont(resourceName, filePath);
					break;

				default:
					Debugger::GetLogger().AddMessage("Skipping unknown Resource Type \"" + resourceType + "\" while loading resources.", SeverityLevel::WARNING);
					break;
			}
		}
	}


	void AssetManager::Reload(const std::filesystem::path& resourceListFilePath)
	{
		meshes.clear();
		shaders.clear();
		shadersPrograms.clear();
		textures.clear();
		animations.clear();
		fonts.clear();

		Initialize(resourceListFilePath.empty() ? cacheResourceFilePath : resourceListFilePath);
	}


	Mesh& AssetManager::GetMesh(const std::string& meshName)
	{
		return GetMesh(get_hash_id(meshName));
	}


	Shader& AssetManager::GetShader(const std::string& shaderName)
	{
		return GetShader(get_hash_id(shaderName));
	}


	ShaderProgram& AssetManager::GetShaderProgram(const std::string& shaderProgramName)
	{
		return GetShaderProgram(get_hash_id(shaderProgramName));
	}


	Texture& AssetManager::GetTexture(const std::string& textureName)
	{
		return GetTexture(get_hash_id(textureName));
	}

	std::unordered_map<TextureID, Texture>& AssetManager::GetTextures()
	{
		return textures;
	}


	Animation& AssetManager::GetAnimation(const std::string& animationName)
	{
		return GetAnimation(get_hash_id(animationName));
	}

	std::unordered_map<AnimationID, Animation>& AssetManager::GetAnimations()
	{
		return animations;
	}


	Font& AssetManager::GetFont(const std::string& fontName)
	{
		return GetFont(get_hash_id(fontName));
	}



	Mesh& AssetManager::GetMesh(const MeshID meshID)
	{
		return meshes.at(meshID);
	}


	Shader& AssetManager::GetShader(const ShaderID shaderID)
	{
		return shaders.at(shaderID);
	}


	ShaderProgram& AssetManager::GetShaderProgram(const ShaderProgramID shaderProgramID)
	{
		return shadersPrograms.at(shaderProgramID);
	}


	Texture& AssetManager::GetTexture(const TextureID textureID)
	{
		return textures.at(textureID);
	}


	Animation& AssetManager::GetAnimation(const AnimationID animationID)
	{
		return animations.at(animationID);
	}


	Font& AssetManager::GetFont(const FontID fontID)
	{
		return fonts.at(fontID);
	}

}
