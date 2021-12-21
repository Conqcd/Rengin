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

    Rengin::RenderCommand::SetClearColor({0.1f,0.1f,0.1f,1});
    Rengin::RenderCommand::Clear();

    Rengin::Renderer2D::BeginScene(m_camera_controller.getCamera());

    // Rengin::MaterialRef material = new Rengin::Material(m_shader);

    Rengin::Renderer2D::DrawQuad({1.0f,0.0f},{1.0f,1.0f},{0.2f,0.3f,0.8f,1.0f});
    // Rengin::Renderer::Submit(m_shader,m_verarr);

    Rengin::Renderer2D::EndScene();
}

void SandBox2D::OnImGuiRender()
{        
    ImGui::Begin("Settings");
    ImGui::Text("Hello World");
    ImGui::End();
}

void SandBox2D::OnAttach()
{
}

void SandBox2D::OnDetach()
{

}

void SandBox2D::OnEvent(Rengin::Event& ev)
{
    m_camera_controller.OnEvent(ev);
}