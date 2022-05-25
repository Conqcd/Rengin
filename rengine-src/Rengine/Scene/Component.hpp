#pragma once
#include "Rengine/Renderer/Camera.hpp"
#include "SceneCamera.hpp"
#include "Rengine/Core/TimeStep.hpp"
#include "Rengine/Core/UUID.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <Rengine/Renderer/Texture.hpp>
#include <Rengine/Utils/TransferFunction.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace Rengin
{

struct IDComponent
{
    UUID ID;
    IDComponent() = default;
    IDComponent(const IDComponent&) = default;
    IDComponent(uint64_t id)
        :ID(id)   {}
};


class ScriptableEntity;
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
        glm::mat4 rotation = glm::toMat4(glm::quat(Rotation));
        // glm::mat4 rotation = glm::rotate(glm::mat4(1.0f),Rotation.x,{1.0f,0.0f,0.0f})
        // * glm::rotate(glm::mat4(1.0f),Rotation.y,{0.0f,1.0f,0.0f})
        // * glm::rotate(glm::mat4(1.0f),Rotation.z,{0.0f,0.0f,1.0f});
        
        return glm::translate(glm::mat4(1.0f),Translation)
        *   rotation
        *   glm::scale(glm::mat4(1.0f),Scale);
    }
};

struct SpriteRendererComponent
{
    glm::vec4 Color{1.0f,1.0f,1.0f,1.0f};
    Ref<Texture2D> Texture;
    float TilingFactor = 1.0f;
    SpriteRendererComponent() = default;
    SpriteRendererComponent(const SpriteRendererComponent&) = default;
    SpriteRendererComponent(const glm::vec4& color)
        :Color(color)   {}
};

struct EmissionComponent
{
    glm::vec4 Color{1.0f,1.0f,1.0f,1.0f};
    EmissionComponent() = default;
    EmissionComponent(const EmissionComponent&) = default;
    EmissionComponent(const glm::vec4& color)
        :Color(color)   {}
};

struct OpacityTransferFunctionComponent
{
    TransferFunction<float,float> Opacity;
    OpacityTransferFunctionComponent() = default;
    OpacityTransferFunctionComponent(const OpacityTransferFunctionComponent &) = default;
    OpacityTransferFunctionComponent(const TransferFunction<float, float> &opacity)
        : Opacity(opacity) {}
};

struct ColorTransferFunctionComponent
{
    TransferFunction<float, glm::vec3> Color;
    ColorTransferFunctionComponent() = default;
    ColorTransferFunctionComponent(const ColorTransferFunctionComponent &) = default;
    ColorTransferFunctionComponent(const TransferFunction<float, glm::vec3> &color)
        : Color(color) {}
};

struct Texture2DComponent
{
    Ref<Texture2D> Texture;
    std::string Path;
    int width,height;
    Texture2DComponent() = default;
    Texture2DComponent(const Texture2DComponent&) = default;
    Texture2DComponent(const Ref<Texture2D> texture)
        :Texture(texture)   {}
};

struct Texture3DComponent
{
    Ref<Texture3D> Texture;
    std::string Path;
    float Threshold = 0.0f;
    int width,height,depth;
    Texture3DComponent() = default;
    Texture3DComponent(const Texture3DComponent&) = default;
    Texture3DComponent(const Ref<Texture3D> texture)
        :Texture(texture)   {}
};

struct CircleRendererComponent
{
    glm::vec4 Color{1.0f,1.0f,1.0f,1.0f};
    float Radius = 0.5f;
    float Thickness = 1.0f;
    float Fade = 1.0f;

    CircleRendererComponent() = default;
    CircleRendererComponent(const CircleRendererComponent&) = default;
    CircleRendererComponent(const glm::vec4& color,float r,float t,float f)
            : Color(color),Radius(r),Thickness(t),Fade(f)   {}
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

struct ForceComponent
{
    Ref<Texture3D> Texture;
    ForceComponent() = default;
    ForceComponent(const ForceComponent&) = default;
    // CameraComponent(const glm::mat4& projection)
    //     :Camera(projection)   {}
};

struct ConstraintComponent
{
    Ref<Texture3D> Texture;
    ConstraintComponent() = default;
    ConstraintComponent(const ConstraintComponent&) = default;
};

struct SolveComponent
{
    Ref<Texture3D> Texture;
    SolveComponent() = default;
    SolveComponent(const SolveComponent&) = default;
};

struct ResultComponent
{
    Ref<Texture3D> Texture;
    std::string Path;
    float Threshold = 0.0f;
    int width,height,depth;
    int showId = 1;
    glm::vec4 maxvalue;
    ResultComponent() = default;
    ResultComponent(const ResultComponent&) = default;
    ResultComponent(const Ref<Texture3D> texture)
        :Texture(texture)   {}
};

struct ObjectComponent
{
    //TODO
    ObjectComponent() = default;
    ObjectComponent(const ObjectComponent&) = default;
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

struct Rigidbody2DComponent
{
    enum class BodyType {Static = 0,Dynamic, Kinematic};
    BodyType Type = BodyType::Static;

    //Storage for runtime
    void* RuntimeBody = nullptr;

    Rigidbody2DComponent() = default;
    Rigidbody2DComponent(const Rigidbody2DComponent&) = default;
};

struct BoxCollider2DComponent
{
    glm::vec2 Offset = {0.0f,0.0f};
    glm::vec2 Size = {0.5f,0.5f};

    float Density = 1.0f;
    float Friction = 0.5f;
    float Resititution = 0.0f;
    float RestitutionThreshold = 0.5f;

    void* RuntimeFixture = nullptr;

    BoxCollider2DComponent() = default;
    BoxCollider2DComponent(const BoxCollider2DComponent&) = default;
};

} // namespace Rengin
