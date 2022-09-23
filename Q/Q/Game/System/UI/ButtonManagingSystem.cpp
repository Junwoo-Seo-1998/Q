/*
    File Name: ButtonManagingSystem.cpp
    Project Name: Q
    Author(s):
        Primary: Yoonki Kim
        Secondary:
    All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#include "ButtonManagingSystem.h"
#include "../Engine/GameState.h"
#include "Engine/Component/LuaScriptComponent.h"
#include "Engine/ECS/InputManager.h"
#include "Engine/Scripting/ScriptManager.h"
#include "Component/UIComponents/ButtonComponent.h"
#include "Engine/Core/Math.h"
#include "Entity/UIEntities/UserFocusEntity.h"
#include "Engine/AssetManager.h"
#include "EObjectZDepth.h"

void ButtonManagingSystem::Load()
{
    auto& transforms = m_controlled_state->GetComponents<q_engine::TransformComponent>();

    std::sort(m_buttonEntityIDs.begin(), m_buttonEntityIDs.end(), [&transforms](const auto& ent1, const auto& ent2)
    {
            glm::vec2 transform1 = transforms[ent1].GetPosition();
            glm::vec2 transform2 = transforms[ent2].GetPosition();

            return transform1.y > transform2.y;
    });

    if(!m_buttonEntityIDs.empty())
    {
        q_engine::LuaScriptComponent& script_component = m_controlled_state->GetComponents<q_engine::LuaScriptComponent>()[*m_buttonEntityIDs.begin()];

        script_component.GetLuaData()->set("IsFocused", true);


    }

    auto& scripts = m_controlled_state->GetComponents<q_engine::LuaScriptComponent>();
    for(auto& script : scripts)
    {
        ButtonComponent& button_component = m_controlled_state->GetComponents<ButtonComponent>()[script.GetEntityID()];
        std::string type = button_component.GetScript();
        if (script.GetLuaData()->get<std::string>("ButtonType") == "Slider")
        {
            script.GetLuaData()->set("Base", type);
            if (type == "SFX")
            {
                script.GetLuaData()->set("CurLevelAsFloat", q_engine::Engine::GetEngineComponent<q_engine::SoundComponent>().GetCurEffectVolume());
            }
            else if (type == "BG")
            {
                script.GetLuaData()->set("CurLevelAsFloat", q_engine::Engine::GetEngineComponent<q_engine::SoundComponent>().GetCurBGVolume());
            }
            else
            {
                script.GetLuaData()->set("CurLevelAsFloat", 0.5);
            }
        }
        else if (script.GetLuaData()->get<std::string>("ButtonType") == "CheckBox")
        {
            script.GetLuaData()->set("IsChecked", q_engine::Engine::GetIsFullScreen());
            if(q_engine::Engine::GetIsFullScreen())
            {
                script.GetLuaData()->set("CurrentCheckBoxTextureID", script.GetLuaData()->get<IDtype>("CheckedTexture"));
            }
            else
            {
                script.GetLuaData()->set("CurrentCheckBoxTextureID", script.GetLuaData()->get<IDtype>("UnCheckedTexture"));
            }
        }
    }
}
void ButtonManagingSystem::Update([[maybe_unused]] double deltaTime)
{
    auto& scripts = m_controlled_state->GetComponents<q_engine::LuaScriptComponent>();
    auto& buttons = m_controlled_state->GetComponents<ButtonComponent>();
    const int contSize = static_cast<int>(m_buttonEntityIDs.size());



    static std::list<std::pair<q_engine::TransformComponent, q_engine::TextureID>> additionalTextureToDraw;
    additionalTextureToDraw.clear();
    if(contSize == 1)
    {
        q_engine::LuaScriptComponent& script_component = scripts[*m_buttonEntityIDs.begin()];
        script_component.GetLuaData()->set("IsFocused", true);
        const bool is_focused = script_component.GetLuaData()->get<bool>("IsFocused");
        m_FocusedButtonIdx = 0;
        auto inputmanager = q_engine::Engine::GetEngineComponent<q_engine::InputManager>();

        if (q_engine::InputManager::IsKeyPressed(Key::SPACE) || q_engine::InputManager::IsKeyPressed(Key::ENTER))
        {
            q_engine::LuaScriptComponent& lua = *m_controlled_state->GetComponents<q_engine::LuaScriptComponent>().begin();
            lua.LoadLuaScript(buttons.begin()->GetScript());
            lua.OnInit();
            m_buttonEntityIDs.clear();
        }
    }
    else
    {
        for (auto itr = m_buttonEntityIDs.begin(); itr < m_buttonEntityIDs.end(); ++itr)
        {
            using namespace q_engine;
            q_engine::LuaScriptComponent& script_component = scripts[*itr];


            const bool is_focused = script_component.GetLuaData()->get<bool>("IsFocused");
            const std::string buttonTypeText = script_component.GetLuaData()->get<std::string>("ButtonType");

            if (buttonTypeText == "CheckBox")
            {
                q_engine::LuaScriptComponent& lua = m_controlled_state->GetComponents<q_engine::LuaScriptComponent>()[*itr];
                const q_engine::TextureID checkBoxTexID = lua.GetLuaData()->get<q_engine::TextureID>("CurrentCheckBoxTextureID");

                static const glm::vec2 checkBoxTextureSize = { AssetManager::GetTexture(checkBoxTexID).GetWidth(), AssetManager::GetTexture(checkBoxTexID).GetHeight() };

                const glm::vec2 checkBoxPos = lua.GetLuaData()->get<glm::vec2>("checkBoxPos");
                const glm::vec2 checkBoxSize = lua.GetLuaData()->get<glm::vec2>("checkBoxSize");

                const glm::vec2 texFitToSizeFactor = checkBoxSize / checkBoxTextureSize;

                TransformComponent texTransform;
                texTransform.SetPosition(checkBoxPos);
                texTransform.SetScale(texFitToSizeFactor);

                additionalTextureToDraw.emplace_back(std::pair{ texTransform, checkBoxTexID });
            }
            else if (buttonTypeText == "Slider")
            {
                {
                    q_engine::LuaScriptComponent& lua = m_controlled_state->GetComponents<q_engine::LuaScriptComponent>()[*itr];
                    const q_engine::TextureID sliderLayerTexID = lua.GetLuaData()->get<q_engine::TextureID>("SliderLayerTextureID");

                    static const glm::vec2 sliderLayerTextureSize = { AssetManager::GetTexture(sliderLayerTexID).GetWidth(), AssetManager::GetTexture(sliderLayerTexID).GetHeight() };

                    const glm::vec2 sliderLayerPos = lua.GetLuaData()->get<glm::vec2>("SliderLayerPos");
                    const glm::vec2 sliderLayerSize = lua.GetLuaData()->get<glm::vec2>("SliderLayerSize");

                    const glm::vec2 texFitToSizeFactor = sliderLayerSize / sliderLayerTextureSize;

                    TransformComponent texTransform;
                    texTransform.SetPosition(sliderLayerPos);
                    texTransform.SetScale(texFitToSizeFactor);

                    additionalTextureToDraw.emplace_back(std::pair{ texTransform, sliderLayerTexID });
                }
                {
                    q_engine::LuaScriptComponent& lua = m_controlled_state->GetComponents<q_engine::LuaScriptComponent>()[*itr];
                    const q_engine::TextureID slideTexID = lua.GetLuaData()->get<q_engine::TextureID>("SliderTextureID");

                    static const glm::vec2 slideTextureSize = { AssetManager::GetTexture(slideTexID).GetWidth(), AssetManager::GetTexture(slideTexID).GetHeight() };

                    const glm::vec2 sliderPos = lua.GetLuaData()->get<glm::vec2>("SliderPos");
                    const glm::vec2 sliderSize = lua.GetLuaData()->get<glm::vec2>("SliderSize");

                    const glm::vec2 texFitToSizeFactor = sliderSize / slideTextureSize;

                    TransformComponent texTransform;
                    texTransform.SetPosition(sliderPos);
                    texTransform.SetScale(texFitToSizeFactor);

                    additionalTextureToDraw.emplace_back(std::pair{ texTransform, slideTexID });
                }
            }

            if (is_focused)
            {

                m_FocusedButtonIdx = std::distance(m_buttonEntityIDs.begin(), itr);
                auto inputmanager = q_engine::Engine::GetEngineComponent<q_engine::InputManager>();
                if(q_engine::InputManager::IsKeyPressed(Key::DOWN_ARROW))
                {
                    q_engine::LuaScriptComponent& next_component = (itr + 1) >= m_buttonEntityIDs.end() ? scripts[*m_buttonEntityIDs.begin()] : scripts[*(itr + 1)];
                    script_component.GetLuaData()->set("IsFocused", false);
                    next_component.GetLuaData()->set("IsFocused", true);
                    break;
                }
                if(q_engine::InputManager::IsKeyPressed(Key::UP_ARROW))
                {
                    q_engine::LuaScriptComponent& prev_component = itr == m_buttonEntityIDs.begin() ? scripts[*(m_buttonEntityIDs.end() - 1)] : scripts[*(itr - 1)];
                    script_component.GetLuaData()->set("IsFocused", false);
                    prev_component.GetLuaData()->set("IsFocused", true);
                    break;
                }



                if (buttonTypeText == "Select")
                {
                    if (q_engine::InputManager::IsKeyPressed(Key::SPACE) || q_engine::InputManager::IsKeyPressed(Key::ENTER))
                    {
                        q_engine::LuaScriptComponent& lua = m_controlled_state->GetComponents<q_engine::LuaScriptComponent>()[*itr];
                        lua.LoadLuaScript(buttons[*itr].GetScript());
                        lua.OnInit();
                        m_buttonEntityIDs.clear();
                        break;
                    }
                }
                else if (buttonTypeText == "CheckBox")
                {
                    if (q_engine::InputManager::IsKeyPressed(Key::SPACE) || q_engine::InputManager::IsKeyPressed(Key::ENTER))
                    {
                        q_engine::LuaScriptComponent& lua = m_controlled_state->GetComponents<q_engine::LuaScriptComponent>()[*itr];
                        lua.GetLuaData()->set("IsChecked", !(lua.GetLuaData()->get<bool>("IsChecked")));
                        
                    }
                    q_engine::LuaScriptComponent& lua = m_controlled_state->GetComponents<q_engine::LuaScriptComponent>()[*itr];
                    if(lua.GetLuaData()->get<bool>("IsChecked") != lua.GetLuaData()->get<bool>("WasChecked"))
                    {
                        if(lua.GetLuaData()->get<std::string>("CheckBoxType") == "FullScreen")
                        {
                           
                            Engine::ToggleFullScreen();
                        }
                    }
                }
                else if (buttonTypeText == "Slider")
                {
                    auto& lua = m_controlled_state->GetComponents<q_engine::LuaScriptComponent>()[*itr];
                    float targetVolumeMove = 0.f;
                    const float moveDis = lua.GetLuaData()->get<float>("MoveDistance");
                    if (q_engine::InputManager::IsKeyPressed(Key::LEFT_ARROW)) {
                        targetVolumeMove -= moveDis;
                    }
                    if (q_engine::InputManager::IsKeyPressed(Key::RIGHT_ARROW)) {
                        targetVolumeMove += moveDis;
                    }

                    if(targetVolumeMove != 0.f)
                    {
                        if (lua.GetLuaData()->get<std::string>("Base") == "SFX")
                        {
                            float volume_cur = Engine::GetEngineComponent<SoundComponent>().GetCurEffectVolume();
                            float next_vol = std::clamp(volume_cur + targetVolumeMove, 0.f, 1.f);
                            Engine::GetEngineComponent<SoundComponent>().SetEffectVolume(next_vol);
                            lua.GetLuaData()->set("CurLevelAsFloat", next_vol);
                        }
                        else if (lua.GetLuaData()->get<std::string>("Base") == "BG")
                        {
                            float volume_cur = Engine::GetEngineComponent<SoundComponent>().GetCurBGVolume();
                            float next_vol = std::clamp(volume_cur + targetVolumeMove, 0.f, 1.f);
                            Engine::GetEngineComponent<SoundComponent>().SetBGVolume(next_vol);
                            lua.GetLuaData()->set("CurLevelAsFloat", next_vol);
                        }

                    }

                }
            }
        }
    }

   
    if(q_engine::Engine::GetEngineComponent<q_engine::GameStateManager>().HasState(GameStateType::MAIN_MENU_BUTTON) && 
        !q_engine::Engine::GetEngineComponent<q_engine::GameStateManager>().HasState(GameStateType::IN_GAME_MENU3))
    {
        auto& transforms = m_controlled_state->GetComponents<q_engine::TransformComponent>();

        auto& userFocusTransform = transforms[m_controlled_state->GetEntities<UserFocusEntity>().begin()->GetEntityID()];

        userFocusTransform.SetPosition(q_engine::smooth_step(userFocusTransform.GetPosition(), GetFocusedButtonPos(), 0.15f));
    }

    for(const auto& additionalTexture : additionalTextureToDraw)
    {
        static q_engine::DrawObject textureDrawer(q_engine::get_hash_id("SquareMesh"), q_engine::get_hash_id("DefaultTextureShaderProgram"));
        static const auto winSize = glm::vec2{q_engine::get_window_size().first, q_engine::get_window_size().second };
        static const glm::mat3 winToNdc
        {
            2.f / winSize.x, 0, 0,
            0, 2.f / winSize.y , 0,
            -1, -1 , 1
        };

        textureDrawer.SetZDepth(static_cast<int>(EObjectZDepth::IN_GAME_MENU_START));
        textureDrawer.SetTextureID(additionalTexture.second);
        textureDrawer.Draw(winToNdc* additionalTexture.first.GetOrCalculateTransformMatrix());
    }

}

void ButtonManagingSystem::Unload()
{
    m_buttonEntityIDs.clear();
    m_FocusedButtonIdx = -1;
}

glm::vec2 ButtonManagingSystem::GetFocusedButtonPos()
{
    return !m_buttonEntityIDs.empty() ? m_controlled_state->GetComponents<q_engine::TransformComponent>()[m_buttonEntityIDs[m_FocusedButtonIdx]].GetPosition() : glm::vec2{-1, -1};
}
    