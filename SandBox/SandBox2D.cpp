#include "SandBox2D.hpp"
#include <chrono>
template<typename Fn>
class Timer
{
private:
    const char* m_name;
    Fn m_func;
    bool m_stop;
    std::chrono::time_point<std::chrono::steady_clock> m_StartTimePoint;
public:
    Timer(const char* name,Fn&& func)
       : m_name(name) , m_func(func) , m_stop(false)
    {
        m_StartTimePoint = std::chrono::high_resolution_clock::now();
    }
    ~Timer()
    {
        if(!m_stop)
            Stop();
    }
    void Stop()
    {
        auto m_EndTimePoint = std::chrono::high_resolution_clock::now();
        
        long long start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimePoint).time_since_epoch().count();
        long long end = std::chrono::time_point_cast<std::chrono::microseconds>(m_EndTimePoint).time_since_epoch().count();

        float duration = (end - start) * 0.001f;
        m_stop = true;
        m_func({m_name , duration});

    }
};


#define PROFILE_SCOPE(name) Timer timer##__LINE__ (name,[&] (ProfileResult profileresult){m_ProfileResults.push_back(profileresult);})

SandBox2D::SandBox2D(/* args */)
        :Layer("SandBox2D"),m_camera_controller(1280.f/720.f,true)
{
}

SandBox2D::~SandBox2D()
{
}

void SandBox2D::OnUpdate(Rengin::TimeStep timestep)
{
    PROFILE_SCOPE("SandBox2D::OnUpdate");
    {
        m_camera_controller.OnUpdate(timestep);
        PROFILE_SCOPE("SandBox2D::Control");
    }

    {
        Rengin::RenderCommand::SetClearColor({0.1f,0.1f,0.1f,1});
        Rengin::RenderCommand::Clear();
        PROFILE_SCOPE("SandBox2D::Prep");
    }

    {
        Rengin::Renderer2D::BeginScene(m_camera_controller.getCamera());

        // Rengin::MaterialRef material = new Rengin::Material(m_shader);

        Rengin::Renderer2D::DrawQuad({1.0f,0.0f},{1.0f,1.0f},{0.2f,0.3f,0.8f,1.0f});
        Rengin::Renderer2D::DrawQuad({-1.0f,0.0f},{1.0f,1.0f},m_texture);
        // Rengin::Renderer::Submit(m_shader,m_verarr);

        Rengin::Renderer2D::EndScene();
        PROFILE_SCOPE("SandBox2D::Draw");
    }
}

void SandBox2D::OnImGuiRender()
{        
    ImGui::Begin("Settings");
    for(auto& profile : m_ProfileResults)
    {
        char label[50];
        strcpy(label," %.3f ms ");
        strcac(label,profile.m_name);
        ImGui::Text(label,profile.time);    
    }
    m_ProfileResults.clear();
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