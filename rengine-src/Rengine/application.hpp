#pragma once
#include "../repch.hpp"
#include "core.hpp"
#include "Window.hpp"
#include "Event/ApplicationEvent.hpp"

namespace Rengin
{

class RE_API Application
{
private:
    std::unique_ptr<Window> m_window;
    bool m_running = true;
public:
    Application();

    virtual ~Application();

    void Run();

    void OnEvent(Event& e);

private:
    bool OnWindowClose(WindowCloseEvent& ev);
};

Application* CreateApplication();
}
