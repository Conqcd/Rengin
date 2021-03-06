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

    m_camera_controller.OnUpdate(timestep);
    Rengin::Renderer2D::resetStats();
    {
        RE_PROFILE_SCOPE("SandBox2D::Prep");

        m_framebuffer->Bind();
        Rengin::RenderCommand::SetClearColor({0.1f,0.1f,0.1f,1});
        Rengin::RenderCommand::Clear();
    }

    {
        RE_PROFILE_SCOPE("SandBox2D::Draw");
        Rengin::Renderer2D::BeginScene(m_camera_controller.getCamera());

        // Rengin::MaterialRef material = new Rengin::Material(m_shader);

        Rengin::Renderer2D::DrawQuad({1.0f,0.0f},{1.0f,1.0f},{0.2f,0.3f,0.8f,1.0f});
        // Rengin::Renderer2D::DrawRotatedQuad({1.0f,0.0f},{1.0f,1.0f},glm::radians(45.0f),{0.2f,0.3f,0.8f,1.0f});
        // Rengin::Renderer2D::DrawQuad({-1.0f,0.0f},{1.0f,1.0f},m_texture);
        // Rengin::Renderer::Submit(m_shader,m_verarr);

        Rengin::Renderer2D::EndScene();
        
        m_framebuffer->Unbind();
    }
}

void SandBox2D::OnImGuiRender()
{
    RE_PROFILE_FUNCTION();
    ImGui::Begin("Settings");

    auto stats = Rengin::Renderer2D::getStats();
    ImGui::Text("Renderer2D Stats:");
    ImGui::Text("Call Draw: %d",stats.DrawCall);
    ImGui::Text("Quads: %d",stats.QuadCount);
    ImGui::Text("Vertices: %d",stats.GetTotalVertexCount());
    ImGui::Text("Indices: %d",stats.GetTotalIndexCount());

    ImGui::End();

    static bool p_open = true;
    static bool opt_fullscreen = true;
    static bool opt_padding = false;
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

    // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
    // because it would be confusing to have two docking targets within each others.
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    if (opt_fullscreen)
    {
        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    }
    else
    {
        dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
    }

    // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
    // and handle the pass-thru hole, so we ask Begin() to not render a background.
    if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
        window_flags |= ImGuiWindowFlags_NoBackground;

    // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
    // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
    // all active windows docked into it will lose their parent and become undocked.
    // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
    // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
    if (!opt_padding)
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("DockSpace Demo", &p_open, window_flags);
    if (!opt_padding)
        ImGui::PopStyleVar();

    if (opt_fullscreen)
        ImGui::PopStyleVar(2);

    // Submit the DockSpace
    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
    {
        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
    }

    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("Options"))
        {
            // Disabling fullscreen would allow the window to be moved to the front of other windows,
            // which we can't undo at the moment without finer window depth/z control.
            ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen);
            ImGui::MenuItem("Padding", NULL, &opt_padding);
            ImGui::Separator();

            if (ImGui::MenuItem("Flag: NoSplit",                "", (dockspace_flags & ImGuiDockNodeFlags_NoSplit) != 0))                 { dockspace_flags ^= ImGuiDockNodeFlags_NoSplit; }
            if (ImGui::MenuItem("Flag: NoResize",               "", (dockspace_flags & ImGuiDockNodeFlags_NoResize) != 0))                { dockspace_flags ^= ImGuiDockNodeFlags_NoResize; }
            if (ImGui::MenuItem("Flag: NoDockingInCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_NoDockingInCentralNode) != 0))  { dockspace_flags ^= ImGuiDockNodeFlags_NoDockingInCentralNode; }
            if (ImGui::MenuItem("Flag: AutoHideTabBar",         "", (dockspace_flags & ImGuiDockNodeFlags_AutoHideTabBar) != 0))          { dockspace_flags ^= ImGuiDockNodeFlags_AutoHideTabBar; }
            if (ImGui::MenuItem("Flag: PassthruCentralNode",    "", (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode) != 0, opt_fullscreen)) { dockspace_flags ^= ImGuiDockNodeFlags_PassthruCentralNode; }
            ImGui::Separator();

            if (ImGui::MenuItem("Close", NULL, false, p_open != NULL))
                p_open = false;
            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }

    uint32_t textureID = m_framebuffer->getColorAttachment();

    ImGui::Image((void*)textureID,ImVec2{320.f,180.f});
    ImGui::End();
}

void SandBox2D::OnAttach()
{
    RE_PROFILE_FUNCTION();

    // m_texture = Rengin::Texture2D::Create("D:\\secret\\109951166175641343.jpg");
    m_texture = Rengin::Texture2D::Create("../../../SandBox/assets/textures/France.jpg");
    
    Rengin::FrameBufferSpecification FbSpec;
    FbSpec.Width = 1280;
    FbSpec.Height = 720;
    m_framebuffer = Rengin::FrameBuffer::Create(FbSpec);
}

void SandBox2D::OnDetach()
{
    RE_PROFILE_FUNCTION();

}

void SandBox2D::OnEvent(Rengin::Event& ev)
{
    m_camera_controller.OnEvent(ev);
}