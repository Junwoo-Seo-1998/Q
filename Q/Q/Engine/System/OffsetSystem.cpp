/*
	File Name: OffsetSystem.cpp
	Project Name: Q
	Author(s):
		Primary: Hoseob Jeong
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/

#include "OffsetSystem.h"
#include "../../Game/Entity/PhysicsTestEntities.h"
#include "../../Game/Entity/WeaponEntity.h"
#include "../Component/CollisionComponent.h"
#include "../Component/ObjectStateComponent.h"
#include "../Component/OffsetComponent.h"
#include "../Component/TransformComponent.h"
#include "../Component/WidthHeightComponent.h"


namespace q_engine
{
	void OffsetSystem::Load()
	{

	}

	void OffsetSystem::Update([[maybe_unused]]double dt) noexcept
	{
		
			const auto& currentState = m_controlled_state;
			auto& RectCollisionComponents = currentState->GetComponents<RectCollisionComponent>();
			auto& ObjectStateComponents = currentState->GetComponents<ObjectStateComponent>();
			auto& TransformComponents = currentState->GetComponents<TransformComponent>();
			auto& WidthHeightComponents = currentState->GetComponents<WidthHeightComponent>();
			auto& OffsetComponents = currentState->GetComponents<OffsetComponent>();
		for (ObjectStateComponent& playerstate : ObjectStateComponents)
		{

			if (playerstate.GetEntityTypeID() == q_engine::GetTypeID<PlayerEntity>())
			{
				auto GetEntityID = playerstate.GetEntityID();
				TransformComponent& playerTransformComponent = TransformComponents[GetEntityID];
				bool playerIsFilpped = ObjectStateComponents[playerstate.GetEntityID()].GetIsFlipped();
				const float playerscaleXabs = std::abs(playerTransformComponent.GetScaleX());
				const float playerscaleYabs = std::abs(playerTransformComponent.GetScaleY());
				if (playerIsFilpped == true)
				{
					playerTransformComponent.SetScale(-playerscaleXabs, playerscaleYabs);
				
				}
				else
				{
					playerTransformComponent.SetScale(playerscaleXabs, playerscaleYabs);
				}
			}
			if(playerstate.GetEntityTypeID() == q_engine::GetTypeID<EnemyEntity>())
			{
				auto GetEntityID = playerstate.GetEntityID();
				TransformComponent& playerTransformComponent = TransformComponents[GetEntityID];

				bool playerIsFilpped = ObjectStateComponents[playerstate.GetEntityID()].GetIsFlipped();
				const float playerscaleXabs = std::abs(playerTransformComponent.GetScaleX());
				const float playerscaleYabs = std::abs(playerTransformComponent.GetScaleY());
				OffsetComponent& offset = OffsetComponents[GetEntityID];
				if (playerIsFilpped == true)
				{
					playerTransformComponent.SetScale(-playerscaleXabs, playerscaleYabs);
					offset.SetFlipped(true);
				}
				else
				{
					playerTransformComponent.SetScale(playerscaleXabs, playerscaleYabs);
					offset.SetFlipped(false);
				}
			}
		}

		for (RectCollisionComponent& weaponHotspot : RectCollisionComponents)
		{

			if (weaponHotspot.GetEntityTypeID() == q_engine::GetTypeID<WeaponEntity>())
			{
				auto GetEntityID = weaponHotspot.GetEntityID();
				OffsetComponent& offset = OffsetComponents[GetEntityID];
				int characterEntityID = offset.GetObjectMappingEntityID();
				if (characterEntityID != -1)
				{
					TransformComponent& transform_component = TransformComponents[GetEntityID];
					WidthHeightComponent& WidthHeight_component = WidthHeightComponents[GetEntityID];
					ObjectStateComponent& object_state_component = ObjectStateComponents[GetEntityID];
					RectCollisionComponent& rectcollision = RectCollisionComponents[GetEntityID];


					TransformComponent& charactertransform = TransformComponents[characterEntityID];
					WidthHeightComponent& characterwidthheight = WidthHeightComponents[characterEntityID];
					ObjectStateComponent& characterobjectstate = ObjectStateComponents[characterEntityID];
					bool chararcterisfipped = characterobjectstate.GetIsFlipped();

					rectcollision.SetColliderBoxSize(WidthHeight_component.GetWidth<float>(), WidthHeight_component.GetWidth<float>());

					float CharacterCenterPositionX = charactertransform.GetPositionX() + characterwidthheight.GetWidth<float>() / 2.f;
					float ChararcterCenterPositionY = charactertransform.GetPositionY();

					const float weaponScaleXAbs = std::abs(transform_component.GetScaleX());
					if (chararcterisfipped == true)
					{
						object_state_component.setIsFlipped(true);
						offset.SetFlipped(true);
						transform_component.SetPosition(CharacterCenterPositionX - WidthHeight_component.GetWidth<float>(), ChararcterCenterPositionY + characterwidthheight.GetHeight<float>() / 2.5f);
						transform_component.SetScaleX(-weaponScaleXAbs);

						offset.SetOffsetPosition(glm::vec2{ -WidthHeight_component.GetWidth<float>() / transform_component.GetScaleX(), 0.f });
					}
					else
					{
						object_state_component.setIsFlipped(false);
						offset.SetFlipped(false);
						transform_component.SetPosition(CharacterCenterPositionX, ChararcterCenterPositionY + characterwidthheight.GetHeight<float>() / 2.5f);
						transform_component.SetScaleX(weaponScaleXAbs);
						offset.SetOffsetPosition(glm::vec2{ WidthHeight_component.GetWidth<float>() / transform_component.GetScaleX(), 0.f });;
					}
				}
			}

		}	
	}




	
}
