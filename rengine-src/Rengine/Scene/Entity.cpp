#include "repch.hpp"
#include "Entity.hpp"

namespace Rengin
{

Entity::Entity(entt::entity entityHandle,Scene* scene)
    : m_EntityHandle(entityHandle) , m_scene(scene)
{   
    HasComponent<int>();
}
} // namespace Rengin
