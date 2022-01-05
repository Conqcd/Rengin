#include "repch.hpp"
#include "application.hpp"
#include "Rengine/Event/Event.hpp"
#include "Rengine/Event/ApplicationEvent.hpp"
#include "log.hpp"
#include "Rengine/Renderer/RenderCommand.hpp"
#include "Rengine/Renderer/Renderer.hpp"

#include <glad/glad.h>
#include "GLFW/glfw3.h"
#include <GL/gl.h>

#include "Rengine/Renderer/Buffer.hpp"
#include "Rengine/Renderer/OrthoGraphicsCamera.hpp"
#include "Input.hpp"


namespace Rengin
{

Application* Application::m_instance = nullptr;

Application::Application()
{
    RE_PROFILE_FUNCTION();
    RE_CORE_ASSERT(!m_instance,"Application already exists!");
    m_instance = this;

    m_window = Window::WindowCreate();
    
    m_window->setEventCallBack(RE_BIND_FUNC_EVENT_1(Application::OnEvent));

    Renderer::Init();

    m_imgui_layer = new ImGuiLayer();
    PushOverLayer(m_imgui_layer);
}

Application::~Application()
{
    RE_PROFILE_FUNCTION();

    // Renderer::Shutdown();
}

void Application::Close()
{
    m_running = false;
}

bool Application::OnWindowClose(WindowCloseEvent& ev)
{
    m_running = false;
    return true;    
}

bool Application::OnWindowResize(WindowResizeEvent& ev)
{
    RE_PROFILE_FUNCTION();
    
    if(ev.getHeight() == 0 || ev.getWidth() == 0)
    {
        m_minimized = true;
        return false;
    }
    m_minimized = false;
    Renderer::OnWindowResized(ev.getWidth(),ev.getHeight());
    return false;
}

void Application::OnEvent(Event& e)
{
    RE_PROFILE_FUNCTION();
    
    EventDispatcher dispatcher(e);

    dispatcher.Dispatch<WindowCloseEvent>(RE_BIND_FUNC_EVENT_1(Application::OnWindowClose));
    dispatcher.Dispatch<WindowResizeEvent>(RE_BIND_FUNC_EVENT_1(Application::OnWindowResize));
    
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

    RE_PROFILE_FUNCTION();
    

    while(m_running)
    {
        RE_PROFILE_SCOPE("RunLoop");

        float time = static_cast<float>(glfwGetTime());
        TimeStep timestep = time - m_last_frame_time;
        m_last_frame_time = time;
        if(!m_minimized)
        {
            
            RE_PROFILE_SCOPE("LayerStack OnUpdate");
    
            for(auto* layer : m_layer_stack)
            {
                layer->OnUpdate(timestep);
            }
        }
        

        m_imgui_layer->Begin();
        {
            RE_PROFILE_SCOPE("LayerStack OnImGuiRender");
            for(auto* layer : m_layer_stack)
            {
                layer->OnImGuiRender();
            }
        }
        m_imgui_layer->End();

        auto [x,y] = Input::getMousePosition();

        RE_CORE_TRACE("{0} {1}",x,y);
        m_window->OnUpdate();
    }
}

void Application::PushLayer(Layer* layer)
{
    RE_PROFILE_FUNCTION();
    
    m_layer_stack.PushLayer(layer);
    layer->OnAttach();
}

void Application::PushOverLayer(Layer* layer)
{
    RE_PROFILE_FUNCTION();

    m_layer_stack.PushOverLayer(layer);
    layer->OnAttach();
}
}