#include "SandBox2D.hpp"

SandBox2D::SandBox2D(/* args */)
        :Layer("SandBox2D"),m_camera_controller(1280.f/720.f,true)
{
}

SandBox2D::~SandBox2D()
{
}


void SandBox2D::OnUpdate(Rengin::TimeStep timestep)
{
    m_camera_controller.OnUpdate(timestep);
}

void SandBox2D::OnImGuiRender()
{        
    ImGui::Begin("Settings");
    ImGui::Text("Hello World");
    ImGui::End();
}

void SandBox2D::OnAttach()
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

    m_verbuf.reset(Rengin::VertexBuffer::Create(vertices,sizeof(vertices)));
    // m_verbuf->Bind();
    Rengin::BufferLayout layout = {{Rengin::ShadeDataType::Float3 , "a_position"}};
    m_verbuf->SetLayout(layout);

    m_verarr->AddVertexBuffer(m_verbuf);

    m_indbuf.reset(Rengin::IndexBuffer::Create(indices,sizeof(indices) / sizeof(uint32_t)));
    // m_indbuf->Bind();

    m_verarr->SetIndexBuffer(m_indbuf);

    m_shader = Rengin::Shader::Create("litle","assets/shaders/vertex.glsl","assets/shaders/fragment.glsl");
}

void SandBox2D::OnDetach()
{

}

void SandBox2D::OnEvent(Rengin::Event& ev)
{
    m_camera_controller.OnEvent(ev);
}