/*
    File Name: TextureComponent.h
    Project Name: Q
    Author(s):
        Primary: Joonho Hwang
        Secondary:
    All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#pragma once
#include "../ECS/Component.h"

#include "../Core/StringHash.h"
#include "../Graphics/DrawObject.h"


namespace q_engine
{
	class TextureComponent final : public Component<TextureComponent>
    {
    public:
        TextureComponent();
        TextureComponent(TextureID textureID, ZDepth zDepth = 0, bool isFixedOnScreen = false, EFrameOfReference frameOfReference = EFrameOfReference::BOTTOM_LEFT, ShaderProgramID shaderProgramID = "DefaultTextureShaderProgram"_hashID);
        TextureComponent(TextureID textureID, int texelX, int texelY, int texelWidth, int texelHeight, ZDepth zDepth = 0, bool isFixedOnScreen = false, EFrameOfReference frameOfReference = EFrameOfReference::BOTTOM_LEFT, ShaderProgramID shaderProgramID = "DefaultTextureShaderProgram"_hashID);
        TextureComponent(TextureID textureID, float texelX, float texelY, float texelWidth, float texelHeight, ZDepth zDepth = 0, bool isFixedOnScreen = false, EFrameOfReference frameOfReference = EFrameOfReference::BOTTOM_LEFT, ShaderProgramID shaderProgramID = "DefaultTextureShaderProgram"_hashID);

        [[nodiscard]] std::string GetTextureName() const { return mDrawObject.GetTextureName(); }
        [[nodiscard]] std::string GetShaderProgramName() const { return mDrawObject.GetShaderName(); }
        [[nodiscard]] constexpr TextureID GetTextureID() const noexcept { return mDrawObject.GetTextureID(); }

        [[nodiscard]] constexpr bool IsFixedOnScreen() const noexcept { return mDrawObject.IsFixedOnScreen(); }

        [[nodiscard]] int GetWidth() const noexcept;
        [[nodiscard]] int GetHeight() const noexcept;
        
    	constexpr void SetTexture(TextureID textureID) noexcept { mDrawObject.SetTextureID(textureID); }
        constexpr void SetShader(ShaderProgramID shaderID) noexcept { mDrawObject.SetShaderProgramID(shaderID); }
        void SetZDepth(ZDepth zDepth) noexcept { mDrawObject.SetZDepth(zDepth); }

        void Draw(const glm::mat3& model2NDCTransform = { 1, 0, 0, 0, 1, 0, 0, 0, 1 }) const;

        constexpr const DrawObject& GetDrawObject() const noexcept { return mDrawObject; }
        constexpr DrawObject& GetDrawObject() noexcept { return mDrawObject; }

        virtual Json::Value Serialize() override;
        virtual void Deserialize(Json::Value& jsonValue) override;
    private:
        DrawObject mDrawObject;
    };

    void RegisterTextureComponent(sol::state& state);
}
