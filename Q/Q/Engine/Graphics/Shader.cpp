/*
    File Name: Shader.cpp
    Project Name: Q
    Author(s):
        Primary: Joonho Hwang
        Secondary:
    All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#include "Shader.h"

#include <fstream>
#include <sstream>

#include <GL/glew.h>

#include "../Debugger/Debugger.h"


namespace q_engine
{
	std::istream& operator>>(std::istream& inputStream, EShaderType& shaderType)
	{
		std::string text;

		inputStream >> text;

		if (text == "VERTEX_SHADER")
		{
			shaderType = EShaderType::VERTEX_SHADER;
		}
		else if (text == "FRAGMENT_SHADER")
		{
			shaderType = EShaderType::FRAGMENT_SHADER;
		}

		return inputStream;
	}


	Shader::Shader(EShaderType shaderType, const std::filesystem::path& shaderFilePath)
	{
		GLenum gl_shader_type;
		
		switch (shaderType)
		{
			case EShaderType::VERTEX_SHADER:
				gl_shader_type = GL_VERTEX_SHADER;
				break;

			case EShaderType::FRAGMENT_SHADER:
				gl_shader_type = GL_FRAGMENT_SHADER;
				break;
		}

		const std::ifstream shaderFile{ shaderFilePath };
		if (!shaderFile)
		{
			Debugger::GetLogger().AddMessage("Failed to open file " + shaderFilePath.string(), SeverityLevel::ERROR);
		#ifdef _DEBUG
            std::exit(EXIT_FAILURE);
		#else
			return;
		#endif
		}

		std::stringstream sstr;
		sstr << shaderFile.rdbuf();
		const std::string content = sstr.str();
		const char* const pContent = content.c_str();
		
		const ShaderHandle shaderHandle = glCreateShader(gl_shader_type);
		glShaderSource(shaderHandle, 1, &pContent, nullptr);
		glCompileShader(shaderHandle);

		GLint compileStatus;
		glGetShaderiv(shaderHandle, GL_COMPILE_STATUS, &compileStatus);
		if (compileStatus == GL_FALSE)
		{
			GLint log_len;
			glGetShaderiv(shaderHandle, GL_INFO_LOG_LENGTH, &log_len);
			if (log_len > 0) {
				GLchar* log = new GLchar[log_len];
				GLsizei written_log_len;
				glGetShaderInfoLog(shaderHandle, log_len, &written_log_len, log);
				Debugger::GetLogger().AddMessage("Error while compiling shader file " + shaderFilePath.string() + ": " + std::string(log), SeverityLevel::ERROR);
				delete[] log;
			}
		#ifdef _DEBUG
            std::exit(EXIT_FAILURE);
		#else
			return;
		#endif
		}

		mShaderHandle = shaderHandle;
		mShaderType = shaderType;
	}


	Shader::~Shader()
	{
		glDeleteShader(mShaderHandle);
	}
} // namespace q_engine
