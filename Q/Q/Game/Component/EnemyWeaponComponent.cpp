/*
    File Name: EnemyWeaponComponent.cpp
    Project Name: Q
    Author(s):
        Primary: Minjae Kyung
        Secondary: Hoseob Jeong
    All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#include "EnemyWeaponComponent.h"


#include "../../Engine/Component/RaycastComponent.h"
#include "../../Engine/System/TimerManagerSystem.h"
#include "Engine/Loader/Loader.h"
#include "Engine/Types.h"
EnemyWeaponComponent::EnemyWeaponComponent()
{
}

EnemyWeaponComponent::EnemyWeaponComponent(q_engine::IDType ownerEntity, glm::vec2 weaponOffset, int atkDamage, DamageType type) : mOwnerEntity(ownerEntity),
mWeaponOffset(weaponOffset), mAtkDamage(atkDamage), mDamageType(type)
{
}

EnemyWeaponComponent::EnemyWeaponComponent(q_engine::IDType ownerEntity, int atkDamage, bool isBullet, bool isFollowingOwner, DamageType type)
    : mOwnerEntity(ownerEntity), mAtkDamage(atkDamage), mIsBullet(isBullet), mIsFollowingOwner(isFollowingOwner), mDamageType(type)
{
}

EnemyWeaponComponent::~EnemyWeaponComponent()
{
}

Json::Value EnemyWeaponComponent::Serialize()
{
    Json::Value value;
    value["OwnerEntity"] = mOwnerEntity;
    value["WeaponOffset"].append(mWeaponOffset.x);
    value["WeaponOffset"].append(mWeaponOffset.y);
    value["AtkDamage"] = mAtkDamage;
    return  value;
}

void EnemyWeaponComponent::Deserialize(Json::Value& jsonValue)
{
    mOwnerEntity = q_engine::Loader::GetLoadedIDWithSavedID(jsonValue["OwnerEntity"].asInt());
    mWeaponOffset.x = jsonValue["WeaponOffset"][0].asFloat();
    mWeaponOffset.y = jsonValue["WeaponOffset"][1].asFloat();
    mAtkDamage = jsonValue["AtkDamage"].asInt();
}
