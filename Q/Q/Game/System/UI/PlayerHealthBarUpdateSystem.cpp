/*
    File Name: PlayerHealthBarUpdateDrawSystem.cpp
    Project Name: Q
    Author(s):
        Primary: Yoonki Kim
        Secondary:
    All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#include "PlayerHealthBarUpdateSystem.h"
#include "../../../Engine/Component/WidthHeightComponent.h"
#include "../../../Engine/Engine.h"
#include "../../../Engine/Graphics/Window.h"
#include "../Engine/GameStateManager.h"
#include "Component/HealthComponent.h"
#include "Component/UIComponents/UIItemComponents.h"

#include "../Game/EObjectZDepth.h"
#include "Entity/UIEntities/InGameEntities.h"

void PlayerHealthBarUpdateDrawSystem::Load()
{
    using namespace q_engine;


}

void PlayerHealthBarUpdateDrawSystem::Update(double deltaTime)
{
    using namespace q_engine;

    static auto lerp = [](float start, float end, float lerpRate)
    {
        return start + (end - start) * lerpRate;
    };

    const IDType healthBarUI_ID = m_controlled_state->GetEntities<HealthBarUIEntity>().Front().GetEntityID();

    float playerHealthPercentage = Engine::GetEngineComponent<GameStateManager>().GetPlayState()->GetComponents<HealthComponent>()[Engine::GetEngineComponent<GameStateManager>().GetPlayerID()].AsPercentage();

    mPlayerHealthUIPercentage = lerp(mPlayerHealthUIPercentage, playerHealthPercentage, static_cast<float>(deltaTime) * 4);
    playerHealthPercentage = mPlayerHealthUIPercentage;

    auto& UIItemTextures = m_controlled_state->GetComponents<UIItemTextureComponent>()[healthBarUI_ID];

    const glm::vec2 UIposition = m_controlled_state->GetComponents<TransformComponent>()[healthBarUI_ID].GetPosition();
    const glm::vec2 UIsize = m_controlled_state->GetComponents<WidthHeightComponent>()[healthBarUI_ID].GetWidthHeight();

    //Updating Logic
    TransformComponent layerTransform;
    layerTransform.SetPosition(UIposition);
    Texture& layerTexture = AssetManager::GetTexture(UIItemTextures.GetTexture("Layer"));
    const glm::vec2 layerTextureSize{ layerTexture.GetWidth(), layerTexture.GetHeight() };
    glm::vec2 layerScale = UIsize / layerTextureSize;
    layerTransform.SetScale(layerScale);

  
    const glm::vec2 barTextureSize{ 62, 6 };
    TransformComponent healthBarTransform;
    healthBarTransform.SetPosition(UIposition);

    healthBarTransform.SetScale(UIsize / (barTextureSize / (glm::vec2{ 62, 6 } / layerTextureSize)));

    const static glm::vec2 barWorldPos = UIposition + UIsize * (glm::vec2{18, 3} / layerTextureSize);
    const glm::vec2 barWorldSize = UIsize * (glm::vec2{62, 6} / layerTextureSize);

    TransformComponent noneTransform;
    Texture& noneTexture = AssetManager::GetTexture(UIItemTextures.GetTexture("Empty"));
    const glm::vec2 noneTextureSize{ noneTexture.GetWidth(), noneTexture.GetHeight() };

    noneTransform.SetPosition(barWorldPos + glm::vec2{ barWorldSize.x, 0 });
    noneTransform.SetScale(barWorldSize * glm::vec2{  playerHealthPercentage - 1.f, 1.f } / noneTextureSize);

    std::vector<TransformComponent> transforms = { layerTransform , healthBarTransform , noneTransform };

    //Drawing Logic
    static DrawObject drawer( get_hash_id("SquareMesh"), get_hash_id("DefaultTextureShaderProgram") );

    const auto winSize = glm::vec2{ get_window_size().first, get_window_size().second };

    const glm::mat3 winToNdc
    {
        2.f / winSize.x, 0, 0,
        0, 2.f / winSize.y , 0,
        -1, -1 , 1
    };

    static std::vector<TextureID> mTextures = { UIItemTextures.GetTexture("Layer"), UIItemTextures.GetTexture("Bar"), UIItemTextures.GetTexture("Empty") };

    for(size_t i = 0; i < mTextures.size(); ++i)
    {
        const TextureID textureID = mTextures[i];
        const TransformComponent& transform = transforms[i];
        int zDepth = static_cast<int>(EObjectZDepth::IN_GAME_UI_THIRD_OBJ) - static_cast<int>(i);
        drawer.SetZDepth(zDepth);
        drawer.SetTextureID(textureID);
        drawer.Draw(winToNdc * transform.GetOrCalculateTransformMatrix());
    }
}

