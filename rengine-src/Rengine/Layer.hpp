#pragma once
#include "../repch.hpp"
#include "core.hpp"
#include "Event/Event.hpp"

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
    virtual void OnUpdate(){}
    virtual void OnEvent(Event& ev){}

    inline const std::string& getName()const {return m_name;}
};


}
