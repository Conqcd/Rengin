#pragma once
#include "Rengine/Core/core.hpp"
#include "Entity.hpp"
#include "Rengine/Core/TimeStep.hpp"
namespace Rengin
{
class Entity;
class RE_API ScriptableEntity
{
private:
    Entity m_Entity;
protected:
    virtual void OnUpdate(TimeStep ts) {}
    virtual void OnDestroy(){}
    virtual void OnCreate() {}
public:
    ScriptableEntity(/* args */) = default;
    virtual ~ScriptableEntity() = default;
    
    template<typename T>
    T& GetComponent()
    {
        return m_Entity.GetComponent<T>();
    }
    
    friend class Scene;
};


} // namespace Rengin
