/*
    File Name: AssetManager.h
    Project Name: Q
    Author(s):
        Primary: Joonho Hwang
        Secondary:
    All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#pragma once
#include <filesystem>
#include <unordered_map>

#include "Core/StringHash.h"

#include "Debugger/Debugger.h"

#include "Graphics/Animation.h"
#include "Graphics/Font.h"
#include "Graphics/Mesh.h"
#include "Graphics/Shader.h"
#include "Graphics/ShaderProgram.h"
#include "Graphics/Texture.h"
#include "Engine/Types.h"

namespace q_engine
{
	using Script = std::string;

	
	class [[nodiscard]] AssetManager
	{
	public:
        static void Initialize(const std::filesystem::path& resourceListFilePath);
		static void Reload(const std::filesystem::path& resourceListFilePath = "");

		static Mesh& GetMesh(const std::string& meshName);
		static Shader& GetShader(const std::string& shaderName);
		static ShaderProgram& GetShaderProgram(const std::string& shaderProgramName);
		static Texture& GetTexture(const std::string& textureName);
		static std::unordered_map<TextureID, Texture>& GetTextures();
		static Animation& GetAnimation(const std::string& animationName);
		static std::unordered_map<AnimationID, Animation>& GetAnimations();
		static Font& GetFont(const std::string& fontName);
		
		static Mesh& GetMesh(MeshID meshID);
		static Shader& GetShader(ShaderID shaderID);
		static ShaderProgram& GetShaderProgram(ShaderProgramID shaderProgramID);
		static Texture& GetTexture(TextureID textureID);
		static Animation& GetAnimation(AnimationID animationID);
		static Font& GetFont(FontID fontID);

		template<typename ...Args>
		static MeshID TryRegisterMesh(const std::string& meshName, Args&&... args);
		template<typename ...Args>
		static ShaderID TryRegisterShader(const std::string& shaderName, Args&&... args);
		template<typename ...Args>
		static ShaderProgramID TryRegisterShaderProgram(const std::string& shaderProgramName, Args&&... args);
		template<typename ...Args>
		static TextureID TryRegisterTexture(const std::string& textureName, Args&&... args);
		template<typename ...Args>
		static AnimationID TryRegisterAnimation(const std::string& animationName, Args&&... args);
		template<typename ...Args>
		static FontID TryRegisterFont(const std::string& fontName, Args&&... args);


	private:
		inline static std::unordered_map<MeshID, Mesh> meshes;
		inline static std::unordered_map<ShaderID, Shader> shaders;
		inline static std::unordered_map<ShaderProgramID, ShaderProgram> shadersPrograms;
		inline static std::unordered_map<TextureID, Texture> textures;
		inline static std::unordered_map<AnimationID, Animation> animations;
		inline static std::unordered_map<FontID, Font> fonts;

		inline static std::filesystem::path cacheResourceFilePath;
	};



	
	
	template <typename ... Args>
	MeshID AssetManager::TryRegisterMesh(const std::string& meshName, Args&&... args)
	{
	#ifdef _DEBUG
		const auto [pair, wasNew] = meshes.try_emplace(get_hash_id(meshName), std::forward<Args>(args)...);
		if (!wasNew)
		{
			Debugger::GetLogger().AddMessage("A mesh has the same name as \"" + meshName + "\" already exists. Didn't add the new one.", SeverityLevel::WARNING);
		}
		auto& [id, mesh] = *pair;
	#else
		auto& [id, mesh] = *meshes.try_emplace(get_hash_id(meshName), std::forward<Args>(args)...).first;
	#endif
		mesh.mMeshID = id;
		return id;
	}


	template <typename ... Args>
	ShaderID AssetManager::TryRegisterShader(const std::string& shaderName, Args&&... args)
	{
	#ifdef _DEBUG
		const auto [pair, wasNew] = shaders.try_emplace(get_hash_id(shaderName), std::forward<Args>(args)...);
		if (!wasNew)
		{
			Debugger::GetLogger().AddMessage("A shader has the same name as \"" + shaderName + "\" already exists. Didn't add the new one.", SeverityLevel::WARNING);
		}
		auto& [id, shader] = *pair;
	#else
		auto& [id, shader] = *shaders.try_emplace(get_hash_id(shaderName), std::forward<Args>(args)...).first;
	#endif
		shader.mShaderID = id;
		return id;
	}


	template <typename ... Args>
	ShaderProgramID AssetManager::TryRegisterShaderProgram(const std::string& shaderProgramName, Args&&... args)
	{
	#ifdef _DEBUG
		const auto [pair, wasNew] = shadersPrograms.try_emplace(get_hash_id(shaderProgramName), std::forward<Args>(args)...);
		if (!wasNew)
		{
			Debugger::GetLogger().AddMessage("A shader program has the same name as \"" + shaderProgramName + "\" already exists. Didn't add the new one.", SeverityLevel::WARNING);
		}
		auto& [id, shaderProgram] = *pair;
	#else
		auto& [id, shaderProgram] = *shadersPrograms.try_emplace(get_hash_id(shaderProgramName), std::forward<Args>(args)...).first;
	#endif
		shaderProgram.mShaderProgramID = id;
		shaderProgram.mShaderProgramName = shaderProgramName;
		return id;
	}


	template <typename ... Args>
	TextureID AssetManager::TryRegisterTexture(const std::string& textureName, Args&&... args)
	{
	#ifdef _DEBUG
		const auto [pair, wasNew] = textures.try_emplace(get_hash_id(textureName), std::forward<Args>(args)...);
		if (!wasNew)
		{
			Debugger::GetLogger().AddMessage("A texture has the same name as \"" + textureName + "\" already exists. Didn't add the new one.", SeverityLevel::WARNING);
		}
		auto& [id, texture] = *pair;
	#else
		auto& [id, texture] = *textures.try_emplace(get_hash_id(textureName), std::forward<Args>(args)...).first;
	#endif
		texture.mTextureID = id;
		texture.mTextureName = textureName;
		return id;
	}


	template <typename ... Args>
	AnimationID AssetManager::TryRegisterAnimation(const std::string& animationName, Args&&... args)
	{
	#ifdef _DEBUG
		const auto [pair, wasNew] = animations.try_emplace(get_hash_id(animationName), std::forward<Args>(args)...);
		if (!wasNew)
		{
			Debugger::GetLogger().AddMessage("An animation has the same name as \"" + animationName + "\" already exists. Didn't add the new one.", SeverityLevel::WARNING);
		}
		auto& [id, animation] = *pair;
	#else
		auto& [id, animation] = *animations.try_emplace(get_hash_id(animationName), std::forward<Args>(args)...).first;
	#endif
		animation.mAnimationID = id;
		animation.mAnimationName = animationName;
		return id;
	}


	template <typename ... Args>
	FontID AssetManager::TryRegisterFont(const std::string& fontName, Args&&... args)
	{
#ifdef _DEBUG
		const auto [pair, wasNew] = fonts.try_emplace(get_hash_id(fontName), std::forward<Args>(args)...);
		if (!wasNew)
		{
			Debugger::GetLogger().AddMessage("A font has the same name as \"" + fontName + "\" already exists. Didn't add the new one.", SeverityLevel::WARNING);
		}
		auto& [id, font] = *pair;
#else
		auto& [id, font] = *fonts.try_emplace(get_hash_id(fontName), std::forward<Args>(args)...).first;
#endif
		font.mFontID = id;
		return id;
	}

}
