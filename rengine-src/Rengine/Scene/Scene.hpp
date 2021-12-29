#pragma once
#include "entt.hpp"
#include "Rengine/Core/TimeStep.hpp"
#include "Component.hpp"

namespace Rengin
{
class Scene
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
