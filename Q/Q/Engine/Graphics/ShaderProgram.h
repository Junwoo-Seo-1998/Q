/*
    File Name: ShaderProgram.h
    Project Name: Q
    Author(s):
        Primary: Joonho Hwang
        Secondary:
    All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#pragma once
#include <map>

#include "Shader.h"
#include "Engine/Types.h"

namespace q_engine
{

	class [[nodiscard]] ShaderProgram
	{
		using ShaderHandle = unsigned;
		using ShaderProgramHandle = unsigned;
		
		friend class AssetManager;
	public:
		ShaderProgram();
		~ShaderProgram();

		ShaderProgram(const ShaderProgram& other) = delete;
		ShaderProgram(ShaderProgram&& other) noexcept = default;
		ShaderProgram& operator=(const ShaderProgram& other) = delete;
		ShaderProgram& operator=(ShaderProgram&& other) noexcept = default;

		bool AttachOrReplaceShader(const Shader& shader);
		bool Link() const;
		bool Validate() const;
		bool AttachLinkValidate(const Shader& shader);

		[[nodiscard]] constexpr ShaderProgramHandle GetShaderProgramHandle() const noexcept { return mShaderProgramHandle; }
		[[nodiscard]] constexpr ShaderProgramID GetShaderProgramID() const noexcept { return mShaderProgramID; }
		[[nodiscard]] std::string GetShaderProgramName() const { return mShaderProgramName; }
		[[nodiscard]] bool HasShader(EShaderType shaderType) const;
		[[nodiscard]] ShaderHandle GetShaderHandle(EShaderType shaderType) const;
		[[nodiscard]] constexpr const std::map<EShaderType, ShaderHandle>& GetShaders() const noexcept { return mShaders; }
		
		

	private:
		ShaderProgramHandle mShaderProgramHandle;

		std::map<EShaderType, ShaderHandle> mShaders;

		ShaderProgramID mShaderProgramID = INVALID_ID;
		std::string mShaderProgramName="";
	};
}