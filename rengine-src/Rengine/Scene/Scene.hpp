#pragma once
#include "Rengine/Core/core.hpp"
#include "Rengine/Core/TimeStep.hpp"
#include "Component.hpp"
#include "entt.hpp"

namespace Rengin
{
class Entity;
class RE_API Scene
{
private:
    entt::registry m_registry;
    uint32_t m_ViewportWidth = 0;
    uint32_t m_ViewportHeight = 0;
public:
    Scene(/* args */);
    ~Scene();
    void OnUpdate(TimeStep ts);
    Entity CreateEntity(const std::string name = std::string());

    void OnViewportResize(uint32_t width,uint32_t height);
    //TEMP
    entt::registry& Reg() {return m_registry;}

    friend class Entity;
};

} // namespace Rengin
