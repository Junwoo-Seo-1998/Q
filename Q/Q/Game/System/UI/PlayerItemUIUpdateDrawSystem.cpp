/*
    File Name: PlayeritemUIUpdateDrawSystem.cpp
    Project Name: Q
    Author(s):
        Primary: Yoonki Kim
        Secondary:
    All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#include "PlayerItemUIUpdateDrawSystem.h"

#include "../Engine/GameStateManager.h"
#include "Component/NewItemComponent.h"
#include "Component/UIComponents/UIItemComponents.h"
#include "Engine/Component/TextComponent.h"
#include "Engine/Component/WidthHeightComponent.h"
#include "Engine/Core/Timer.h"
#include "Engine/Graphics/Window.h"
#include "Engine/System/TimerManagerSystem.h"
#include "Entity/UIEntities/InGameEntities.h"
#include "EObjectZDepth.h"
#include "Component/NewPlayerInventorySlots.h"

PlayeritemUIUpdateDrawSystem::PlayeritemUIUpdateDrawSystem(q_engine::GameState* state)
    : System<PlayeritemUIUpdateDrawSystem>(state)
{
    static float itemUseInteractionTime = 2.f;
    mFirstItemUseInteractionTimer.first = false;
    mSecondItemUseInteractionTimer.first = false;

    auto& timerManager = m_controlled_state->GetUpdateSystem<q_engine::TimerManagerSystem>();
    mFirstItemUseInteractionTimer.second = timerManager.IssueTimer(itemUseInteractionTime);
    mSecondItemUseInteractionTimer.second = timerManager.IssueTimer(itemUseInteractionTime);
}

void PlayeritemUIUpdateDrawSystem::Load()
{
    using namespace q_engine;

}

void PlayeritemUIUpdateDrawSystem::Update([[maybe_unused]] double deltaTime)
{
    using namespace q_engine;

    UpdateItemAmountText();

    const IDType itemBox_ID = m_controlled_state->GetEntities<ItemBoxEntity>().Front().GetEntityID();
    auto& UIItemTextures = m_controlled_state->GetComponents<UIItemTextureComponent>()[itemBox_ID];

    const auto& game_state_mgr = Engine::GetEngineComponent<GameStateManager>();
    const auto playState = game_state_mgr.GetPlayState();
    auto player_ID = game_state_mgr.GetPlayerID();

    const auto& player_inventory = playState->GetComponents<NewPlayerInventorySlots>()[player_ID];

    const auto& player_item_container = player_inventory.ItemSlot;

    glm::vec2 UIposition = m_controlled_state->GetComponents<TransformComponent>()[itemBox_ID].GetPosition();
    glm::vec2 UIsize = m_controlled_state->GetComponents<WidthHeightComponent>()[itemBox_ID].GetWidthHeight();
    glm::vec2 iconSize = UIsize * 0.7f;

    const auto winSize = glm::vec2{ get_window_size().first, get_window_size().second };

    static DrawObject textureDrawer(get_hash_id("SquareMesh"), get_hash_id("DefaultTextureShaderProgram"));
    static DrawObject filledRectDrawer(get_hash_id("SquareMesh"), get_hash_id("DefaultColorShaderProgram"));
    static DrawObject hollowRectDrawer(get_hash_id("HollowSquareMesh"), get_hash_id("DefaultColorShaderProgram"));
    const glm::mat3 winToNdc
    {
        2.f / winSize.x, 0, 0,
        0, 2.f / winSize.y , 0,
        -1, -1 , 1
    };

    static Color interactionColor = { 0.f, 1.f, 1.f, 1.f };
    constexpr Color itemUICoolDownColor = { 0.8f, 0.3f, 0.6f, 1.f };
    //First Item Update & Draw Logic
    {
        const std::string cur_major_item_name = player_item_container[player_inventory.item_cur];
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

        textureIDs[1] = UIItemTextures.GetTexture(cur_major_item_name);

        TransformComponent itemIconTransform;
        Texture* itemIconTexture = textureIDs[1] != 0 ? &AssetManager::GetTexture(textureIDs[1]) : nullptr;
        itemIconTransform.SetPosition(UIposition + (UIsize - iconSize) * 0.5f);
        const glm::vec2 itemIconSize = itemIconTexture != nullptr ? glm::vec2{ itemIconTexture->GetWidth(), itemIconTexture->GetHeight() } : glm::vec2{ 0, 0 };
        itemIconTransform.SetScale(UIsize / itemIconSize * (iconSize / UIsize));

        transforms[1] = itemIconTransform;

        {
            //ItemUsingInteractionLogic
            const Timer& timer = TimerManagerSystem::GetTimer(mFirstItemUseInteractionTimer.second);
            if (!timer.IsPaused())
            {
                TransformComponent interactionBoxTransform;
                interactionBoxTransform.SetPosition(boxTransform.GetPosition());
                interactionBoxTransform.SetScale(UIsize * 0.98f);

                const float remainingTimeAsPercentage = timer.GetRemainingTime() / timer.GetOriginalTime();

                Color drawColor = interactionColor;

                drawColor.a = remainingTimeAsPercentage;
                hollowRectDrawer.SetLineWidth(5.f);
                hollowRectDrawer.SetZDepth(static_cast<int>(EObjectZDepth::IN_GAME_UI_FOURTH_OBJ));
                hollowRectDrawer.SetColor(drawColor);
                hollowRectDrawer.Draw(winToNdc * interactionBoxTransform.GetOrCalculateTransformMatrix());
            }
        }
        //Drawing Logic
        const size_t numOfDrawing = transforms.size();

        for (size_t i = 0; i < numOfDrawing; ++i)
        {
            const TextureID textureID = textureIDs[i];
            if (textureID != 0)
            {
                const TransformComponent& transform = transforms[i];
                int zDepth = static_cast<int>(EObjectZDepth::IN_GAME_UI_FIRST_OBJ) + static_cast<int>(i);
                textureDrawer.SetZDepth(zDepth);
                textureDrawer.SetTextureID(textureID);
                textureDrawer.Draw(winToNdc * transform.GetOrCalculateTransformMatrix());
            }
        }

    }
    //Second Item Update & Draw Logic
    {
        UIposition.x += UIsize.x * 1.3f;
        UIsize = UIsize * 0.7f;
        iconSize = UIsize * 0.7f;

        //Update Logic
        const std::string second_item_name = player_item_container[player_inventory.item_cur == 0 ? 1 : 0];
           
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

        textureIDs[1] = UIItemTextures.GetTexture(second_item_name);

        TransformComponent itemIconTransform;
        Texture* itemIconTexture = textureIDs[1] != 0 ? &AssetManager::GetTexture(textureIDs[1]) : nullptr;
        itemIconTransform.SetPosition(UIposition + (UIsize - iconSize) * 0.5f);
        const glm::vec2 itemIconSize = itemIconTexture != nullptr ? glm::vec2{ itemIconTexture->GetWidth(), itemIconTexture->GetHeight() } : glm::vec2{ 0, 0 };
        itemIconTransform.SetScale(UIsize / itemIconSize * (iconSize / UIsize));

        transforms[1] = itemIconTransform;
       
        {
            {
                const Timer& timer = TimerManagerSystem::GetTimer(mSecondItemUseInteractionTimer.second);
                if (!timer.IsPaused())
                {
                    TransformComponent interactionBoxTransform;
                    interactionBoxTransform.SetPosition(boxTransform.GetPosition());
                    interactionBoxTransform.SetScale(UIsize * 0.98f);

                    const float remainingTimeAsPercentage = timer.GetRemainingTime() / timer.GetOriginalTime();

                    Color drawColor = interactionColor;

                    drawColor.a = remainingTimeAsPercentage;
                    hollowRectDrawer.SetLineWidth(5.f);
                    hollowRectDrawer.SetZDepth(static_cast<int>(EObjectZDepth::IN_GAME_UI_FOURTH_OBJ));
                    hollowRectDrawer.SetColor(drawColor);
                    hollowRectDrawer.Draw(winToNdc * interactionBoxTransform.GetOrCalculateTransformMatrix());
                }
            }
        }
        //Drawing Logic

        const size_t numOfDrawing = transforms.size();

        for (size_t i = 0; i < numOfDrawing; ++i)
        {
            const TextureID textureID = textureIDs[i];
            if (textureID != 0)
            {
                const TransformComponent& transform = transforms[i];
                int zDepth = static_cast<int>(EObjectZDepth::IN_GAME_UI_FIRST_OBJ) + static_cast<int>(i);
                textureDrawer.SetZDepth(zDepth);
                textureDrawer.SetTextureID(textureID);
                textureDrawer.Draw(winToNdc * transform.GetOrCalculateTransformMatrix());
            }
        }
    }

    UIsize = m_controlled_state->GetComponents<WidthHeightComponent>()[itemBox_ID].GetWidthHeight();
    TransformComponent cubeTransform;
    cubeTransform.SetPosition(UIposition - glm::vec2{ UIsize.x * 5.5f, 0.f });
    const TextureID cubeTexID = UIItemTextures.GetTexture("Cube");
    static const glm::vec2 cubeTextureSizeFactor = UIsize / glm::vec2{ AssetManager::GetTexture(cubeTexID).GetWidth(), AssetManager::GetTexture(cubeTexID).GetHeight() };
    cubeTransform.SetScale(cubeTextureSizeFactor);
    const TransformComponent& transform = cubeTransform;
    int zDepth = static_cast<int>(EObjectZDepth::IN_GAME_UI_FIRST_OBJ);
    textureDrawer.SetZDepth(zDepth);
    textureDrawer.SetTextureID(cubeTexID);
    textureDrawer.Draw(winToNdc* transform.GetOrCalculateTransformMatrix());
    
}

void PlayeritemUIUpdateDrawSystem::Unload()
{

}

void PlayeritemUIUpdateDrawSystem::ResolveItemInteractionEventFromItemEntity(q_engine::IDType entityID)
{
    if(entityID == mFirstItemEntityID)
    {
        mFirstItemUseInteractionTimer.first = !mFirstItemUseInteractionTimer.first;
        q_engine::Timer& timer = q_engine::TimerManagerSystem::GetTimer(mFirstItemUseInteractionTimer.second);
        timer.Reset();
        timer.Resume();
    }
    else if(entityID == mSecondItemEntityID)
    {
        mSecondItemUseInteractionTimer.first = !mSecondItemUseInteractionTimer.first;
        q_engine::Timer& timer = q_engine::TimerManagerSystem::GetTimer(mSecondItemUseInteractionTimer.second);
        timer.Reset();
        timer.Resume();
    }
    else
    {
        //Can't be happened
    }
}

void PlayeritemUIUpdateDrawSystem::ResolveItemUsedEvent([[maybe_unused]]const std::string& item_name)
{

  
        mFirstItemUseInteractionTimer.first = !mFirstItemUseInteractionTimer.first;
        q_engine::Timer& timer = q_engine::TimerManagerSystem::GetTimer(mFirstItemUseInteractionTimer.second);
        timer.Reset();
        timer.Resume();
 
}

void PlayeritemUIUpdateDrawSystem::UpdateItemAmountText()
{
    auto& itemSlot = q_engine::Engine::GetEngineComponent<q_engine::GameStateManager>().GetPlayState()->GetComponents<NewPlayerInventorySlots>().Front();
    const int major_left = itemSlot.GetCurrentItemCount();
    const int minor_left = itemSlot.GetSlotItemCount();
    const int cube_amount = itemSlot.GetCubeAmount();

    auto& tags = m_controlled_state->GetComponents<TagComponent>();

    unsigned major_ent_loc = 0;
    unsigned minor_ent_loc = 0;
    unsigned cube_ent_loc = 0;

    int loc = 0;
    for(auto itr = tags.begin(); itr != tags.end(); ++itr)
    {
         if(itr->Tag == "MajorItemText")
         {
             major_ent_loc = loc;
         }
         else if(itr->Tag == "MinorItemText")
         {
             minor_ent_loc = loc;
         }
         else if(itr->Tag == "CubeAmountText")
         {
             cube_ent_loc = loc;
         }
         loc++;
    }

    const q_engine::IDType major_entID = (tags.begin() + major_ent_loc)->GetEntityID();

    auto& texts = m_controlled_state->GetComponents<q_engine::TextComponent>();

    texts[major_entID].SetText(major_left != 0 ? std::to_string(major_left) : "");

    const q_engine::IDType minor_entID = (tags.begin() + minor_ent_loc)->GetEntityID();
    texts[minor_entID].SetText(minor_left != 0 ? std::to_string(minor_left) : "");

    const q_engine::IDType cube_entID = (tags.begin() + cube_ent_loc)->GetEntityID();
    texts[cube_entID].SetText(std::to_string(cube_amount) + " of " +std::to_string(NewPlayerInventorySlots::GetMaxCubeAmount()));
}
