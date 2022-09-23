#include "UIEditorScene.h"



#include "Engine/Component/CameraShakeComponent.h"
#include "Engine/Engine.h"
#include "Engine/Entity/CameraEntity.h"
#include "Engine/System/AnimationUpdateDrawSystem.h"
#include "Engine/System/ScriptSystem.h"
#include "Engine/System/TextureDrawSystem.h"
#include "Engine/System/TextureBoxUpdateSystem.h"
#include "System/UI/ButtonManagingSystem.h"

void UIEditorScene::Load()
{
    using namespace q_engine;
    const glm::vec2 winSize = Engine::GetWindowSize();

    //Camera
    CameraEntity* camera = AddEntity<CameraEntity>("UICam");
    AddComponent<TransformComponent>(camera, glm::vec2{});
    AddComponent<CameraShakeComponent>(camera);

    AddDrawSystem<TextureDrawSystem>();
    AddDrawSystem<AnimationUpdateDrawSystem>();
    AddUpdateSystem<ScriptSystem>();
    AddUpdateSystem<TextureBoxUpdateSystem>();
    AddUpdateSystem<ButtonManagingSystem>();
    GameState::Load();
}
