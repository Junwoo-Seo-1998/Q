/*
    File Name: TransformComponent.h
    Project Name: Q
    Author(s):
        Primary: Joonho Hwang
        Secondary:
    All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#pragma once
#include "../ECS/Component.h"

#include <glm/mat3x3.hpp>


namespace sol
{
    class state;
}

namespace q_engine
{
    
	class TransformComponent final : public Component<TransformComponent>
	{
        friend class EntityDebugger;

    public:
        TransformComponent() noexcept = default;
		
        TransformComponent(glm::vec2 pos) noexcept
            : mPosition(pos)
        {}
		
        TransformComponent(float posX, float posY) noexcept
            : mPosition{ posX, posY }
        {}
		
        TransformComponent(glm::vec2 pos, float rotationRadian) noexcept
            : mPosition(pos)
        {
            SetRotationRadian(rotationRadian);
        }
		
        TransformComponent(float posX, float posY, float rotationRadian) noexcept
            : mPosition{posX, posY}
        {
            SetRotationRadian(rotationRadian);
        }
		
        TransformComponent(glm::vec2 pos, float rotationRadian, glm::vec2 scale) noexcept
            : mPosition(pos), mScale(scale)
        {
            SetRotationRadian(rotationRadian);
        }
		
        TransformComponent(float posX, float posY, float rotationRadian, float scaleX, float scaleY) noexcept
            : mPosition{posX, posY}, mScale{scaleX, scaleY}
        {
            SetRotationRadian(rotationRadian);
        }

        [[nodiscard]] constexpr glm::vec2 GetPosition() const noexcept { return mPosition; }
        [[nodiscard]] constexpr float GetPositionX() const noexcept { return mPosition.x; }
        [[nodiscard]] constexpr float GetPositionY() const noexcept { return mPosition.y; }

        [[nodiscard]] constexpr float GetRotationRadian() const noexcept { return mRotationRadian; }
		
        [[nodiscard]] constexpr glm::vec2 GetScale() const noexcept { return mScale; }
        [[nodiscard]] constexpr float GetScaleX() const noexcept { return mScale.x; }
        [[nodiscard]] constexpr float GetScaleY() const noexcept { return mScale.y; }

        constexpr void SetPosition(float x, float y) noexcept { mPosition = { x, y }; mIsCacheDirty = true; }
        constexpr void SetPosition(glm::vec2 pos) noexcept { mPosition = pos; mIsCacheDirty = true; }
        constexpr void SetPositionX(float x) noexcept { mPosition.x = x; mIsCacheDirty = true; }
        constexpr void SetPositionY(float y) noexcept { mPosition.y = y; mIsCacheDirty = true; }

        void SetRotationRadian(float angleRadian) noexcept;

        constexpr void SetScale(float x, float y) noexcept { mScale = { x, y }; mIsCacheDirty = true; }
        constexpr void SetScale(glm::vec2 scale) noexcept { mScale = scale; mIsCacheDirty = true; }
        constexpr void SetScaleX(float x) noexcept { mScale.x = x; mIsCacheDirty = true; }
        constexpr void SetScaleY(float y) noexcept { mScale.y = y; mIsCacheDirty = true; }


        constexpr void AddPosition(float x, float y) noexcept { mPosition += glm::vec2{ x, y }; mIsCacheDirty = true; }
        constexpr void AddPosition(glm::vec2 posOffset) noexcept { mPosition += posOffset; mIsCacheDirty = true; }

        void AddRotationRadian(float angleRadian) noexcept;

        constexpr void AddScale(float x, float y) noexcept { mScale += glm::vec2{ x, y }; mIsCacheDirty = true; }
        constexpr void AddScale(glm::vec2 scaleOffset) noexcept { mScale += scaleOffset; mIsCacheDirty = true; }


        constexpr void MultiplyScale(float multiplier) noexcept { mScale *= multiplier; mIsCacheDirty = true; }
		
        constexpr void MultiplyScale(float x, float y) noexcept { mScale *= glm::vec2{ x, y }; mIsCacheDirty = true; }
        constexpr void MultiplyScale(glm::vec2 multiplier) noexcept { mScale *= multiplier; mIsCacheDirty = true; }


        [[nodiscard]] const glm::mat3& GetOrCalculateTransformMatrix() const noexcept;


        Json::Value Serialize() override;
        void Deserialize(Json::Value& jsonValue) override;
        void test();
    private:
        mutable glm::vec2 mPosition{ 0, 0 };
        float mRotationRadian{ 0 };
        glm::vec2 mScale{ 1, 1 };

        mutable glm::mat3 mTransformMatrix{ 1, 0, 0, 0, 1, 0, 0, 0, 1 };

        mutable bool mIsCacheDirty = true;
	};
    //for lua
    void RegisterTransformComponent(sol::state& state);
   
}
