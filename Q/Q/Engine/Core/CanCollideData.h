/*
	File Name: CanCollideData.h
	Project Name: Q
	Author(s):
		Primary: Hoseob Jeong
		Secondary: Joonho Hwang
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/

#pragma once

#include <unordered_set>

#include "../../Game/Entity/BombEntity.h"
#include "../../Game/Entity/ItemEntity.h"
#include "../Game/Entity/PhysicsTestEntities.h"
#include "../Game/Entity/MapEntity.h"
#include "../Game/Entity/JumpTriggerEntity.h"
#include "../Game/Entity/WeaponEntity.h"
#include "Entity/BulletEntity.h"
#include "Entity/ProjectileEntity.h"

class CanCollideData
{
public:
	using EntityTypeID = int;
	template<typename Collider, typename Collidee>
	static void add(std::unordered_map<EntityTypeID,std::unordered_set<EntityTypeID>>& MapContainer)
	{
		MapContainer[q_engine::GetTypeID<Collider>()].emplace(q_engine::GetTypeID<Collidee>());
	}
	
	static const std::unordered_map<EntityTypeID, std::unordered_set<EntityTypeID>>& GetCanCollideData()
	{
		return canCollideData;
	}


	inline static const std::unordered_map<EntityTypeID, std::unordered_set<EntityTypeID>> canCollideData = []()
	{
		std::unordered_map<EntityTypeID, std::unordered_set<EntityTypeID>> CanCollisionData;
		
		add<PlayerEntity, GroundEntity>(CanCollisionData);
		add<PlayerEntity, q_engine::MapEntity>(CanCollisionData);
		add<PlayerEntity, EnemyEntity>(CanCollisionData);

		add<BallEntity, GroundEntity>(CanCollisionData);
		add<BallEntity, BallEntity>(CanCollisionData);
		
		add<EnemyEntity, GroundEntity>(CanCollisionData);
		add<EnemyEntity, q_engine::MapEntity>(CanCollisionData);

		add<WeaponEntity, EnemyEntity>(CanCollisionData);

		add<BombEntity, GroundEntity>(CanCollisionData);
		add<BombEntity, q_engine::MapEntity>(CanCollisionData);
		add<BombEntity, EnemyEntity>(CanCollisionData);

		add<ItemEntity, PlayerEntity>(CanCollisionData);
		add<CubeEntity, PlayerEntity>(CanCollisionData);

		add<JumpTriggerEntity, EnemyEntity>(CanCollisionData);
		add<q_engine::PortalEntity, PlayerEntity>(CanCollisionData);

		add<EnemyWeaponEntity, q_engine::MapEntity>(CanCollisionData);
		add<EnemyWeaponEntity, PlayerEntity>(CanCollisionData);

		add<q_engine::ProjectileEntity, q_engine::MapEntity>(CanCollisionData);
		add<q_engine::ProjectileEntity, EnemyEntity>(CanCollisionData);
		
		return CanCollisionData;
	}();

	
};
