#pragma once
#include "../../repch.hpp"
#include "../core.hpp"
#include "../Layer.hpp"
#include "../Event/Event.hpp"
#include "../Event/KeyEvent.hpp"
#include "../Event/MouseEvent.hpp"
#include "../Event/ApplicationEvent.hpp"

namespace Rengin
{

    // WindowClose,,WindowFocus,WindowLoseFocus,WindowMove,
    // AppTick,AppUpdate,AppRender

class RE_API ImGuiLayer :public Layer
{
private:
    float m_time = 0.0f;
    bool OnMouseButtonPressEvent(MouseButtonPressEvent& ev);
    bool OnMouseButtonReleaseEvent(MouseButtonReleaseEvent& ev);
    bool OnMouseMovedEvent(MouseMovedEvent& ev);
    bool OnMouseScrolledEvent(MouseScrolledEvent& ev);
    bool OnKeyTypeEvent(KeyTypeEvent& ev);
    bool OnKeyReleaseEvent(KeyReleaseEvent& ev);
    bool OnKeyPressEvent(KeyPressEvent& ev);
    bool OnWindowResizeEvent(WindowResizeEvent& ev);
public:
    ImGuiLayer(/* args */);
    ~ImGuiLayer();

    void OnAttach();
    void OnDetach();
    
    void OnUpdate();
    void OnEvent(Event& ev);
};

} // namespace Rengin
