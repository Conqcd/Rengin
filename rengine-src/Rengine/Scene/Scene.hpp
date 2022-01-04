#pragma once
#include <entt.hpp>
#include "Rengine/Core/core.hpp"
#include "Rengine/Core/TimeStep.hpp"


namespace Rengin
{
class RE_API Entity;
class RE_API Scene
{
private:
    entt::registry m_registry;
    uint32_t m_ViewportWidth = 0;
    uint32_t m_ViewportHeight = 0;
private:
    template<typename T>
    void OnComponentAdd(Entity entity,T& component);
public:
    Scene(/* args */);
    ~Scene();
    void OnUpdate(TimeStep ts);
    Entity CreateEntity(const std::string name = std::string());
    void DestroyEntity(Entity entity);

    void OnViewportResize(uint32_t width,uint32_t height);
    //TEMP
    entt::registry& Reg() {return m_registry;}

    friend class Entity;
    friend class SceneSerializer;
    friend class SceneHierarchyPanel;
};

} // namespace Rengin
