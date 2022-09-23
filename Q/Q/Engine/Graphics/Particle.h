/*
    File Name: Particle.h
    Project Name: Q
    Author(s):
        Primary: Yoonki Kim
        Secondary:
    All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#pragma once
#include <glm/vec2.hpp>



#include "../Component/GravityComponent.h"
#include "../Component/LifeTimeComponent.h"
#include "../Component/TextureComponent.h"
#include "../ECS/Entity.h"
#include "../Engine/Engine.h"
#include "../Engine/Core/RandomMachine.h"
#include "../Engine/GameStateManager.h"

#include "../Engine/Component/VelocityComponent.h"
#include "../Engine/Entity/CameraEntity.h"
#include "../Engine/Component/CameraShakeComponent.h"

#include "DrawObject.h"

namespace q_engine
{
    class IParticle : public Entity<IParticle>
    {
        friend class IParticleEmitter;
        friend class RectMeshParticleEmitter;
    private:
        void Update(double dt)
        {
            Engine::GetEngineComponent<GameStateManager>().GetPlayState()->GetComponents<LifeTimeComponent>().find(GetEntityID())->Update(dt);
        }
        [[nodiscard]] bool IsDead() const
        {
            return Engine::GetEngineComponent<GameStateManager>().GetPlayState()->GetComponents<LifeTimeComponent>().find(GetEntityID())->IsDead();
        }
    protected:
        IParticle() = default;
    };

    class RectMeshParticle : public IParticle
    {
    public:
        RectMeshParticle() : IParticle() {}
    };

    class IParticleEmitter : public Entity<IParticleEmitter>
    {
    public:
        virtual void Update(double dt);

        virtual void Draw() const = 0;

        [[nodiscard]] std::string GetName() const
        {
            return mName;
        }

        virtual void Emit(unsigned num_particles, glm::vec2 emitPos, glm::vec2 emitVel, glm::vec2 minVel, glm::vec2 maxVel, glm::vec2 size, glm::vec2 lifeTime, float gravityConstant) = 0;

    protected:
        IParticleEmitter(std::string name, int size) : mName(name), maxSize(size)
        {}
    protected:
        std::list<std::shared_ptr<IParticle>> mControllingParticles;
        std::list<std::shared_ptr<IParticle>> mDeletingQueParticles;

    private:
        std::string mName;

    protected:
        unsigned maxSize;
    };

    class RectMeshParticleEmitter : public IParticleEmitter
    {
    public:
        RectMeshParticleEmitter(std::string name, int maxSize, float particleSize, Color color)
        : IParticleEmitter(name, maxSize), mSize(particleSize), mDrawer("SquareMesh"_hashID, "DefaultColorShaderProgram"_hashID, color)
        {}

        void Draw() const override;

        virtual void Emit(unsigned num_particles, glm::vec2 emitPos, glm::vec2 emitVel, glm::vec2 minVel, glm::vec2 maxVel, glm::vec2 size, glm::vec2 lifeTime, float gravityConstant) override;

    private:
        float mSize;
        mutable DrawObject mDrawer;
    };

  
    class TextureParticleEmitter : public IParticleEmitter
    {
    private:
        Texture mHandlingTexture;
    };

    class ParticleSystem : public System<ParticleSystem>
    {
    public:
        ParticleSystem() = delete;
        ParticleSystem(GameState* state) : System<ParticleSystem>(state) {};
        virtual void Load() override
        {

        }
        virtual void Update(double deltaTime) override
        {
            for(auto& emitter : mEmitters)
            {
                emitter->Update(deltaTime);
                emitter->Draw();
            }
        }
        virtual void Unload() override
        {
            mEmitters.clear();
        }

        void RegisterEmitter(std::shared_ptr<IParticleEmitter> emitter)
        {
            mEmitters.emplace_back(emitter);
        }

        std::shared_ptr<IParticleEmitter> Find(std::string&& name)
        {
            for(auto& emitter : mEmitters)
            {
                if(emitter->GetName() == name)
                {
                    return emitter;
                }
            }
            Debugger::GetLogger().AddMessage("Can not find demanding particle Emitter : " + name, SeverityLevel::NOTICE);
            return nullptr;
        }

    private:
        std::vector<std::shared_ptr<IParticleEmitter>> mEmitters;
    };
}
