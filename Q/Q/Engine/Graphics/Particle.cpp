/*
    File Name: Particle.cpp
    Project Name: Q
    Author(s):
        Primary: Yoonki Kim
        Secondary:
    All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#include "Particle.h"

void q_engine::IParticleEmitter::Update(double dt)
{
    auto playState = Engine::GetEngineComponent<GameStateManager>().GetPlayState();

    for (auto& particle : mControllingParticles)
    {
        particle->Update(dt);
    }

    for (auto& particle : mControllingParticles)
    {
        if (particle->IsDead())
        {
            mDeletingQueParticles.push_back(particle);
        }
    }

    for (auto& particle : mDeletingQueParticles)
    {
        playState->DeleteEntityComponents(particle->GetEntityID());
        mControllingParticles.erase(std::find(mControllingParticles.begin(), mControllingParticles.end(), particle));
    }

    mDeletingQueParticles.clear();
}

void q_engine::RectMeshParticleEmitter::Draw() const
{
    auto playState = Engine::GetEngineComponent<GameStateManager>().GetPlayState();
    auto& transformComponents = playState->GetComponents<TransformComponent>();

    const TransformComponent& camTransform = transformComponents[playState->GetEntities<CameraEntity>().Front().GetEntityID()];

    auto& lifeTimeComponents = playState->GetComponents<LifeTimeComponent>();

    for (auto& particle : mControllingParticles)
    {
        if (!particle->IsDead())
        {
            const glm::mat3 world2NDC = CameraEntity::CalculateWorldToNDCTransform(camTransform.GetPosition(), camTransform.GetRotationRadian(), camTransform.GetScale());
            auto& particleTransformComp = *transformComponents.find(particle->GetEntityID());

            const double lifeTimePercentage = lifeTimeComponents[particle->GetEntityID()].GetLifeTimeAsPercentage();

            Color particleColor = mDrawer.GetColor();

            particleColor.a = static_cast<float>(lifeTimePercentage);

            mDrawer.SetColor(particleColor);
            mDrawer.Draw(world2NDC * particleTransformComp.GetOrCalculateTransformMatrix());
        }
    }
}

void q_engine::RectMeshParticleEmitter::Emit(unsigned num_particles, glm::vec2 emitPos, glm::vec2 emitVel, glm::vec2 minVel, glm::vec2 maxVel, glm::vec2 size, glm::vec2 lifeTime, float gravityConstant)
{
    auto& rand = Engine::GetEngineComponent<RandomMachine>();
    auto playState = Engine::GetEngineComponent<GameStateManager>().GetPlayState();

    if (minVel.x > maxVel.x)
    {
        std::swap(minVel.x, maxVel.x);
    }
    if (minVel.y > maxVel.y)
    {
        std::swap(minVel.y, maxVel.y);
    }

    const unsigned int numOfRemainingParticleSpawnerSize = maxSize - static_cast<unsigned>(mControllingParticles.size());
    if (numOfRemainingParticleSpawnerSize < num_particles)
    {
        auto& lifeTimeComps = playState->GetComponents<LifeTimeComponent>();
        mControllingParticles.sort([&lifeTimeComps](auto particle1, auto particle2)
            {
                const double lifetime1 = lifeTimeComps[particle1->GetEntityID()].GetLifeTime();
                const double lifetime2 = lifeTimeComps[particle2->GetEntityID()].GetLifeTime();
                return lifetime1 < lifetime2;
            });

        auto controllingParticleBegin = mControllingParticles.begin();

        int numOfDeletingParticle = num_particles;
        while (numOfDeletingParticle > 0)
        {
            auto deletingParticle = controllingParticleBegin++;
            mDeletingQueParticles.push_back(*deletingParticle);
            --numOfDeletingParticle;
        }
        Update(0);
    }

    if (num_particles > maxSize)
    {
        num_particles = maxSize;
    }
    while (num_particles > 0)
    {
        glm::vec2 particleVelocity = { rand.GetRandomValue(minVel.x, maxVel.x), rand.GetRandomValue(minVel.y, maxVel.y) };
        const float particleSize = rand.GetRandomValue(size.x, size.y);
        const float particleLifeTime = rand.GetRandomValue(lifeTime.x, lifeTime.y);

        const auto particle = playState->AddEntity<RectMeshParticle>();

        playState->AddComponent<TransformComponent>(particle, emitPos, 0.f, glm::vec2{ particleSize,particleSize });
        playState->AddComponent<VelocityComponent>(particle, particleVelocity + emitVel);
        playState->AddComponent<LifeTimeComponent>(particle, particleLifeTime);
        playState->AddComponent<GravityComponent>(particle, gravityConstant);

        mControllingParticles.push_back(std::make_shared<RectMeshParticle>(*particle));

        num_particles--;
    }
}