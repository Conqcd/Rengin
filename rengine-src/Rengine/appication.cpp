#include "../repch.hpp"
#include "application.hpp"
#include "Event/Event.hpp"
#include "Event/ApplicationEvent.hpp"
#include "log.hpp"

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <GL/gl.h>

namespace Rengin
{

// #define BIND_APP_EVENT_1(x) std::bind(&Application::x , this , std::placeholders::_1)

Application* Application::m_instance = nullptr;

Application::Application()
{
    RE_CORE_ASSERT(!m_instance,"Application already exists!");

    m_instance = this;

    m_window = std::unique_ptr<Window>(Window::WindowCreate());
    
    m_window->setEventCallBack(RE_BIND_FUNC_EVENT_1(Application::OnEvent));

    unsigned int id;

    glGenVertexArrays(1,&id);
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
    // RE_CORE_TRACE("{0}",e);

    for(auto it = m_layer_stack.end() ; it != m_layer_stack.begin();)
    {
        (*(--it))->OnEvent(e);
        if(e.getHandle())
        {
            break;
        }
    }
}

void Application::Run()
{
    WindowResizeEvent e(1200,900);
    RE_CORE_TRACE(e);
    

    while(m_running)
    {
        glClearColor(1,0,1,1);
        glClear(GL_COLOR_BUFFER_BIT);

        
        for(auto* layer : m_layer_stack)
        {
            layer->OnUpdate();
        }
        m_window->OnUpdate();
    }
}
void Application::PushLayer(Layer* layer)
{
    m_layer_stack.PushLayer(layer);
    layer->OnAttach();
}

void Application::PushOverLayer(Layer* layer)
{
    m_layer_stack.PushOverLayer(layer);
    layer->OnAttach();
}
}