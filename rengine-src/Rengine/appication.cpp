#include "../repch.hpp"
#include "application.hpp"
#include "Event/Event.hpp"
#include "Event/ApplicationEvent.hpp"
#include "log.hpp"
#include "GLFW/glfw3.h"
#include <GL/gl.h>

namespace Rengin
{

#define BIND_APP_EVENT_1(x) std::bind(&Application::x , this , std::placeholders::_1)

Application::Application()
{
    m_window = std::unique_ptr<Window>(Window::WindowCreate());
    
    m_window->setEventCallBack(BIND_APP_EVENT_1(OnEvent));
}

Application::~Application()
{

}

bool Application::OnWindowClose(WindowCloseEvent& ev)
{
    m_running = false;
    return true;    
}

void Application::OnEvent(Event& e)
{
    EventDispatcher dispatcher(e);
    
    dispatcher.Dispatch<WindowCloseEvent>(std::bind(&Application::OnWindowClose , this, std::placeholders::_1));
    RE_CORE_TRACE("{0}",e);
}

void Application::Run()
{
    WindowResizeEvent e(1200,900);
    RE_CORE_TRACE(e);
    

    while(m_running)
    {
        glClearColor(1,0,1,1);
        glClear(GL_COLOR_BUFFER_BIT);
        m_window->OnUpdate();
    }
}

}