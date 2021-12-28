#pragma once
#include "entt.hpp"

namespace Rengin
{
class Scene
{
private:
    entt::registry m_registry;
public:
    Scene(/* args */);
    ~Scene();
};

} // namespace Rengin
