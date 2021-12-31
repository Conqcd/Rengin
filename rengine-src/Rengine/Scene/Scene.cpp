#include "repch.hpp"
#include "Scene.hpp"
#include "Entity.hpp"
#include "Rengine/Renderer/Renderer2D.hpp"
#include "Component.hpp"
#include <glm/glm.hpp>
namespace Rengin
{

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

template<typename T>
void Scene::OnComponentAdd(Entity entity,T& component)
{
    static_assert(false);
}


template<>
void Scene::OnComponentAdd<TransformComponent>(Entity entity,TransformComponent& component)
{

}

template<>
void Scene::OnComponentAdd<TagComponent>(Entity entity,TagComponent& component)
{

}

template<>
void Scene::OnComponentAdd<SpriteRendererComponent>(Entity entity,SpriteRendererComponent& component)
{

}

template<>
void Scene::OnComponentAdd<NativeScriptComponent>(Entity entity,NativeScriptComponent& component)
{

}

template<>
void Scene::OnComponentAdd<CameraComponent>(Entity entity,CameraComponent& component)
{
    component.Camera.SetViewportSize(m_ViewportWidth,m_ViewportHeight);
}

void Scene::OnUpdate(TimeStep ts)
{    
    //Update Script
    {
        m_registry.view<NativeScriptComponent>().each([=](auto entity,auto& nsc)
        {
            if (!nsc.Instance)
            {
                nsc.Instance = nsc.InstantiateScript();
                nsc.Instance->m_Entity = Entity{entity,this};
                nsc.Instance->OnCreate();
            }
            nsc.Instance->OnUpdate(ts);
        });
    }

    //Render 2D
    auto view = m_registry.view<TransformComponent,CameraComponent>();
    Camera* MainCamera = nullptr;
    glm::mat4 CameraTransform;
    for(auto entity : view)
    {
        auto&[transform,camera] = view.get<TransformComponent,CameraComponent>(entity);
        if(camera.Primary)
        {
            MainCamera = & camera.Camera;
            CameraTransform = transform.GetTransform();
            break;
        }
    }
    if (MainCamera)
    {
        Renderer2D::BeginScene(MainCamera->getProjection(),CameraTransform);

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

void Scene::DestroyEntity(Entity entity)
{
    m_registry.destroy(entity);
}

void Scene::OnViewportResize(uint32_t width,uint32_t height)
{
    m_ViewportWidth = width;
    m_ViewportHeight = height;

    auto view = m_registry.view<CameraComponent>();
    for(auto entity : view)
    {
        // auto& CameraComponent = view.get<CameraComponent>(entity).Camera;
        // if(!CameraComponent.FixedAspectRatio)
        // {
        //     CameraComponent.Camera.SetViewportSize(width,height);
        // }
    }
}

} // namespace Rengin
