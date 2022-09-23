#pragma once
/*
    File Name: StarBGUpdateDrawSystem.h
    Project Name: Q
    Author(s):
        Primary: Yoonki Kim
        Secondary:
    All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#pragma once
#include <glm/vec2.hpp>
#include <utility>
#include <vector>


#include "../../Engine/Core/RandomMachine.h"
#include "../../Engine/Core/Rect.h"
#include "../../Engine/ECS/System.h"
#include "../../Engine/Graphics/DrawObject.h"
#include "../../Engine/Core/StringHash.h"
#include "../../Engine/Engine.h"

using namespace q_engine;
class Star
{
    friend class StarBGUpdateDrawSystem;
public:
    Star(const glm::vec2& pos, const float& size) :
        mPos(pos), mInitSize(size), mCurSize(mInitSize)
    {
        RandomMachine& rand = Engine::GetEngineComponent<RandomMachine>();
        maxChangingRate = rand.GetRandomValue(0.2f, 1.5f);
        const int statePicker = rand.GetRandomValue(0, 1);
        mState = statePicker == 0 ? State::INCREASING : State::DECREASING;
        mCurAlphaVal = rand.GetRandomValue(0, 255);
    }

    void Update(double dt);
    void Draw(const Rect& boundingBox) const;

private:
    [[nodiscard]] glm::vec2 GetCamMapRelativePos() const;


    enum class State
    {
        INCREASING, DECREASING,
    };

    const glm::vec2 mPos;
    const float mInitSize;

    int mCurAlphaVal;

    float mCurSize;
    float maxChangingRate;
    State mState;
    static DrawObject mDrawer;
};

class StarBGUpdateDrawSystem final: public q_engine::System<StarBGUpdateDrawSystem>
{
    using BoundingBox = Rect;
    using StarContainer = std::list<Star>;

public:
    using StarInitializer = std::pair<BoundingBox, int>;


    StarBGUpdateDrawSystem(q_engine::GameState* state) : System<StarBGUpdateDrawSystem>(state) {};
    StarBGUpdateDrawSystem(q_engine::GameState* state, const std::initializer_list<StarInitializer>& list) : System<StarBGUpdateDrawSystem>(state), mInitializerVector(list)
    {
        mStarContainer.resize(list.size());
    };
    virtual void Load() override;
    virtual void Update(double deltaTime) override;
    virtual void Unload() override;


private:
    void GenerateStar(StarInitializer init, int where);

    std::vector<StarInitializer> mInitializerVector;

    std::vector<std::pair<BoundingBox, StarContainer>> mStarContainer;
};