/*
    File Name: StarBGUpdateDrawSystem.cpp
    Project Name: Q
    Author(s):
        Primary: Yoonki Kim
        Secondary:
    All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#include "StarBGUpdateDrawSystem.h"
#include "../../Engine/Entity/CameraEntity.h"
#include "../../Engine/GameStateManager.h"
#include "../../Engine/Graphics/Window.h"
#include "Entity/UIEntities/UserFocusEntity.h"
#include "EObjectZDepth.h"

DrawObject Star::mDrawer = { "SquareMesh"_hashID, "DefaultColorShaderProgram"_hashID, {255, 255, 255, 180} };

void StarBGUpdateDrawSystem::Load()
{
    int counter = 0;
    for(auto& init : mInitializerVector)
    {
        GenerateStar(init, counter++);
    }
}

void StarBGUpdateDrawSystem::Update(double deltaTime)
{
    for(auto& list : mStarContainer)
    {
        const Rect boundingBox = list.first;
        for(auto& star : list.second)
        {
            star.Update(deltaTime);
            star.Draw(boundingBox);
        }
    }
}

void StarBGUpdateDrawSystem::Unload()
{
    for(auto& list : mStarContainer)
    {
        list.second.clear();
    }

    mStarContainer.clear();
}

void StarBGUpdateDrawSystem::GenerateStar(StarInitializer init, int where)
{
    RandomMachine& rand = Engine::GetEngineComponent<RandomMachine>();

    int remainingStars = init.second;

    const Rect boundingBox = init.first;

    mStarContainer[where].first = boundingBox;

    while(remainingStars > 0)
    {
        const glm::vec2 pos = { rand.GetRandomValue(boundingBox.Left(), boundingBox.Right()),
            rand.GetRandomValue(boundingBox.Bottom(), boundingBox.Top()) };

        const float size = rand.GetRandomValue(1.f, 3.f);


        mStarContainer[where].second.emplace_back(pos, size);


        --remainingStars;
    }
}

void Star::Update(double dt)
{
    const static float lerpRate = 0.2f;

    if(mState == State::INCREASING)
    {
        const float maxTargetSize = mInitSize * (1.f + maxChangingRate);

        mCurSize += (maxTargetSize - mCurSize) * lerpRate * static_cast<float>(dt);

        mCurAlphaVal += static_cast<int>((255 - mCurAlphaVal) * lerpRate * static_cast<float>(dt));

        if(mCurSize > maxTargetSize * 0.9f)
        {
            mState = State::DECREASING;
        }
    }
    else
    {
        const float minTargetSize = mInitSize;

        mCurSize += (minTargetSize - mCurSize) * lerpRate * static_cast<float>(dt);

        mCurAlphaVal -= static_cast<int>((mCurAlphaVal - 100)* lerpRate * static_cast<float>(dt));

        if (mCurSize < minTargetSize * 1.1f)
        {
            mState = State::INCREASING;
        }
    }
}

void Star::Draw(const Rect& boundingBox) const
{
    const glm::vec2 winSize = { get_window_size().first, get_window_size().second };

    const glm::mat3 winToNdc
    {
        2.f / winSize.x, 0, 0,
        0, 2.f / winSize.y , 0,
        -1, -1 , 1
    };

    const glm::mat3 sizeMat
    {
        mCurSize, 0.f, 0,
        0.f, mCurSize, 0,
        0.f, 0.f, 1.f
    };

    if(q_engine::Engine::GetEngineComponent<GameStateManager>().HasState(GameStateType::GAME))
    {
        static const glm::vec2 moveByPlayerRelativePosSize = boundingBox.Size() * 0.1f;

        glm::vec2 movedByPlayerRelativeStarPos = mPos;

        const glm::vec2 moveFactor = GetCamMapRelativePos() * moveByPlayerRelativePosSize;
        movedByPlayerRelativeStarPos -= moveFactor;

        if (movedByPlayerRelativeStarPos.x < boundingBox.Left())
        {
            movedByPlayerRelativeStarPos.x += boundingBox.Size().x;
        }
        else if (movedByPlayerRelativeStarPos.x > boundingBox.Right())
        {
            movedByPlayerRelativeStarPos.x -= boundingBox.Size().x;
        }
        if (movedByPlayerRelativeStarPos.y < boundingBox.Bottom())
        {
            movedByPlayerRelativeStarPos.y += boundingBox.Size().y;
        }
        else if (movedByPlayerRelativeStarPos.y > boundingBox.Top())
        {
            movedByPlayerRelativeStarPos.y -= boundingBox.Size().y;
        }

        const glm::mat3 posMat{
            1, 0, 0,
            0, 1, 0,
            movedByPlayerRelativeStarPos.x, movedByPlayerRelativeStarPos.y ,1
        };


        mDrawer.SetZDepth(static_cast<int>(EObjectZDepth::IN_GAME_PLAY_BG));
        mDrawer.SetColor(Color{ 255, 255, 255, mCurAlphaVal });
        mDrawer.Draw(winToNdc * (posMat * sizeMat));
    }
    if (Engine::GetEngineComponent<GameStateManager>().HasState(GameStateType::MAIN_MENU_BUTTON))
    {


        auto mainMenuState = Engine::GetEngineComponent<GameStateManager>().GetAllStates().find(GameStateType::MAIN_MENU_BUTTON)->second;

        glm::vec2 movedByPlayerRelativeStarPos = mPos -
       glm::vec2 {0.f,  mainMenuState->GetComponents<TransformComponent>()[mainMenuState->GetEntities<UserFocusEntity>().begin()->GetEntityID()].GetPositionY() * 1.f
    };

        if (movedByPlayerRelativeStarPos.x < boundingBox.Left())
        {
            movedByPlayerRelativeStarPos.x += boundingBox.Size().x;
        }
        else if (movedByPlayerRelativeStarPos.x > boundingBox.Right())
        {
            movedByPlayerRelativeStarPos.x -= boundingBox.Size().x;
        }
        if (movedByPlayerRelativeStarPos.y < boundingBox.Bottom())
        {
            movedByPlayerRelativeStarPos.y += boundingBox.Size().y;
        }
        else if (movedByPlayerRelativeStarPos.y > boundingBox.Top())
        {
            movedByPlayerRelativeStarPos.y -= boundingBox.Size().y;
        }

        const glm::mat3 posMat{
            1, 0, 0,
            0, 1, 0,
            movedByPlayerRelativeStarPos.x, movedByPlayerRelativeStarPos.y ,1
        };


        mDrawer.SetZDepth(static_cast<int>(EObjectZDepth::MAIN_MENU_BG));
        mDrawer.SetColor(Color{ 255, 255, 255, mCurAlphaVal });
        mDrawer.Draw(winToNdc * (posMat * sizeMat));
    }
   
}

glm::vec2 Star::GetCamMapRelativePos() const
{
    auto& gameStateMgr = Engine::GetEngineComponent<GameStateManager>();
    const glm::vec2 mapSize = gameStateMgr.GetMapSize();

    const glm::vec2 camPos = gameStateMgr.GetPlayState()->GetComponents<TransformComponent>().find(gameStateMgr.GetPlayState()->GetEntities<CameraEntity>().Front().GetEntityID())->GetPosition();
    
    return (camPos / mapSize) - glm::vec2{0.5f, 0.5f};
}
