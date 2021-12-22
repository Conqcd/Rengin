#include "SandBox2D.hpp"
#include <chrono>

SandBox2D::SandBox2D(/* args */)
        :Layer("SandBox2D"),m_camera_controller(1280.f/720.f,true)
{
}

SandBox2D::~SandBox2D()
{
}

void SandBox2D::OnUpdate(Rengin::TimeStep timestep)
{
    RE_PROFILE_FUNCTION();
    {
        m_camera_controller.OnUpdate(timestep);
        RE_PROFILE_SCOPE("SandBox2D::Control");
    }

    {
        Rengin::RenderCommand::SetClearColor({0.1f,0.1f,0.1f,1});
        Rengin::RenderCommand::Clear();
        RE_PROFILE_SCOPE("SandBox2D::Prep");
    }

    {
        Rengin::Renderer2D::BeginScene(m_camera_controller.getCamera());

        // Rengin::MaterialRef material = new Rengin::Material(m_shader);

        Rengin::Renderer2D::DrawQuad({1.0f,0.0f},{1.0f,1.0f},{0.2f,0.3f,0.8f,1.0f});
        Rengin::Renderer2D::DrawQuad({-1.0f,0.0f},{1.0f,1.0f},m_texture);
        // Rengin::Renderer::Submit(m_shader,m_verarr);

        Rengin::Renderer2D::EndScene();
        RE_PROFILE_SCOPE("SandBox2D::Draw");
    }
}

void SandBox2D::OnImGuiRender()
{        
    RE_PROFILE_FUNCTION();
    ImGui::Begin("Settings");

    ImGui::End();
}

void SandBox2D::OnAttach()
{
    m_texture = Rengin::Texture2D::Create("E:\\CAD&&CG\\Rengin\\SandBox\\assets\\textures\\France.jpg");
}

void SandBox2D::OnDetach()
{

}

void SandBox2D::OnEvent(Rengin::Event& ev)
{
    m_camera_controller.OnEvent(ev);
}