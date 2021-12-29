#pragma once
#include "Rengine/Core/core.hpp"
#include "Entity.hpp"

namespace Rengin
{
class RE_API ScriptableEntity
{
private:
    Entity m_Entity;
public:
    ScriptableEntity(/* args */) = default;
    ~ScriptableEntity() = default;
    
    template<typename T>
    T& GetComponent()
    {
        return m_Entity.GetComponent<T>();
    }

    friend class Scene;
};


} // namespace Rengin
