#pragma once
#include "Rengine/Core/core.hpp"
#include "Rengine/Core/log.hpp"
#include "Rengine/Core/UUID.hpp"
#include "Component.hpp"
#include "Scene.hpp"
#include <entt.hpp>

namespace Rengin {
class RE_API Entity {
private:
  entt::entity m_EntityHandle{entt::null};
  Scene *m_scene = nullptr;

public:
  Entity(entt::entity entityHandle, Scene *scene);
  Entity() = default;
  Entity(const Entity &other) = default;

  template <typename T> bool HasComponent() {
    return m_scene->m_registry.all_of<T>(m_EntityHandle);
  }

  template <typename T> T &GetComponent() {
    RE_CORE_ASSERT(HasComponent<T>(), "Entity does not have component!");
    return m_scene->m_registry.get<T>(m_EntityHandle);
  }

    template <typename T, typename... Args> T &AddComponent(Args &&...args) {
        RE_CORE_ASSERT(!HasComponent<T>(), "Entity already has component!");

        T &component = m_scene->m_registry.emplace<T>(m_EntityHandle,std::forward<Args>(args)...);
        m_scene->OnComponentAdd<T>(*this, component);
        return component;
    }

    template <typename T, typename... Args> T &AddOrReplaceComponent(Args &&...args) {
        T &component = m_scene->m_registry.emplace_or_replace<T>(m_EntityHandle,std::forward<Args>(args)...);
        m_scene->OnComponentAdd<T>(*this, component);
        return component;
    }

  template <typename T> void RemoveComponent() {
    RE_CORE_ASSERT(HasComponent<T>(), "Entity does not have component!");
    m_scene->m_registry.remove<T>(m_EntityHandle);
  }

  operator bool() const { return m_EntityHandle != entt::null; }
  operator uint32_t() const { return static_cast<uint32_t>(m_EntityHandle); }
  operator entt::entity() const { return m_EntityHandle; }

  UUID GetUUID() {return GetComponent<IDComponent>().ID;}
  const std::string& GetName() {return GetComponent<TagComponent>().Tag;}

  bool operator==(const Entity &others) const {
    return m_EntityHandle == others.m_EntityHandle && m_scene == others.m_scene;
  }

  bool operator!=(const Entity &others) const { return !operator==(others); }
};

} // namespace Rengin
