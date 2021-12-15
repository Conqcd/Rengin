#include "repch.hpp"
#include "application.hpp"
#include "Event/Event.hpp"
#include "Event/ApplicationEvent.hpp"
#include "log.hpp"

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <GL/gl.h>

#include "Renderer/Buffer.hpp"

#include "Input.hpp"


namespace Rengin
{

Application* Application::m_instance = nullptr;

Application::Application()
{
    RE_CORE_ASSERT(!m_instance,"Application already exists!");

    m_instance = this;

    m_window = std::unique_ptr<Window>(Window::WindowCreate());
    
    m_window->setEventCallBack(RE_BIND_FUNC_EVENT_1(Application::OnEvent));

    m_imgui_layer = new ImGuiLayer();

    PushOverLayer(m_imgui_layer);
    
    m_verarr.reset(VertexArray::Create());
    
    float vertices[9]={
        -0.5f,-0.5f,0.0f,
        0.5f,-0.5f,0.0f,
        0.0f,0.5f,0.0f
    };
    unsigned int indices[3]={
        0,1,2
    };

    m_verbuf.reset(VertexBuffer::Create(vertices,sizeof(vertices)));
    // m_verbuf->Bind();
    BufferLayout layout = {{ShadeDataType::Float3 , "a_position"}};
    m_verbuf->SetLayout(layout);

    m_verarr->AddVertexBuffer(m_verbuf);

    m_indbuf.reset(IndexBuffer::Create(indices,sizeof(indices)/sizeof(uint32_t)));
    // m_indbuf->Bind();

    m_verarr->SetIndexBuffer(m_indbuf);
    m_SquareVA.reset(VertexArray::Create());
    std::string vertexSrc = R"(
        #version 330

        layout(location = 0) in vec3 a_position;

        out vec3 v_position;
        void main()
        {
            v_position = a_position;
            gl_Position = vec4(a_position,1.0);
        }
    )";
    std::string fragmentSrc = R"(
        #version 330
        
        in vec3 v_position;
        out vec4 color;

        void main()
        {
            color = vec4(v_position,1.0);
        }
    )";
    m_shader.reset(new Shader(vertexSrc,fragmentSrc));
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
        glClearColor(0.1f,0.1f,0.1f,1);
        glClear(GL_COLOR_BUFFER_BIT);

        RenderCommand::SetClearColor();

        Renderer::BeginScene();

        m_shader->Bind();
        m_verarr->Bind();

        Renderer::Submit(m_verarr);

        Renderer::EndScene();
        glDrawElements(GL_TRIANGLES,m_indbuf->GetCount(),GL_UNSIGNED_INT,nullptr);

        for(auto* layer : m_layer_stack)
        {
            layer->OnUpdate();
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