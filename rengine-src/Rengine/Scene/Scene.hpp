#pragma once
#include "Rengine/Core/core.hpp"
#include "Rengine/Core/TimeStep.hpp"
#include "Component.hpp"
#include "entt.hpp"

namespace Rengin
{
class RE_API Scene
{
private:
    entt::registry m_registry;
public:
    Scene(/* args */);
    ~Scene();
    void OnUpdate(TimeStep ts);
    entt::entity CreateEntity();

    //TEMP
    entt::registry& Reg() {return m_registry;}
};

} // namespace Rengin
