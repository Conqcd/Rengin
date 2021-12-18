#include <rengine.hpp>

#include <imgui.h>

class ExampleLayer : public Rengin::Layer
{
public:
    ExampleLayer()
    :Layer("Example"),m_camera(-1.0f,1.0f,-1.0f,1.0f)
    {
        m_verarr.reset(Rengin::VertexArray::Create());
    
        float vertices[9] = {
            -0.5f,-0.5f,0.0f,
            0.5f,-0.5f,0.0f,
            0.0f,0.5f,0.0f
        };
        unsigned int indices[3] = {
            0,1,2
        };

        m_verbuf.reset(Rengin::VertexBuffer::Create(vertices,sizeof(vertices)));
        // m_verbuf->Bind();
        Rengin::BufferLayout layout = {{Rengin::ShadeDataType::Float3 , "a_position"}};
        m_verbuf->SetLayout(layout);

        m_verarr->AddVertexBuffer(m_verbuf);

        m_indbuf.reset(Rengin::IndexBuffer::Create(indices,sizeof(indices) / sizeof(uint32_t)));
        // m_indbuf->Bind();

        m_verarr->SetIndexBuffer(m_indbuf);

        std::string vertexSrc = R"(
            #version 330

            layout(location = 0) in vec3 a_position;

            uniform mat4 u_ViewProjection;
            uniform mat4 u_Transform;

            out vec3 v_position;
            void main()
            {
                v_position = a_position;
                gl_Position = u_ViewProjection * u_Transform * vec4(a_position,1.0);
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
        m_shader = Rengin::Shader::Create("litle",vertexSrc,fragmentSrc);
    }

    void OnUpdate(Rengin::TimeStep timestep) override
    {
        RE_INFO("Delta Time is {0}s ({1}ms)",timestep.getSeconds(),timestep.getMilliSeconds());
        // RE_INFO("Updating");
        // if(Rengin::Input::isKeyPressed(RE_KEY_TAB))
        // {
        //     RE_TRACE("Tab is Pressed!");
        // }
        Rengin::RenderCommand::SetClearColor({0.1f,0.1f,0.1f,1});
        Rengin::RenderCommand::Clear();

        Rengin::Renderer::BeginScene(m_camera);

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
        
        RE_TRACE("Event {0}",ev);
    }
private:
    Rengin::Ref<Rengin::Shader> m_shader;
    Rengin::Ref<Rengin::VertexBuffer> m_verbuf;
    Rengin::Ref<Rengin::IndexBuffer> m_indbuf;
    Rengin::Ref<Rengin::VertexArray> m_verarr;
    Rengin::Ref<Rengin::Texture> m_texture;

    Rengin::OrthoGraphicsCamera m_camera;
};

class SandBox :public Rengin::Application
{
public:
    SandBox() :Rengin::Application()
    {

        PushLayer(new ExampleLayer());
    }

    ~SandBox(){}
};


Rengin::Application* Rengin::CreateApplication()
{
    return new SandBox();
}