#include "LayerStack.hpp"

namespace Rengin
{
LayerStack::LayerStack(/* args */)
{
    m_layit = m_layers.begin();
}

LayerStack::~LayerStack()
{
    for(auto* layer:m_layers )
        delete layer;
}

void LayerStack::PushLayer(Layer* layer)
{
    m_layit = m_layers.emplace(m_layit,layer);
}

void LayerStack::PushOverLayer(Layer* layer)
{=
    m_layit = m_layers.emplace_back(layer);
}

void LayerStack::PopLayer(Layer* layer)
{
    auto it = std::find(m_layers.being(),m_layers.end(),layer);
    if(it != m_layers.end())
    {
        m_layers.erase(it);
        m_layit--;
    }
}

void LayerStack::PopOverLayer(Layer* layer)
{
    auto it = std::find(m_layers.being(),m_layers.end(),layer);
    if(it != m_layers.end())
    {
        m_layers.erase(it);
    }
}

}