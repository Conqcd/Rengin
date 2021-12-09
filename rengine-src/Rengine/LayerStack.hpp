#pragma once
#include "../repch.hpp"
#include "core.hpp"
#include "Layer.hpp"

namespace Rengin
{
class RE_API LayerStack
{
private:
    std::vector<Layer*> m_layers;
    std::vector<Layer*>::iterator m_layit;
public:
    LayerStack(/* args */);
    ~LayerStack();

    void PushLayer(Layer* layer);
    void PushOverLayer(Layer* layer);
    void PopLayer(Layer* layer);
    void PopOverLayer(Layer* layer);

    inline std::vector<Layer*>::iterator begin(){return m_layers.begin();}
    inline std::vector<Layer*>::iterator end(){return m_layers.end();}
};

}