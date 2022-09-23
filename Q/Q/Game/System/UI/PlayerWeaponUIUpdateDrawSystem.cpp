/*
    File Name: PlayerWeaponUIUpdateDrawSystem.cpp
    Project Name: Q
    Author(s):
        Primary: Yoonki Kim
        Secondary:
    All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#include "PlayerWeaponUIUpdateDrawSystem.h"

#include "../../../Engine/Component/WidthHeightComponent.h"
#include "../../../Engine/Engine.h"
#include "../../../Engine/Graphics/Window.h"
#include "../Engine/GameState.h"
#include "Component/UIComponents/UIItemComponents.h"
#include "Entity/UIEntities/InGameEntities.h"
#include "../Engine/GameStateManager.h"
#include "Component/NewPlayerInventorySlots.h"
#include "Component/NewWeaponComponent.h"
#include "Engine/Entity/CameraEntity.h"

#include "EObjectZDepth.h"

void PlayerWeaponUIUpdateDrawSystem::Load()
{
    using namespace q_engine;
}

void PlayerWeaponUIUpdateDrawSystem::Update(double deltaTime)
{
    using namespace q_engine;

    const auto& game_state_mgr = Engine::GetEngineComponent<GameStateManager>();
    const auto playState = game_state_mgr.GetPlayState();
    auto player_ID = game_state_mgr.GetPlayerID();

    const auto& player_inven = playState->GetComponents<NewPlayerInventorySlots>()[player_ID];

    const std::string firstWeaponName = player_inven.weaponSlot[player_inven.weapon_cur];
    const std::string secondWeaponName = player_inven.weaponSlot[1 - player_inven.weapon_cur];

    const IDType weaponBox_ID = m_controlled_state->GetEntities<WeaponBoxEntity>().Front().GetEntityID();
    auto& UIItemTextures = m_controlled_state->GetComponents<UIItemTextureComponent>()[weaponBox_ID];


    const glm::vec2 UIposition = m_controlled_state->GetComponents<TransformComponent>()[weaponBox_ID].GetPosition();
    const glm::vec2 UIsize = m_controlled_state->GetComponents<WidthHeightComponent>()[weaponBox_ID].GetWidthHeight();
    const glm::vec2 iconSize = UIsize * 0.7f;

    const auto winSize = glm::vec2{ get_window_size().first, get_window_size().second };

    static DrawObject drawer(get_hash_id("SquareMesh"), get_hash_id("DefaultTextureShaderProgram"));
    static DrawObject drawer_Color(get_hash_id("SquareMesh"), get_hash_id("DefaultColorShaderProgram"));

    const glm::mat3 winToNdc
    {
        2.f / winSize.x, 0, 0,
        0, 2.f / winSize.y , 0,
        -1, -1 , 1
    };

    //Major Weapon Update & Draw Logic
    {
        //Update Logic
        std::vector<TextureID> textureIDs(2);
        std::vector<TransformComponent> transforms(textureIDs.size());

        TransformComponent boxTransform;
        boxTransform.SetPosition(UIposition);

        Texture& boxTexture = AssetManager::GetTexture(UIItemTextures.GetTexture("Box"));
        const glm::vec2 boxTextureSize{ boxTexture.GetWidth(), boxTexture.GetHeight() };
        boxTransform.SetScale(UIsize / boxTextureSize);

        textureIDs[0] = UIItemTextures.GetTexture("Box");
        transforms[0] = boxTransform;

        textureIDs[1] = UIItemTextures.GetTexture(firstWeaponName);

        TransformComponent weaponIconTransform;
        Texture& weaponIconTexture = AssetManager::GetTexture(textureIDs[1]);
        weaponIconTransform.SetPosition(UIposition + (UIsize - iconSize) * 0.5f);
        const glm::vec2 weaponIconSize{ weaponIconTexture.GetWidth(), weaponIconTexture.GetHeight() };
        weaponIconTransform.SetScale(UIsize / weaponIconSize * (iconSize / UIsize));

        transforms[1] = weaponIconTransform;
        //Drawing Logic

        const size_t numOfDrawing = transforms.size();

        for (size_t i = 0; i < numOfDrawing; ++i)
        {
            const TextureID textureID = textureIDs[i];
            const TransformComponent& transform = transforms[i];
            int zDepth = static_cast<int>(EObjectZDepth::IN_GAME_UI_FIRST_OBJ) + static_cast<int>(i);
            drawer.SetZDepth(zDepth);
            drawer.SetTextureID(textureID);
            drawer.Draw(winToNdc * transform.GetOrCalculateTransformMatrix());
        }
    }

    //Holster Weapon Update & Draw Logic
    {
        //Update Logic
        std::vector<TextureID> textureIDs(2);
        std::vector<TransformComponent> transforms(textureIDs.size());

        constexpr float majorAndMinorRatio = 0.65f;

        const glm::vec2 minorUIPosition = UIposition + glm::vec2{ UIsize.x * 1.2f, 0 };
        const glm::vec2 minorUISize = UIsize * majorAndMinorRatio;
        const glm::vec2 minorIconSize = iconSize * majorAndMinorRatio;

        TransformComponent boxTransform;
        boxTransform.SetPosition(minorUIPosition);

        Texture& boxTexture = AssetManager::GetTexture(UIItemTextures.GetTexture("Box"));
        const glm::vec2 boxTextureSize{ boxTexture.GetWidth(), boxTexture.GetHeight() };
        boxTransform.SetScale(minorUISize / boxTextureSize);

        textureIDs[0] = UIItemTextures.GetTexture("Box");
        transforms[0] = boxTransform;

        textureIDs[1] = UIItemTextures.GetTexture(secondWeaponName);

        TransformComponent weaponIconTransform;
        Texture& weaponIconTexture = AssetManager::GetTexture(textureIDs[1]);
        weaponIconTransform.SetPosition(minorUIPosition + (minorUISize - minorIconSize) * 0.5f);
        const glm::vec2 weaponIconSize{ weaponIconTexture.GetWidth(), weaponIconTexture.GetHeight() };
        weaponIconTransform.SetScale(minorUISize / weaponIconSize * (minorIconSize / minorUISize));

        transforms[1] = weaponIconTransform;
        //Drawing Logic

        const size_t numOfDrawing = transforms.size();

        for (size_t i = 0; i < numOfDrawing; ++i)
        {
            const TextureID textureID = textureIDs[i];
            const TransformComponent& transform = transforms[i];
            int zDepth = static_cast<int>(EObjectZDepth::IN_GAME_UI_FIRST_OBJ) + static_cast<int>(i);
            drawer.SetZDepth(zDepth);
            drawer.SetTextureID(textureID);
            drawer.Draw(winToNdc * transform.GetOrCalculateTransformMatrix());
        }
    }

    //Ammo Logic
    NewWeaponComponent& first_weapon = playState->GetComponents<NewWeaponComponent>().Front();
    if(first_weapon.IsRangedWeapon())
    {
        int maxAmmo = first_weapon.GetMaxAmmo();
        int curAmmo = first_weapon.GetCurrentAmmo();

        const float loadFactor = static_cast<float>(curAmmo) / maxAmmo;

        const int rowMaxAmmo = maxAmmo < 5 ? maxAmmo : 5;

        const auto playerID = Engine::GetEngineComponent<GameStateManager>().GetPlayerID();
        auto& playStateTransforms = playState->GetComponents<TransformComponent>();
        const glm::vec2 playerTransform = playStateTransforms[playerID].GetPosition();
        const glm::vec2 camTransform = playStateTransforms[playState->GetEntities<CameraEntity>().begin()->GetEntityID()].GetPosition();

        glm::vec2 playerWinRelativePos = playerTransform - camTransform + winSize * 0.5f;
        glm::vec2 playerSize = playState->GetComponents<WidthHeightComponent>()[playerID].GetWidthHeight();

        constexpr TextureID textureID = "Bullet"_hashID;


        const glm::vec2 bulletUIStartPos = playerWinRelativePos + glm::vec2{ 0, playerSize.y };

        const float bulletAndSpaceTargetSizeX = playerSize.x / rowMaxAmmo;
        const glm::vec2 bulletAndSpaceTargetSize = { bulletAndSpaceTargetSizeX , bulletAndSpaceTargetSizeX * 2 };
        const glm::vec2 bulletOffset = bulletAndSpaceTargetSize * glm::vec2{ 0.1f, 0.1f };

        const glm::vec2 bulletTargetSize = bulletAndSpaceTargetSize - bulletOffset * 2.f;

        //height width is transposed
        const glm::vec2 curTextureSize = { AssetManager::GetTexture(textureID).GetHeight(), AssetManager::GetTexture(textureID).GetWidth() };

        const glm::vec2 scaleFactor = bulletTargetSize / curTextureSize;

        TransformComponent curTransform;
        curTransform.SetPosition(bulletUIStartPos);
        curTransform.SetScale(bulletTargetSize/* * 0.8f*/);
        Color bulletUIColor = Color{ (1.f - loadFactor), loadFactor, 0.f, 1.f };
        for (int i = 1; i <= curAmmo; ++i)
        {
            glm::vec2 curPos = curTransform.GetPosition();

            TransformComponent transform = curTransform;
            transform.AddPosition(bulletOffset);
            int zDepth = static_cast<int>(EObjectZDepth::IN_GAME_UI_FIRST_OBJ);

            drawer_Color.SetZDepth(zDepth);
            drawer_Color.SetColor(bulletUIColor);
            drawer_Color.Draw(winToNdc* transform.GetOrCalculateTransformMatrix());
            if(i % (rowMaxAmmo)  == 0)
            {
                curTransform.SetPositionX(bulletUIStartPos.x);
                curTransform.AddPosition(0.f, bulletAndSpaceTargetSize.y);
            }
            else
            {
                curTransform.AddPosition(bulletAndSpaceTargetSize.x, 0.f);
            }
        }




    }

}
