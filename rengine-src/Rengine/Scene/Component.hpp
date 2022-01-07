#pragma once
#include "Rengine/Renderer/Camera.hpp"
#include "SceneCamera.hpp"
#include "ScriptableEntity.hpp"
#include "Rengine/Core/TimeStep.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <Rengine/Renderer/Texture.hpp>
#include <Rengine/Utils/TransferFunction.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

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
    glm::vec3 Translation {0.0f,0.0f,0.0f};
    glm::vec3 Rotation {0.0f,0.0f,0.0f};
    glm::vec3 Scale {1.0f,1.0f,1.0f};

    TransformComponent() = default;
    TransformComponent(const TransformComponent&) = default;
    TransformComponent(const glm::vec3& translation)
        :Translation(translation)   {}

    glm::mat4 GetTransform()const{
        // glm::mat4 rotation = glm::toMat4(glm::quat(Rotation));
        glm::mat4 rotation = glm::rotate(glm::mat4(1.0f),Rotation.x,{1.0f,0.0f,0.0f})
        * glm::rotate(glm::mat4(1.0f),Rotation.y,{0.0f,1.0f,0.0f})
        * glm::rotate(glm::mat4(1.0f),Rotation.z,{0.0f,0.0f,1.0f});
        
        return glm::translate(glm::mat4(1.0f),Translation)
        *   rotation
        *   glm::scale(glm::mat4(1.0f),Scale);
    }
};

struct SpriteRendererComponent
{
    glm::vec4 Color{1.0f,1.0f,1.0f,1.0f};
    SpriteRendererComponent() = default;
    SpriteRendererComponent(const SpriteRendererComponent&) = default;
    SpriteRendererComponent(const glm::vec4& color)
        :Color(color)   {}
};

struct OpacityTransferFunctionComponent
{
    TransferFunction<int,int> Opacity;
    OpacityTransferFunctionComponent() = default;
    OpacityTransferFunctionComponent(const OpacityTransferFunctionComponent &) = default;
    OpacityTransferFunctionComponent(const TransferFunction<int, int> &opacity)
        : Opacity(opacity) {}
};

struct ColorTransferFunctionComponent
{
    TransferFunction<int, glm::vec4> Color;
    ColorTransferFunctionComponent() = default;
    ColorTransferFunctionComponent(const ColorTransferFunctionComponent &) = default;
    ColorTransferFunctionComponent(const TransferFunction<int, glm::vec4> &color)
        : Color(color) {}
};

struct Texture2DComponent
{
    Ref<Texture2D> Texture;
    std::string Path;
    Texture2DComponent() = default;
    Texture2DComponent(const Texture2DComponent&) = default;
    Texture2DComponent(const Ref<Texture2D> texture)
        :Texture(texture)   {}
};

struct Texture3DComponent
{
    Ref<Texture3D> Texture;
    std::string Path;
    Texture3DComponent() = default;
    Texture3DComponent(const Texture3DComponent&) = default;
    Texture3DComponent(const Ref<Texture3D> texture)
        :Texture(texture)   {}
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
