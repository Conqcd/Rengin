#include "repch.hpp"
#include "Scene.hpp"

#include <glm/glm.hpp>

namespace Rengin
{

static void DoMath(const glm::mat4& transform)
{

}
static void OnTransformConstruct(entt::registry& _registry,entt::entity _entity)
{

}

Scene::Scene(/* args */)
{
    struct MeshComponent
    {
        float value;
        MeshComponent() = default;
    };
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

    m_registry.on_construct<TransformComponent>().connect<&OnTransformConstruct>();


    // if(m_registry.has<TransformComponent>(entity_))
    //     auto& transform = m_registry.get<TransformComponent>(entity_);
    
    auto view = m_registry.view<TransformComponent>();

    for(auto _entity : view)
    {
        auto& transform = view.get<TransformComponent>(_entity);
    }

    auto group = m_registry.group<TransformComponent>(entt::get<MeshComponent>);
    for(auto _entity : group)
    {
        auto&[transform,mesh] = group.get<TransformComponent,MeshComponent>(_entity);
    }
}

Scene::~Scene()
{
}

void Scene::OnUpdate(TimeStep ts)
{
    auto group = m_registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
    for(auto _entity : group)
    {
        auto&[transform,sprite] = group.get<TransformComponent,SpriteRendererComponent>(_entity);
        Renderer2D::DrawQuad();
    }
}

entt::entity Scene::CreateEntity()
{
    return m_registry.create();
}

} // namespace Rengin
