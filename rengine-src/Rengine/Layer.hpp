#pragma once
#include "core.hpp"
#include "Rengine/Event/Event.hpp"
#include "Core/TimeStep.hpp"

namespace Rengin
{
class RE_API Layer
{
protected:
    std::string m_name;
public:
    Layer(const std::string& name = "Layer");
    virtual ~Layer();

    virtual void OnAttach(){}
    virtual void OnDetach(){}
    virtual void OnUpdate(TimeStep timestep){}
    
    virtual void OnImGuiRender() {}
    virtual void OnEvent(Event& ev){}

    inline const std::string& getName()const {return m_name;}
};


}
