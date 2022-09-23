/*
    File Name: DrawObject.cpp
    Project Name: Q
    Author(s):
        Primary: Joonho Hwang
        Secondary:Junwoo Seo
    All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#include "DrawObject.h"

#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>  // glm::value_ptr

#include "../AssetManager.h"


namespace q_engine
{
	DrawObject::DrawObject()
		: DrawObject(INVALID_ID, INVALID_ID)
	{}


	DrawObject::DrawObject(const MeshID meshID, const ShaderProgramID shaderProgramID, const ZDepth zDepth, const bool isFixedOnScreen, const EFrameOfReference frameOfReference)
		: DrawObject(meshID, shaderProgramID, INVALID_ID, zDepth, isFixedOnScreen, frameOfReference)
	{}


	DrawObject::DrawObject(const MeshID meshID, const ShaderProgramID shaderProgramID, const TextureID textureID, const ZDepth zDepth, const bool isFixedOnScreen, const EFrameOfReference frameOfReference)
		: mMeshID(meshID), mShaderProgramID(shaderProgramID), mTextureID(textureID), mZDepth(zDepth), mFrameOfReference(frameOfReference), mIsFixedOnScreen(isFixedOnScreen)
    {}


	DrawObject::DrawObject(const MeshID meshID, const ShaderProgramID shaderProgramID, const Color color, const ZDepth zDepth, const bool isFixedOnScreen, const EFrameOfReference frameOfReference)
        : DrawObject(meshID, shaderProgramID, color, 1.f, zDepth, isFixedOnScreen, frameOfReference)
    {}


	DrawObject::DrawObject(const MeshID meshID, const ShaderProgramID shaderProgramID, const Color color, const LineOrPointSize lineOrPointSize, const ZDepth zDepth, const bool isFixedOnScreen, const EFrameOfReference frameOfReference)
        : mMeshID(meshID), mShaderProgramID(shaderProgramID), mColor(color), mLineOrPointSize(lineOrPointSize), mZDepth(zDepth), mFrameOfReference(frameOfReference), mIsFixedOnScreen(isFixedOnScreen)
    {}

	std::string DrawObject::GetShaderName() const
	{
        ShaderProgram& shaderProgram = AssetManager::GetShaderProgram(mShaderProgramID);
        return shaderProgram.GetShaderProgramName();
	}

	std::string DrawObject::GetTextureName() const
	{
        const Texture& texture = AssetManager::GetTexture(mTextureID);
        return texture.GetTextureName();
	}

	void DrawObject::Draw(const glm::mat3& model2NDCTransform) const
	{
        glm::mat4 res = glm::identity<glm::mat4>();
        res[0].x = model2NDCTransform[0].x;
        res[0].y = model2NDCTransform[0].y;
        res[1].x = model2NDCTransform[1].x;
        res[1].y = model2NDCTransform[1].y;
        res[3].x = model2NDCTransform[2].x;
        res[3].y = model2NDCTransform[2].y;
        Draw(res);
	}

	void DrawObject::Draw(const glm::mat4& model2NDCTransform) const
	{
        const Mesh& mesh = AssetManager::GetMesh(mMeshID);
        const ShaderProgram& shaderProgram = AssetManager::GetShaderProgram(mShaderProgramID);

        const auto shaderProgramHandle = shaderProgram.GetShaderProgramHandle();

        glUseProgram(shaderProgramHandle);
        glBindVertexArray(mesh.GetVAOHandle());

        const EPrimitiveType primitiveType = mesh.GetPrimitiveType();
        switch (primitiveType)
        {
        case EPrimitiveType::POINTS:
            glPointSize(mLineOrPointSize);
            break;

        case EPrimitiveType::LINES:
        case EPrimitiveType::LINE_STRIP:
        case EPrimitiveType::LINE_LOOP:
            glLineWidth(mLineOrPointSize);
            break;

        default:
            break;
        }

  
        const GLint uVarColor = glGetUniformLocation(shaderProgramHandle, "uColor");
        const GLint uVarModel2NDC = glGetUniformLocation(shaderProgramHandle, "uModel2NDC");
        const GLint uVarTexture2D = glGetUniformLocation(shaderProgramHandle, "uTexture2D");
        const GLint uVarZDepth = glGetUniformLocation(shaderProgramHandle, "uZDepth");
        const GLint uVarFrameOfReference = glGetUniformLocation(shaderProgramHandle, "uFrameOfReference");


        if (uVarColor >= 0)
        {
            glUniform4f(uVarColor, mColor.r, mColor.g, mColor.b, mColor.a);
        }
        if (uVarModel2NDC >= 0)
        {
            if (uVarTexture2D >= 0)
            {
              
                const Texture& texture = AssetManager::GetTexture(mTextureID);
                glBindTextureUnit(0, texture.GetTextureHandle());

                glUniform1i(uVarTexture2D, 0);

                glUniformMatrix4fv(uVarModel2NDC, 1, GL_FALSE, glm::value_ptr(model2NDCTransform * glm::mat4{ texture.GetWidth(), 0, 0, 0, 0,texture.GetHeight(), 0, 0, 0,0,1,0,0,0,0, 1 }));
            }
            else
            {
                glUniformMatrix4fv(uVarModel2NDC, 1, GL_FALSE, glm::value_ptr(model2NDCTransform));
            }
        }
        if (uVarZDepth >= 0)
        {
            glUniform1f(uVarZDepth, mZDepth);
        }
        if (uVarFrameOfReference >= 0)
        {
            glUniform1i(uVarFrameOfReference, static_cast<int>(mFrameOfReference));
        }

        glDrawElements(static_cast<unsigned int>(primitiveType), mesh.GetDrawCount(), GL_UNSIGNED_SHORT, nullptr);

        glBindVertexArray(0);
        glUseProgram(0);
	}
} // namespace q_engine
