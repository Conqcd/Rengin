#pragma once
#include "../repch.hpp"
#include "core.hpp"
#include "Window.hpp"
#include "Event/ApplicationEvent.hpp"
#include "LayerStack.hpp"

namespace Rengin
{

class RE_API Application
{
private:
    std::unique_ptr<Window> m_window;
    bool m_running = true;
    LayerStack m_layer_stack;
public:
    Application();

    virtual ~Application();

    void Run();

    void OnEvent(Event& e);

    void PushLayer(Layer* layer);
    void PushOverLayer(Layer* layer);
private:
    bool OnWindowClose(WindowCloseEvent& ev);
};

Application* CreateApplication();
}
