/*
    File Name: DrawObject.h
    Project Name: Q
    Author(s):
        Primary: Joonho Hwang
        Secondary:Junwoo Seo
    All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#pragma once
#include <glm/mat3x3.hpp>
#include <glm/ext/matrix_transform.hpp>


#include "Color.h"
#include "GraphicsMisc.h"
#include "Mesh.h"
#include "ShaderProgram.h"
#include "Texture.h"


namespace q_engine
{
    class [[nodiscard]] DrawObject
    {
    public:
        DrawObject();
    	
        DrawObject(MeshID meshID, ShaderProgramID shaderProgramID, ZDepth zDepth = 0, bool isFixedOnScreen = false, EFrameOfReference frameOfReference = EFrameOfReference::BOTTOM_LEFT);
        
        DrawObject(MeshID meshID, ShaderProgramID shaderProgramID, TextureID textureID, ZDepth zDepth = 0, bool isFixedOnScreen = false, EFrameOfReference frameOfReference = EFrameOfReference::BOTTOM_LEFT);

        DrawObject(MeshID meshID, ShaderProgramID shaderProgramID, Color color, ZDepth zDepth = 0, bool isFixedOnScreen = false, EFrameOfReference frameOfReference = EFrameOfReference::BOTTOM_LEFT);

        DrawObject(MeshID meshID, ShaderProgramID shaderProgramID, Color color, LineOrPointSize lineOrPointSize, ZDepth zDepth = 0, bool isFixedOnScreen = false, EFrameOfReference frameOfReference = EFrameOfReference::BOTTOM_LEFT);
    	

        [[nodiscard]] constexpr MeshID GetMeshID() const noexcept { return mMeshID; }
        [[nodiscard]] constexpr ShaderProgramID GetShaderProgramID() const noexcept { return mShaderProgramID; }
        [[nodiscard]] std::string GetShaderName() const;
        [[nodiscard]] constexpr TextureID GetTextureID() const { return mTextureID; }
        [[nodiscard]] std::string GetTextureName() const;
        constexpr Color GetColor() const { return mColor; }
        LineOrPointSize GetLineOrPointSize() const { return mLineOrPointSize; }
    	
        ZDepth GetZDepth() const noexcept { return mZDepth; }
        constexpr EFrameOfReference GetFrameOfReference() const noexcept { return mFrameOfReference; }

        [[nodiscard]] constexpr bool IsFixedOnScreen() const noexcept { return mIsFixedOnScreen; }


        constexpr void SetMeshID(MeshID meshID) noexcept { mMeshID = meshID; }
        constexpr void SetShaderProgramID(ShaderProgramID shaderProgramID) noexcept { mShaderProgramID = shaderProgramID; }

    	constexpr void SetTextureID(TextureID textureID) noexcept { mTextureID = textureID; }
		constexpr void SetColor(Color color) noexcept { mColor = color; }
        void SetLineWidth(LineOrPointSize lineOrPointSize) { mLineOrPointSize = lineOrPointSize; }

    	void SetZDepth(ZDepth zDepth) noexcept { mZDepth = zDepth; }
        constexpr void SetFrameOfReference(EFrameOfReference frameOfReference) noexcept { mFrameOfReference = frameOfReference; }

        constexpr void SetIsFixedOnScreen(bool isFixedOnScreen) noexcept { mIsFixedOnScreen = isFixedOnScreen; }
    	

        void Draw(const glm::mat3& model2NDCTransform = glm::identity<glm::mat3>()) const;
        void Draw(const glm::mat4& model2NDCTransform = glm::identity<glm::mat4>()) const;

        //void ChangeShader(ShaderProgramID newID)
        //{
        //    mShaderProgramID = newID;
        //}
    private:
        MeshID mMeshID;
        ShaderProgramID mShaderProgramID;

    	// (TextureID + texel info) or (Color + (LineWidth or PointSize))
        union
        {
            struct
            {
                TextureID mTextureID;
                int mTexelX;
                int mTexelY;
                int mTexelWidth;
                int mTexelHeight;
            };
            struct
            {
                Color mColor;
                LineOrPointSize mLineOrPointSize;
            };
        };
    	
        ZDepth mZDepth;
        EFrameOfReference mFrameOfReference;
        bool mIsFixedOnScreen;
    };
} // namespace q_engine
