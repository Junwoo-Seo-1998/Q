/*
    File Name: FXAnimLuaScriptsComponent.cpp
    Project Name: Q
    Author(s):
        Primary: Yoonki Kim
        Secondary:
    All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#include "FXAnimLuaScriptsComponent.h"
#include "Engine/Component/FollowComponent.h"
#include "Engine/Component/OffsetComponent.h"
#include "Engine/Engine.h"
#include "Engine/GameStateManager.h"
#include "Entity/SimpleEntity.h"
#include "System/FXAnimLuaScriptsUpdateSystem.h"

void FXAnimLuaScriptsComponent::Update(double dt)
{
    using q_engine::IDType;
    std::vector<unsigned> deleting;

    auto& offsetComps = q_engine::Engine::GetEngineComponent<q_engine::GameStateManager>().GetPlayState()->GetComponents<q_engine::OffsetComponent>();

    for(decltype(mRunningScripts)::reverse_iterator itr = mRunningScripts.rbegin(); itr != mRunningScripts.rend(); ++itr)
    {
        auto& script = *itr;
        script.second.OnUpdate(static_cast<float>(dt));

        if(script.second.GetLuaData()->get<bool>("FollowFlip"))
        {
            const IDType followID = script.second.GetLuaData()->get<IDType>("MyID");
            const IDType followingID = script.second.GetLuaData()->get<IDType>("BaseEntID");

            offsetComps[followingID].SetFlipped(offsetComps[followID].GetIsFlipped());
        }

        if (script.second.GetLuaData()->get<bool>("DoDestroy"))
        {
            q_engine::Engine::GetEngineComponent<q_engine::GameStateManager>().GetPlayState()->
                DeleteEntityComponentsLater(script.first.typeID, script.first.entID);
            deleting.push_back(abs(std::distance(itr, mRunningScripts.rend())) - 1);
        }
    }
    for(auto deletingPos : deleting)
    {
        mRunningScripts.erase(mRunningScripts.begin() + deletingPos);
    }
}

void FXAnimLuaScriptsComponent::Unload()
{
    if (!mRunningScripts.empty())
    {
        auto playState = q_engine::Engine::GetEngineComponent<q_engine::GameStateManager>().GetPlayState();
        auto& masterSystem = playState->GetUpdateSystem<FXAnimLuaScriptsUpdateSystem>();

        for (auto& script : mRunningScripts)
        {
            playState->DeleteComponent<q_engine::FollowComponent>(script.first.entID);
            masterSystem.Adopt(std::move(script.first), std::move(script.second));
        }
    }
}

void FXAnimLuaScriptsComponent::OnComponentCreation()
{
    if(this->GetEntityTypeID() == q_engine::GetTypeID<PlayerEntity>())
    {
        LoadPlayerAnims();
    }
    else
    {
        LoadEnemyAnims();
    }
}

void FXAnimLuaScriptsComponent::LoadPlayerAnims()
{
    mInitializers.clear();
    LoadCommonAnims();
    mInitializers.emplace("Shield", "resource/lua/FX/Shield.lua");
}

void FXAnimLuaScriptsComponent::LoadEnemyAnims()
{
    mInitializers.clear();
    LoadCommonAnims();
}

void FXAnimLuaScriptsComponent::RunScript(std::string mName)
{
    mRunningScripts.emplace_back(RunInnerScriptFunc(mInitializers[mName]));
}

void FXAnimLuaScriptsComponent::LoadCommonAnims()
{
    mInitializers.emplace("HitBy", "resource/lua/FX/OnHitExplode.lua");
    mInitializers.emplace("Poisoned", "resource/lua/FX/Poisoned.lua");
    mInitializers.emplace("Flamed", "resource/lua/FX/Flamed.lua");
    mInitializers.emplace("DeadExplode", "resource/lua/FX/DeadExplode.lua");
}

std::pair<FXAnimLuaScriptsComponent::EntInfo, InnerLuaScriptComponent> FXAnimLuaScriptsComponent::RunInnerScriptFunc(const std::string& scriptPath) const
{
    auto simple = q_engine::Engine::GetEngineComponent<q_engine::GameStateManager>().GetPlayState()->AddEntity<SimpleEntity>();
    InnerLuaScriptComponent lua;
    EntInfo animEntInfo;

    lua.InitLua(scriptPath, GetEntityID());
    animEntInfo.typeID = lua.GetLuaData()->get<unsigned>("BaseEntTypeID");
    animEntInfo.entID = lua.GetLuaData()->get<unsigned>("BaseEntID");

    return std::pair{ animEntInfo, lua };
}

void RegisterFXAnimLuaScriptsComponent(sol::state& state)
{
}
