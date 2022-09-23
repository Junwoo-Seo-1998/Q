/*
    File Name: EnemyWeaponComponent.h
    Project Name: Q
    Author(s):
        Primary: Minjae Kyung
        Secondary: Hoseob Jeong
    All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/

#pragma once
#include <glm/vec2.hpp>


#include "DamageComponent.h"
#include "../../Engine/ECS/Component.h"
#include "Engine/Types.h"



class EnemyWeaponComponent final : public q_engine::Component<EnemyWeaponComponent>
{
    bool mIsAttacking = true; //for bullets
    bool mIsBullet = false;
    bool mIsFollowingOwner = true;
	
    int mAtkDamage;
    DamageType mDamageType;
    glm::vec2 mWeaponOffset = glm::vec2(0,0);
    q_engine::IDType mOwnerEntity;
	
public:
    EnemyWeaponComponent(); //to be used by editor.
    EnemyWeaponComponent(q_engine::IDType ownerEntity, glm::vec2 weaponOffset, int atkDamage, DamageType type = DamageType::DIRECTDAMAGE);
    EnemyWeaponComponent(q_engine::IDType ownerEntity, int atkDamage, bool isBullet, bool isFollowingOwner, DamageType type = DamageType::DIRECTDAMAGE);
    ~EnemyWeaponComponent();

    EnemyWeaponComponent(const EnemyWeaponComponent& other) = default;
    EnemyWeaponComponent(EnemyWeaponComponent&& other) noexcept = default;

    EnemyWeaponComponent& operator=(const EnemyWeaponComponent& other) = default;
    EnemyWeaponComponent& operator=(EnemyWeaponComponent&& other) noexcept = default;


    [[nodiscard]] constexpr bool IsFollowingOwner() const noexcept { return mIsFollowingOwner; }
    [[nodiscard]] constexpr bool IsBullet() const noexcept { return mIsBullet; }
    [[nodiscard]] constexpr bool IsAttacking() const noexcept { return mIsAttacking; }
    void SetAttacking(bool attacking) { mIsAttacking = attacking; }
    DamageType GetDamageType() { return mDamageType; }
    int GetAttackDamage() { return mAtkDamage; }
    int GetOwnerEntityID() { return mOwnerEntity; }
    const glm::vec2& GetWeaponOffset() { return mWeaponOffset; }
    void SetWeaponOffset(const glm::vec2& offset) { mWeaponOffset = offset; }
    virtual Json::Value Serialize() override;
    virtual void Deserialize(Json::Value& jsonValue) override;
};