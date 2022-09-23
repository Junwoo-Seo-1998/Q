/*
    File Name: TextureComponent.cpp
    Project Name: Q
    Author(s):
        Primary: Joonho Hwang
        Secondary:
    All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#include "TextureComponent.h"

#include "../AssetManager.h"


namespace q_engine
{
	TextureComponent::TextureComponent()
		: mDrawObject(INVALID_ID, INVALID_ID, INVALID_ID, 0, false, EFrameOfReference::BOTTOM_LEFT)
	{}

	TextureComponent::TextureComponent(const TextureID textureID, const ZDepth zDepth, const bool isFixedOnScreen, const EFrameOfReference frameOfReference, const ShaderProgramID shaderProgramID)
		: mDrawObject("SquareMesh"_hashID, shaderProgramID, textureID, zDepth, isFixedOnScreen, frameOfReference)
	{}


	TextureComponent::TextureComponent(const TextureID textureID, const int texelX, const int texelY, const int texelWidth, const int texelHeight, const ZDepth zDepth, const bool isFixedOnScreen, const EFrameOfReference frameOfReference, const ShaderProgramID shaderProgramID)
	{
		const Texture& original = AssetManager::GetTexture(textureID);
		const TextureID newTextureID = AssetManager::TryRegisterTexture("OriginalTexelModified" + std::to_string(textureID) + std::to_string(texelX) + std::to_string(texelY) + std::to_string(texelWidth) + std::to_string(texelHeight),
			original.GetTextureHandle(), original.GetSizedInternalFormat(), original.GetBaseInternalFormat(),
			texelX, texelY, texelWidth, texelHeight);

		mDrawObject = DrawObject{ "SquareMesh"_hashID, shaderProgramID, newTextureID, zDepth, isFixedOnScreen, frameOfReference };
	}


	TextureComponent::TextureComponent(const TextureID textureID,  float texelX, float texelY, float texelWidth, float texelHeight, const ZDepth zDepth, const bool isFixedOnScreen, const EFrameOfReference frameOfReference, const ShaderProgramID shaderProgramID)
		:TextureComponent(textureID, static_cast<int>(texelX), static_cast<int>(texelY),
			static_cast<int>(texelWidth), static_cast<int>(texelHeight), zDepth, isFixedOnScreen, frameOfReference, shaderProgramID)
	{}


	int TextureComponent::GetWidth() const noexcept
	{
		return AssetManager::GetTexture(mDrawObject.GetTextureID()).GetWidth();
	}


	int TextureComponent::GetHeight() const noexcept
	{
		return AssetManager::GetTexture(mDrawObject.GetTextureID()).GetHeight();
	}


	void TextureComponent::Draw(const glm::mat3& model2NDCTransform) const
	{
		mDrawObject.Draw(model2NDCTransform);
	}

	Json::Value TextureComponent::Serialize()
	{
		Json::Value value;
		value["shader"] = GetShaderProgramName();
		value["texture"] = GetTextureName();
		return value;
	}

	void TextureComponent::Deserialize(Json::Value& jsonValue)
	{
		std::string shader = jsonValue["shader"].asString();
		std::string texture = jsonValue["texture"].asString();
		mDrawObject.SetMeshID("SquareMesh"_hashID);
		SetTexture(q_engine::get_hash_id(texture));
		SetShader(q_engine::get_hash_id(shader));
		
	}
	void RegisterTextureComponent(sol::state& state)
	{
		sol::usertype<TextureComponent> textureComponent = state.new_usertype<TextureComponent>("TextureComponent");
		textureComponent.set_function("SetTexture", &TextureComponent::SetTexture);
		textureComponent.set_function("GetTextureID", &TextureComponent::GetTextureID);
	}
}
