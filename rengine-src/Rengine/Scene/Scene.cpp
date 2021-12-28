#include "repch.hpp"
#include "Scene.hpp"

#include <glm/glm.hpp>

namespace Rengin
{

static void DoMath(const glm::mat4& transform)
{

}

Scene::Scene(/* args */)
{
    struct TransformComponent
    {
        glm::mat4 Transform;
        TransformComponent() = default;
        TransformComponent(const TransformComponent&) = default;
        TransformComponent(const glm::mat4& transform)
            :Transform(transform)   {}

        operator const glm::mat4& () const {return Transform;}
        operator glm::mat4& () {return Transform;}
    };
    // TransformComponent transform;

    // DoMath(transform);

    entt::entity entity_ = m_registry.create();

    m_registry.emplace<TransformComponent>(entity_,glm::mat4(1.0f));

}

Scene::~Scene()
{
}

} // namespace Rengin
