#include "repch.hpp"
#include "LayerStack.hpp"

namespace Rengin
{
LayerStack::LayerStack(/* args */)
{
}

LayerStack::~LayerStack()
{
    for(auto* layer : m_layers )
        delete layer;
}

void LayerStack::PushLayer(Layer* layer)
{
    m_layers.emplace(m_layers.begin() + m_lay_id++,layer);
}

void LayerStack::PushOverLayer(Layer* layer)
{
    m_layers.emplace_back(layer);
}

void LayerStack::PopLayer(Layer* layer)
{
    auto it = std::find(m_layers.begin(),m_layers.end(),layer);
    if(it != m_layers.end())
    {
        m_layers.erase(it);
        m_lay_id--;
    }
}

void LayerStack::PopOverLayer(Layer* layer)
{
    auto it = std::find(m_layers.begin(),m_layers.end(),layer);
    if(it != m_layers.end())
    {
        m_layers.erase(it);
    }
}

}