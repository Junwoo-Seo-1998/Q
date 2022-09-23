/*
    File Name: Shader.h
    Project Name: Q
    Author(s):
        Primary: Joonho Hwang
        Secondary:
    All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#pragma once
#include <filesystem>
#include <istream>
#include "Engine/Types.h"

namespace q_engine
{
    enum class [[nodiscard]] EShaderType
    {
	    VERTEX_SHADER,
    	FRAGMENT_SHADER
    };

    std::istream& operator>>(std::istream& inputStream, EShaderType& shaderType);

    class [[nodiscard]] Shader
    {
        using ShaderHandle = unsigned;
    	
        friend class AssetManager;
    public:
        Shader(EShaderType shaderType, const std::filesystem::path& shaderFilePath);
        ~Shader();

        Shader(const Shader& other) = delete;
        Shader(Shader&& other) noexcept = default;
        Shader& operator=(const Shader & other) = delete;
        Shader& operator=(Shader && other) noexcept = default;

        [[nodiscard]] constexpr ShaderHandle GetShaderHandle() const noexcept { return mShaderHandle; }
        constexpr EShaderType GetShaderType() const noexcept { return mShaderType; }

        [[nodiscard]] constexpr ShaderID GetShaderID() const noexcept { return mShaderID; }
    	


    private:
        ShaderHandle mShaderHandle;
        EShaderType mShaderType;

        ShaderID mShaderID = INVALID_ID;
    };
} // namespace q_engine
