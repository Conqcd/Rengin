#include "repch.hpp"
#include "Scene.hpp"
#include "Entity.hpp"
#include "Rengine/Renderer/Renderer2D.hpp"
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
#if RE_EXAMPLE_CODE
    struct MeshComponent
    {
        float value;
        MeshComponent() = default;
    };

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
#endif
}

Scene::~Scene()
{
}

void Scene::OnUpdate(TimeStep ts)
{    
    auto view = m_registry.view<TransformComponent,CameraComponent>();
    Camera* MainCamera = nullptr;
    glm::mat4* CameraTransform = nullptr;
    for(auto entity : view)
    {
        auto&[transform,camera] = view.get<TransformComponent,CameraComponent>(entity);
        if(camera.Primary)
        {
            MainCamera = & camera.Camera;
            CameraTransform = &transform.Transform;
            break;
        }
    }
    if (MainCamera)
    {
        Renderer2D::BeginScene(MainCamera->getProjection(),*CameraTransform);

        auto group = m_registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
        for(auto _entity : group)
        {
            auto&[transform,sprite] = group.get<TransformComponent,SpriteRendererComponent>(_entity);
            // Renderer2D::DrawQuad();
        }

        Renderer2D::EndScene();
    }
    
    
}

Entity Scene::CreateEntity(const std::string name)
{
    Entity entity{m_registry.create(),this};
    entity.AddComponent<TransformComponent>();
    auto& tag = entity.AddComponent<TagComponent>();
    tag.Tag =name.empty()? "Entity" : name;
    
    return entity; 
}

} // namespace Rengin
