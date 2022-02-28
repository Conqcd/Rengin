#include "Scene.hpp"
#include "Component.hpp"
#include "Entity.hpp"
#include "Rengine/Renderer/Renderer2D.hpp"
#include "Rengine/Renderer/Renderer3D.hpp"
#include "ScriptableEntity.hpp"
#include "repch.hpp"
#include <glm/glm.hpp>
namespace Rengin {

Scene::Scene(/* args */) {
#if RE_EXAMPLE_CODE
  struct MeshComponent {
    float value;
    MeshComponent() = default;
  };

  entt::entity entity_ = m_registry.create();

  m_registry.emplace<TransformComponent>(entity_, glm::mat4(1.0f));

  m_registry.on_construct<TransformComponent>()
      .connect<&OnTransformConstruct>();

  // if(m_registry.has<TransformComponent>(entity_))
  //     auto& transform = m_registry.get<TransformComponent>(entity_);

  auto view = m_registry.view<TransformComponent>();

  for (auto _entity : view) {
    auto &transform = view.get<TransformComponent>(_entity);
  }

  auto group = m_registry.group<TransformComponent>(entt::get<MeshComponent>);
  for (auto _entity : group) {
    auto &[transform, mesh] =
        group.get<TransformComponent, MeshComponent>(_entity);
  }
#endif
}

Scene::~Scene() {}

template <typename T> void Scene::OnComponentAdd(Entity entity, T &component) {
  // static_assert(false);
}

template <typename Component>
static void CopyComponent(entt::registry& dst,entt::registry& src,const std::unordered_map<UUID,entt::entity>& enttMap)
{
    auto view = src.view<Component>();
    for (auto e : view)
    {
        UUID uuid = src.get<IDComponent>(e).ID;
        RE_CORE_ASSERT(enttMap.find(uuid) != enttMap.end());
        entt::entity dstEnttID = enttMap.at(uuid);
        auto &component = src.get<Component>(e);
        dst.emplace_or_replace<Component>(dstEnttID, component);
    }
}

template <typename Component>
static void CopyComponentIfExists(Entity& dst,Entity& src)
{
    if(src.HasComponent<Component>())
        dst.AddOrReplaceComponent<Component>(src.GetComponent<Component>());
}

Ref<Scene> Scene::Copy(Ref<Scene> other)
{
    auto newScene = CreateRef<Scene>();
    newScene->m_ViewportWidth = other->m_ViewportWidth;
    newScene->m_ViewportHeight = other->m_ViewportHeight;

    std::unordered_map<UUID,entt::entity> enttMap;
    
    auto& srcSceneRegistry = other->m_registry;
    auto& dstSceneRegistry = newScene->m_registry;
    auto idView = srcSceneRegistry.view<IDComponent>();

    for (auto e : idView)
    {
        UUID uuid = srcSceneRegistry.get<IDComponent>(e).ID;
        const auto& name = srcSceneRegistry.get<TagComponent>(e).Tag;
        auto newEntity = newScene->CreateEntitywithUUID(uuid,name);

        enttMap[uuid] = static_cast<entt::entity>(newEntity);
    }

    CopyComponent<TransformComponent>(dstSceneRegistry,srcSceneRegistry,enttMap);
    CopyComponent<SpriteRendererComponent>(dstSceneRegistry,srcSceneRegistry,enttMap);
    CopyComponent<CameraComponent>(dstSceneRegistry,srcSceneRegistry,enttMap);
    CopyComponent<NativeScriptComponent>(dstSceneRegistry,srcSceneRegistry,enttMap);
    CopyComponent<Texture3DComponent>(dstSceneRegistry,srcSceneRegistry,enttMap);
    CopyComponent<Texture2DComponent>(dstSceneRegistry,srcSceneRegistry,enttMap);

    return newScene;
}

template <>
void Scene::OnComponentAdd<TransformComponent>(Entity entity,
                                               TransformComponent &component) {}

template <>
void Scene::OnComponentAdd<TagComponent>(Entity entity,
                                         TagComponent &component) {}

template <>
void Scene::OnComponentAdd<SpriteRendererComponent>(
    Entity entity, SpriteRendererComponent &component) {}

template <>
void Scene::OnComponentAdd<OpacityTransferFunctionComponent>(
    Entity entity, OpacityTransferFunctionComponent &component) {}

template <>
void Scene::OnComponentAdd<ColorTransferFunctionComponent>(
    Entity entity, ColorTransferFunctionComponent &component) {}

template <>
void Scene::OnComponentAdd<Texture3DComponent>(Entity entity,
                                               Texture3DComponent &component) {}

template <>
void Scene::OnComponentAdd<Texture2DComponent>(Entity entity,
                                               Texture2DComponent &component) {}

template <>
void Scene::OnComponentAdd<NativeScriptComponent>(
    Entity entity, NativeScriptComponent &component) {}

template <>
void Scene::OnComponentAdd<CameraComponent>(Entity entity,
                                            CameraComponent &component) {
  component.Camera.SetViewportSize(m_ViewportWidth, m_ViewportHeight);
}

void Scene::OnUpdateEditor(TimeStep ts, EditorCamera &camera)
{
    Renderer2D::BeginScene(camera);

    auto group = m_registry.group<TransformComponent>(
        entt::get<SpriteRendererComponent>);
    for (auto _entity : group) {
      auto &&[transform, sprite] =
          group.get<TransformComponent, SpriteRendererComponent>(_entity);
      Renderer2D::DrawSprite(transform.GetTransform(),sprite,(int)_entity);
    }
    Renderer2D::EndScene();

    Renderer3D::BeginScene(camera);

    auto viewv = m_registry.view<TransformComponent, Texture3DComponent,
                                 OpacityTransferFunctionComponent,
                                 ColorTransferFunctionComponent>();
    for (auto _entity : viewv) {
      auto &&[transform, texture, transfera, transferc] =
          viewv.get<TransformComponent, Texture3DComponent,
                    OpacityTransferFunctionComponent,
                    ColorTransferFunctionComponent>(_entity);

      float stepLength = 0.01, focalLength = 1.0 / tan(glm::radians(camera.GetFOV()) / 2.0);

      auto Viewmatrix = camera.GetViewMatrix();
      Renderer3D::DrawVolume(camera.getProjection(), Viewmatrix,
          transform.GetTransform(), texture.Texture, transform.Scale,
          {m_ViewportWidth, m_ViewportHeight}, focalLength,
          camera.GetPosition(),camera.GetPosition(),
          stepLength, transfera.Opacity, transferc.Color);
      Renderer3D::EndScene();
    }
}

void Scene::OnUpdateRuntime(TimeStep ts) {
  // Update Script
  {
    m_registry.view<NativeScriptComponent>().each([=](auto entity, auto &nsc) {
      if (!nsc.Instance) {
        nsc.Instance = nsc.InstantiateScript();
        nsc.Instance->m_Entity = Entity{entity, this};
        nsc.Instance->OnCreate();
      }
      nsc.Instance->OnUpdate(ts);
    });
  }

  // Render 2D
  auto view = m_registry.view<TransformComponent, CameraComponent>();
  Camera *MainCamera = nullptr;
  glm::mat4 CameraTransform;
  SceneCamera::ProjectionType CameraType;
  float MainFOV;
  for (auto entity : view) {
    auto &&[transform, camera] =
        view.get<TransformComponent, CameraComponent>(entity);
    if (camera.Primary) {
      MainCamera = &camera.Camera;
      CameraTransform = transform.GetTransform();
      CameraType = camera.Camera.GetProjectionType();
      MainFOV = camera.Camera.GetPerspectiveFOV();
      break;
    }
  }
  if (MainCamera) {
    Renderer2D::BeginScene(MainCamera->getProjection(), CameraTransform);

    auto group = m_registry.group<TransformComponent>(
        entt::get<SpriteRendererComponent>);
    for (auto _entity : group) {
      auto &&[transform, sprite] =
        group.get<TransformComponent, SpriteRendererComponent>(_entity);
        // Renderer2D::DrawQuad(transform.Translation, transform.Scale,sprite.Color);
        Renderer2D::DrawSprite(transform.GetTransform(),sprite,(int)_entity);
    }
    Renderer2D::EndScene();

    Renderer3D::BeginScene(MainCamera->getProjection(), CameraTransform);

    if(CameraType == SceneCamera::ProjectionType::Perspective)
    {
      auto viewv = m_registry.view<TransformComponent, Texture3DComponent,OpacityTransferFunctionComponent,ColorTransferFunctionComponent>();
      for (auto _entity : viewv) {
        auto &&[transform, texture, transfera, transferc] =
            viewv.get<TransformComponent, Texture3DComponent,
                      OpacityTransferFunctionComponent,
                      ColorTransferFunctionComponent>(_entity);

        float stepLength = 0.01, focalLength = 1.0 / tan(MainFOV / 2.0);
        
        Renderer3D::DrawVolume(
            MainCamera->getProjection(), glm::inverse(CameraTransform),
            transform.GetTransform(), texture.Texture,transform.Scale,
            {m_ViewportWidth, m_ViewportHeight}, focalLength, {CameraTransform[3][0],CameraTransform[3][1],CameraTransform[3][2]},
            {CameraTransform[3][0],CameraTransform[3][1],CameraTransform[3][2]}, stepLength, transfera.Opacity, transferc.Color);
      }
    }

    Renderer3D::EndScene();
  }
}

Entity Scene::CreateEntity(const std::string& name) {
  return CreateEntitywithUUID(UUID(),name);
}

Entity Scene::CreateEntitywithUUID(UUID uuid, const std::string& name)
{
  Entity entity{m_registry.create(), this};
  entity.AddComponent<IDComponent>(uuid);
  entity.AddComponent<TransformComponent>();
  auto &tag = entity.AddComponent<TagComponent>();
  tag.Tag = name.empty() ? "Entity" : name;

  return entity;
}

void Scene::DestroyEntity(Entity entity) { m_registry.destroy(entity); }

void Scene::OnViewportResize(uint32_t width, uint32_t height) {
  m_ViewportWidth = width;
  m_ViewportHeight = height;

  auto view = m_registry.view<CameraComponent>();
  for (auto entity : view) {
    auto &cameraComponent = view.get<CameraComponent>(entity);
    if (!cameraComponent.FixedAspectRatio) {
      cameraComponent.Camera.SetViewportSize(width, height);
    }
  }
}
Entity Scene::GetPrimaryCameraEntity() {
  auto view = m_registry.view<CameraComponent>();
  for (auto entity : view) {
    const auto &cameraComponent = view.get<CameraComponent>(entity);
    if (cameraComponent.Primary) {
      return {entity, this};
    }
  }
  return {};
}

void Scene::OnRuntimeStart()
{

}

void Scene::OnRuntimeStop()
{

}

void Scene::DuplicateEntity(Entity entity)
{
    Entity newentity = CreateEntity(entity.GetName());
    CopyComponentIfExists<TransformComponent>(newentity,entity);
    CopyComponentIfExists<SpriteRendererComponent>(newentity,entity);
    CopyComponentIfExists<CameraComponent>(newentity,entity);
    CopyComponentIfExists<NativeScriptComponent>(newentity,entity);
    CopyComponentIfExists<Texture3DComponent>(newentity,entity);
    CopyComponentIfExists<Texture2DComponent>(newentity,entity);
}

} // namespace Rengin
