#pragma once

#include <glm/glm.hpp>
#include "Rengine/Renderer/Camera.hpp"
#include "SceneCamera.hpp"
namespace Rengin
{
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
        
    };
    
} // namespace Rengin
