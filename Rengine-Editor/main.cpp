#include <rengine.hpp>
#include <Rengine/Core/entrypoint.hpp>
#include "EditorLayer.hpp"
#include <imgui.h>

namespace Rengin{
class ExampleLayer : public Layer
{
public:
    ExampleLayer()
    :Layer("Example"),m_camera_controller(1280.f/720.f,true)
    {
        m_verarr = VertexArray::Create();

        float vertices[9] = {
            -0.5f,-0.5f,0.0f,
            0.5f,-0.5f,0.0f,
            0.0f,0.5f,0.0f
        };
        unsigned int indices[3] = {
            0,1,2
        };

        m_verbuf = VertexBuffer::Create(vertices,sizeof(vertices));
        // m_verbuf->Bind();

        BufferLayout layout = {{ShadeDataType::Float3 , "a_position"}};
        m_verbuf->SetLayout(layout);

        m_verarr->AddVertexBuffer(m_verbuf);

        m_indbuf = IndexBuffer::Create(indices,sizeof(indices) / sizeof(uint32_t));
        // m_indbuf->Bind();

        m_verarr->SetIndexBuffer(m_indbuf);

        m_shader = Shader::Create("litle","../../../SandBox/assets/shaders/vertex.glsl","../../../SandBox/assets/shaders/fragment.glsl");
    }

    void OnUpdate(TimeStep timestep) override
    {
        RE_INFO("Delta Time is {0}s ({1}ms)",timestep.getSeconds(),timestep.getMilliSeconds());
        // RE_INFO("Updating");
        // if(Rengin::Input::isKeyPressed(RE_KEY_TAB))5tr
        // {
        //     RE_TRACE("Tab is Pressed!");
        // }

        m_camera_controller.OnUpdate(timestep);

        RenderCommand::SetClearColor({0.1f,0.1f,0.1f,1});
        RenderCommand::Clear();

        Renderer::BeginScene(m_camera_controller.getCamera());

        // MaterialRef material = new Material(m_shader);

        Renderer::Submit(m_shader,m_verarr);

        Renderer::EndScene();
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
    ShaderLibrary m_shader_lib;
    Ref<Shader> m_shader;
    Ref<VertexBuffer> m_verbuf;
    Ref<IndexBuffer> m_indbuf;
    Ref<VertexArray> m_verarr;
    Ref<Texture> m_texture;
    OrthoGraphicsCameraController m_camera_controller;
};

class SandBox :public Application
{
public:
    SandBox() :Application()
    {

        // PushLayer(new ExampleLayer());
        PushLayer(new EditorLayer());
    }

    ~SandBox(){}
};


Application* CreateApplication()
{
    return new SandBox();
}
}