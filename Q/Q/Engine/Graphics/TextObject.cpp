/*
    File Name: TextObject.cpp
    Project Name: Q
    Author(s):
        Primary: Joonho Hwang
        Secondary:
    All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#include "TextObject.h"

#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>  // glm::value_ptr()

#include "../AssetManager.h"
#include "../Entity/CameraEntity.h"
#include "Window.h"


namespace q_engine
{
    TextObject::TextObject(const FontID fontID, std::string text, const ZDepth zDepth)
        : mFontID(fontID), mText(std::move(text)), mZDepth(zDepth)
    {}


    TextObject::TextObject(const FontID fontID, std::string text, const float fontSize, const Color color, const ZDepth zDepth)
        : mFontID(fontID), mText(std::move(text)), mFontSize(fontSize), mColor(color), mZDepth(zDepth)
    {}


    void TextObject::Draw(const glm::vec2 translation, const float rotation, const glm::vec2 scale) const
    {
        static Mesh& mesh = AssetManager::GetMesh("SquareMesh");
        const auto shaderProgramHandle = AssetManager::GetShaderProgram("DefaultTextShaderProgram").GetShaderProgramHandle();
    	
        glUseProgram(shaderProgramHandle);
        glBindVertexArray(mesh.GetVAOHandle());

        const GLint uVarColor = glGetUniformLocation(shaderProgramHandle, "uColor");
        const GLint uVarModel2NDC = glGetUniformLocation(shaderProgramHandle, "uModel2NDC");
        const GLint uVarTexture2D = glGetUniformLocation(shaderProgramHandle, "uTexture2D");
        const GLint uVarZDepth = glGetUniformLocation(shaderProgramHandle, "uZDepth");

        glUniform3f(uVarColor, mColor.r, mColor.g, mColor.b);

    	
        Font& font = AssetManager::GetFont(mFontID);
        const unsigned int realFontSize = Font::GetClosestFontSize(mFontSize);

        const float fontSizeRatioToBaseFontSize = mFontSize / static_cast<float>(realFontSize);

        const float COS = cos(rotation);
        const float SIN = sin(rotation);
    	
        const glm::mat3 camTransform = CameraEntity::CalculateWindowToNDCTransform();
        const glm::mat3 translationTransform = {
        	1, 0, 0,
        	0, 1, 0,
        	translation.x, translation.y, 1
        };
        const glm::mat3 rotationTransform = {
        	COS, SIN, 0,
        	-SIN, COS, 0,
        	0, 0, 1
        };
    	const glm::mat3 scaleTransform = {
    		fontSizeRatioToBaseFontSize * scale.x, 0, 0,
    		0, fontSizeRatioToBaseFontSize * scale.y, 0,
    		0, 0, 1
    	};

        float xOffset = 0;
        float yOffset = 0;
        CharacterType prev = 0;
    	for (CharacterType c : mText)
    	{
    		if (c == '\n')
    		{
                xOffset = 0;
                yOffset -= font.GetFontHeight(realFontSize) * fontSizeRatioToBaseFontSize * scale.y;
                continue;
    		}
    		if (!std::isprint(c))
    		{
                continue;
    		}

            xOffset += font.GetKerning(realFontSize, prev, c) * fontSizeRatioToBaseFontSize * scale.x;

            const Font::GlyphInfo glyphInfo = font.GetGlyphInfo(c, realFontSize);
    		
            const glm::mat3 translationTransformPerChar = glm::mat3{
            	1, 0, 0,
            	0, 1, 0,
            	static_cast<float>(glyphInfo.bearing[0]) * fontSizeRatioToBaseFontSize, -static_cast<float>(glyphInfo.size[1] - glyphInfo.bearing[1]) * fontSizeRatioToBaseFontSize, 1 }
    		* glm::mat3{
    			1, 0, 0,
    			0, 1, 0,
    			xOffset, yOffset, 1
    		};
            const glm::mat3 scaleTransformPerChar = glm::mat3{
            	glyphInfo.size[0], 0, 0,
            	0, glyphInfo.size[1], 0,
            	0, 0, 1
            };
    		
            glBindTextureUnit(0, glyphInfo.textureHandle);
            glUniform1i(uVarTexture2D, 0);
            glUniformMatrix3fv(uVarModel2NDC, 1, GL_FALSE, glm::value_ptr(
                camTransform * 
                translationTransform * 
                rotationTransform *
                translationTransformPerChar *
                scaleTransformPerChar * 
                scaleTransform
            ));
            if (uVarZDepth >= 0)
            {
                glUniform1f(uVarZDepth, mZDepth);
            }

            glDrawElements(GL_TRIANGLE_STRIP, mesh.GetDrawCount(), GL_UNSIGNED_SHORT, nullptr);

            xOffset += static_cast<float>(glyphInfo.advance >> 6) * fontSizeRatioToBaseFontSize * scale.x;

            prev = c;
    	}

        glBindVertexArray(0);
        glUseProgram(0);
    }
}
