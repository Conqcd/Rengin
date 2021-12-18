#include "repch.hpp"
#include "application.hpp"
#include "Event/Event.hpp"
#include "Event/ApplicationEvent.hpp"
#include "log.hpp"
#include "Renderer/RenderCommand.hpp"
#include "Renderer/Renderer.hpp"

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <GL/gl.h>

#include "Renderer/Buffer.hpp"
#include "Renderer/OrthoGraphicsCamera.hpp"
#include "Input.hpp"


namespace Rengin
{

Application* Application::m_instance = nullptr;

Application::Application()
{
    RE_CORE_ASSERT(!m_instance,"Application already exists!");

    m_instance = this;

    m_window = Scope<Window>(Window::WindowCreate());
    
    m_window->setEventCallBack(RE_BIND_FUNC_EVENT_1(Application::OnEvent));

    Renderer::Init();

    m_imgui_layer = new ImGuiLayer();
    PushOverLayer(m_imgui_layer);
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
    // WindowResizeEvent e(1200,900);
    // RE_CORE_TRACE(e);


    while(m_running)
    {
        float time = static_cast<float>(glfwGetTime());
        TimeStep timestep = time - m_last_frame_time;
        m_last_frame_time = time;
        for(auto* layer : m_layer_stack)
        {
            layer->OnUpdate(timestep);
        }

        m_imgui_layer->Begin();
        for(auto* layer : m_layer_stack)
        {
            layer->OnImGuiRender();
        }
        m_imgui_layer->End();

        auto [x,y] = Input::getMousePosition();

        RE_CORE_TRACE("{0} {1}",x,y);
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