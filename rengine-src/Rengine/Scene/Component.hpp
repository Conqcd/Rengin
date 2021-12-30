#pragma once

#include "Rengine/Renderer/Camera.hpp"
#include "SceneCamera.hpp"
#include "ScriptableEntity.hpp"
#include "Rengine/Core/TimeStep.hpp"
#include <glm/glm.hpp>

namespace Rengin
{
// class ScriptableEntity;
struct TagComponent
{
    std::string Tag;
    TagComponent() = default;
    TagComponent(const TagComponent&) = default;
    TagComponent(const std::string& tag)
        :Tag(tag)   {}
};

struct TransformComponent
{
    glm::mat4 Transform {1.0f};
    TransformComponent() = default;
    TransformComponent(const TransformComponent&) = default;
    TransformComponent(const glm::mat4& transform)
        :Transform(transform)   {}

    operator const glm::mat4& () const {return Transform;}
    operator glm::mat4& () {return Transform;}
};

struct SpriteRendererComponent
{
    glm::vec4 Color{1.0f,1.0f,1.0f,1.0f};
    SpriteRendererComponent() = default;
    SpriteRendererComponent(const SpriteRendererComponent&) = default;
    SpriteRendererComponent(const glm::vec4& color)
        :Color(color)   {}

};

struct CameraComponent
{
    SceneCamera Camera;
    bool Primary = true;
    bool FixedAspectRatio = false;
    CameraComponent() = default;
    CameraComponent(const CameraComponent&) = default;
    // CameraComponent(const glm::mat4& projection)
    //     :Camera(projection)   {}
};

struct NativeScriptComponent
{
    ScriptableEntity* Instance = nullptr;

    ScriptableEntity*(*InstantiateScript)();
    void(*DestroyInstanceScript)(NativeScriptComponent*);
    // std::function<void()> InstantiateScript;
    // std::function<void()> DestroyInstanceScript;
    
    // std::function<void(ScriptableEntity*)> OnCreateFunction;
    // std::function<void(ScriptableEntity*)> OnDestroyFunction;
    // std::function<void(ScriptableEntity*,TimeStep)> OnUpdateFunction;

    template<typename T>
    void Bind()
    {
        // InstantiateScript = [&]() { Instance = new T(); };
        // DestroyInstanceScript = [&]() { delete reinterpret_cast<T*>(Instance); Instance = nullptr; };
        InstantiateScript = []() { return static_cast<ScriptableEntity*>(new T()); };
        DestroyInstanceScript = [](NativeScriptComponent* nsc) { delete nsc->Instance; nsc->Instance = nullptr; };

        // OnCreateFunction = [](ScriptableEntity* instance) { reinterpret_cast<T*>(instance)->OnCreate();};
        // OnDestroyFunction = [](ScriptableEntity* instance) { reinterpret_cast<T*>(instance)->OnDestroy();};
        // OnUpdateFunction = [](ScriptableEntity* instance,TimeStep ts) { reinterpret_cast<T*>(instance)->OnUpdate(ts);};
    }
};

} // namespace Rengin
