/*
    File Name: CameraEntity.h
    Project Name: Q
    Author(s):
        Primary: Joonho Hwang
        Secondary:
    All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#pragma once
#include "../ECS/Entity.h"

#include <glm/mat3x3.hpp>


namespace q_engine
{
    class CameraEntity final : public Entity<CameraEntity>
    {
    public:
    	static glm::vec2 CalculateWindowPositionToWorld(double windowX, double windowY, glm::vec2 camPosition, float camRotationRadian, glm::vec2 camScale);
        static glm::mat3 CalculateWorldToNDCTransform(glm::vec2 camPosition, float camRotationRadian, glm::vec2 camScale);
        static glm::mat3 CalculateWindowToNDCTransform(glm::vec2 camPositionOffset = { 0, 0 }, float camRotationRadianOffset = 0.f);
    	

    private:
        static glm::mat3 calculateWorldToCam(glm::vec2 camPosition, float camRotationRadian, glm::vec2 camScale);
    };
}
