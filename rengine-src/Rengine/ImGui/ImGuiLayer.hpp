#pragma once
#include "../../repch.hpp"
#include "../core.hpp"
#include "../Layer.hpp"
#include "../Event/Event.hpp"

namespace Rengin
{

class RE_API ImGuiLayer :public Layer
{
private:
    float m_time = 0.0f;
public:
    ImGuiLayer(/* args */);
    ~ImGuiLayer();

    void OnAttach();
    void OnDetach();
    
    void OnUpdate();
    void OnEvent(Event& ev);
};

} // namespace Rengin
