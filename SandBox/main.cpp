#include <rengine.hpp>
#include <Rengine/Core/entrypoint.hpp>
#include "SandBox2D.hpp"
#include <imgui.h>

class ExampleLayer : public Rengin::Layer
{
public:
    ExampleLayer()
    :Layer("Example"),m_camera_controller(1280.f/720.f,true)
    {
        m_verarr = Rengin::VertexArray::Create();

        float vertices[9] = {
            -0.5f,-0.5f,0.0f,
            0.5f,-0.5f,0.0f,
            0.0f,0.5f,0.0f
        };
        unsigned int indices[3] = {
            0,1,2
        };

        m_verbuf = Rengin::VertexBuffer::Create(vertices,sizeof(vertices));
        // m_verbuf->Bind();

        Rengin::BufferLayout layout = {{Rengin::ShadeDataType::Float3 , "a_position"}};
        m_verbuf->SetLayout(layout);

        m_verarr->AddVertexBuffer(m_verbuf);

        m_indbuf = Rengin::IndexBuffer::Create(indices,sizeof(indices) / sizeof(uint32_t));
        // m_indbuf->Bind();

        m_verarr->SetIndexBuffer(m_indbuf);

        m_shader = Rengin::Shader::Create("litle","assets/shaders/vertex.glsl","assets/shaders/fragment.glsl");
    }

    void OnUpdate(Rengin::TimeStep timestep) override
    {
        RE_INFO("Delta Time is {0}s ({1}ms)",timestep.getSeconds(),timestep.getMilliSeconds());
        // RE_INFO("Updating");
        // if(Rengin::Input::isKeyPressed(RE_KEY_TAB))5tr
        // {
        //     RE_TRACE("Tab is Pressed!");
        // }

        m_camera_controller.OnUpdate(timestep);

        Rengin::RenderCommand::SetClearColor({0.1f,0.1f,0.1f,1});
        Rengin::RenderCommand::Clear();

        Rengin::Renderer::BeginScene(m_camera_controller.getCamera());

        // Rengin::MaterialRef material = new Rengin::Material(m_shader);

        Rengin::Renderer::Submit(m_shader,m_verarr);

        Rengin::Renderer::EndScene();
    }
    
    virtual void OnImGuiRender() override
    {
        ImGui::Begin("Settings");
        ImGui::Text("Hello World");
        ImGui::End();
    }

    void OnEvent(Rengin::Event& ev){
        
        m_camera_controller.OnEvent(ev);

        // if(ev.getEventType() == Rengin::EventType::WindowResize)
        // {
        //     auto& re = (WindowResizeEvent&) e;

        //     float zoom = static_cast<float>( re.getWidth()) / 1280.0f;
        //     m_camera_controller.setZoom
        // }
    }
private:
    Rengin::ShaderLibrary m_shader_lib;
    Rengin::Ref<Rengin::Shader> m_shader;
    Rengin::Ref<Rengin::VertexBuffer> m_verbuf;
    Rengin::Ref<Rengin::IndexBuffer> m_indbuf;
    Rengin::Ref<Rengin::VertexArray> m_verarr;
    Rengin::Ref<Rengin::Texture> m_texture;

    Rengin::OrthoGraphicsCameraController m_camera_controller;
};

class SandBox :public Rengin::Application
{
public:
    SandBox() :Rengin::Application()
    {

        PushLayer(new ExampleLayer());
        PushLayer(new SandBox2D());
    }

    ~SandBox(){}
};


Rengin::Application* Rengin::CreateApplication()
{
    return new SandBox();
}