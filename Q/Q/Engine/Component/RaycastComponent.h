/*
    File Name: RaycastComponent.h
    Project Name: Q
    Author(s):
        Primary: Hoseob Jeong
        Secondary:
    All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#pragma once
#include "../ECS/Component.h"
#include <vector>
#include "glm/glm.hpp"
namespace q_engine
{
    class RaycastComponent  : public Component<RaycastComponent>
    {
    public:
        RaycastComponent() = default;
        RaycastComponent(glm::vec2 TargetPosition);
        RaycastComponent(glm::vec2 TargetPosition, int Raysize);
        RaycastComponent(int Raysize, float angle);
        RaycastComponent(int Raysize);
        ~RaycastComponent();
        void UpdateTargetPosition(glm::vec2 TargetPosition);
        glm::vec2 GetTargetPosition();
        int GetRaySize();
    	
        [[nodiscard]] std::vector<std::pair<glm::vec2, bool>>& GetIntersectPoints();
        [[nodiscard]] std::vector<glm::vec2>& GetShootRayVectorData();
        [[nodiscard]] std::vector<float>& GetDistanceData();
        [[nodiscard]] std::vector < std::pair<IDType, IDType>>& GetRayCollisionEntityTypeEntityID();
        [[nodiscard]] int GetRayOrderindex();
        void RayIndexChecking();
        float GetRayAngle();
        virtual Json::Value Serialize() override;
        virtual void Deserialize(Json::Value& jsonValue) override;
    private:
        glm::vec2 mTargetPosition{0.f,0.f};
        float mAngle = 2.f; //radians;
        int mRaySize = 1;
    	
        std::vector<std::pair<glm::vec2, bool>> mIntersectPoints{};
        std::vector<glm::vec2> mShootRayVectorData{};
        std::vector<float> mDistanceData{};
        std::vector < std::pair<IDType, IDType>> mRayCollisionEntityTypeEntityID{};
        int mRayOrderindex =0;
    	
    };

}