/*
    File Name: CameraEntity.cpp
    Project Name: Q
    Author(s):
        Primary: Joonho Hwang
        Secondary:
    All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#include "CameraEntity.h"

#include "../Graphics/Window.h"


namespace q_engine
{
	glm::vec2 CameraEntity::CalculateWindowPositionToWorld(const double windowX, const double windowY, const glm::vec2 camPosition, const float camRotationRadian, const glm::vec2 camScale)
	{
		auto [width, height] = get_window_size();
		const glm::mat3 window2Cam = {
			1, 0, 0,
			0, -1, 0,
			-static_cast<float>(width) / 2.f, static_cast<float>(height) / 2.f, 1
		};

		return glm::inverse(calculateWorldToCam(camPosition, camRotationRadian, camScale)) * window2Cam * glm::vec3{ windowX, windowY, 1 };
	}


	glm::mat3 CameraEntity::CalculateWorldToNDCTransform(const glm::vec2 camPosition, const float camRotationRadian, const glm::vec2 camScale)
	{
		return CalculateWindowToNDCTransform() * calculateWorldToCam(camPosition, camRotationRadian, camScale);
	}


	glm::mat3 CameraEntity::CalculateWindowToNDCTransform(const glm::vec2 camPositionOffset, const float camRotationRadianOffset)
	{
		const float COS = cos(camRotationRadianOffset);
		const float SIN = sin(camRotationRadianOffset);
		
		auto [width, height] = get_window_size();
		return glm::mat3{
			2.f / static_cast<float>(width), 0, 0,
			0, 2.f / static_cast<float>(height),
			0, 0, 0, 1
		} * glm::mat3{
			1, 0, 0,
			0, 1, 0,
			camPositionOffset.x, camPositionOffset.y, 1
		} * glm::mat3{
			COS, SIN, 0,
			-SIN, COS, 0,
			0, 0, 1
		};
	}


	glm::mat3 CameraEntity::calculateWorldToCam(const glm::vec2 camPosition, const float camRotationRadian, const glm::vec2 camScale)
	{
		const float COS = cos(camRotationRadian);
		const float SIN = sin(camRotationRadian);
		
		return glm::mat3{
				COS, SIN, 0,
				-SIN, COS, 0,
				0, 0, 1
		} * glm::mat3{
			camScale.x, 0, 0,
			0, camScale.y, 0,
			0, 0, 1
		} * glm::mat3{
			1, 0, 0,
			0, 1, 0,
			-camPosition.x, -camPosition.y, 1
		};
	}
}
