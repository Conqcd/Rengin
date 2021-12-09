#pragma once
#include "../../../repch.hpp"
#include "Rengine/core.hpp"
#include "Rengine/Layer.hpp"
#include "Rengine/Event/Event.hpp"

namespace Rengin
{

class ImGuiLayer :public Layer
{
private:
    /* data */
public:
    ImGuiLayer(/* args */);
    ~ImGuiLayer();

    void OnAttach();
    void OnDetach();
    
    void OnUpdate();
    void OnEvent(Event& ev);
};

} // namespace Rengin
