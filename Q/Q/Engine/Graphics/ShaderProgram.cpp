/*
    File Name: ShaderProgram.cpp
    Project Name: Q
    Author(s):
        Primary: Joonho Hwang
        Secondary:
    All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#include "ShaderProgram.h"

#include <GL/glew.h>


namespace q_engine
{
	ShaderProgram::ShaderProgram()
	{
		mShaderProgramHandle = glCreateProgram();
	}


	ShaderProgram::~ShaderProgram()
	{
		glDeleteProgram(mShaderProgramHandle);
	}


	bool ShaderProgram::AttachOrReplaceShader(const Shader& shader)
	{
		auto [it, wasNew] = mShaders.insert({ shader.GetShaderType(), shader.GetShaderHandle() });
		if (!wasNew)
		{
			glDetachShader(mShaderProgramHandle, it->second);
			it->second = shader.GetShaderHandle();
		}
		
		glAttachShader(mShaderProgramHandle, shader.GetShaderHandle());

		return wasNew;
	}


	bool ShaderProgram::Link() const
	{
		glLinkProgram(mShaderProgramHandle);

		GLint linkStatus;
		glGetProgramiv(mShaderProgramHandle, GL_LINK_STATUS, &linkStatus);
		
		return linkStatus != GL_FALSE;
	}


	bool ShaderProgram::Validate() const
	{
		glValidateProgram(mShaderProgramHandle);
		
		GLint validateStatus;
		glGetProgramiv(mShaderProgramHandle, GL_VALIDATE_STATUS, &validateStatus);

		return validateStatus != GL_FALSE;
	}


	bool ShaderProgram::AttachLinkValidate(const Shader& shader)
	{
		return AttachOrReplaceShader(shader) && Link() && Validate();
	}


	bool ShaderProgram::HasShader(const EShaderType shaderType) const
	{
		return mShaders.count(shaderType);
	}


	ShaderProgram::ShaderHandle ShaderProgram::GetShaderHandle(const EShaderType shaderType) const
	{
		return mShaders.at(shaderType);
	}
}
