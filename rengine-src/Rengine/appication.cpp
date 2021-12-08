#include "../repch.hpp"
#include "application.hpp"
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
    
    m_window->setEventCallBack(std::bind(&Application::OnEvent , this , std::placeholders::_1));
    // m_window->setEventCallBack(BIND_APP_EVENT_1(OnEvent));
}

Application::~Application()
{

}

void Application::OnEvent(Event& e)
{
    RE_CORE_INFO("{0}",e);
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