#include "EditorLayer.hpp"
#include <chrono>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Rengine/Scene/SceneSerializer.hpp"
namespace Rengin
{
    

EditorLayer::EditorLayer(/* args */)
        :Layer("Editor"),m_camera_controller(1280.f/720.f,true)
{
}

EditorLayer::~EditorLayer()
{
}

void EditorLayer::OnUpdate(TimeStep timestep)
{
    RE_PROFILE_FUNCTION();
    if(FrameBufferSpecification spec = m_framebuffer->getSpecification();
        m_ViewPortSize.x > 0.0f && m_ViewPortSize.x > 0.0f && 
        (spec.Width != m_ViewPortSize.x || spec.Height != m_ViewPortSize.y))
    {
        m_framebuffer->Resize(static_cast<uint32_t>(m_ViewPortSize.x),static_cast<uint32_t>(m_ViewPortSize.y));
        m_camera_controller.OnResize(m_ViewPortSize.x,m_ViewPortSize.y);

        m_ActiveScene->OnViewportResize(static_cast<uint32_t>(m_ViewPortSize.x),static_cast<uint32_t>(m_ViewPortSize.y));
    }

    if(m_ViewportFocused)
        m_camera_controller.OnUpdate(timestep);
    
    Renderer2D::resetStats();

    m_framebuffer->Bind();
    RenderCommand::SetClearColor({0.1f,0.1f,0.1f,1});
    RenderCommand::Clear();

    Renderer2D::BeginScene(m_camera_controller.getCamera());

    //Update Scene
    // m_ActiveScene->OnUpdate(timestep);

    Renderer2D::EndScene();
    
    m_framebuffer->Unbind();
}

void EditorLayer::OnImGuiRender()
{
    RE_PROFILE_FUNCTION();


    static bool p_open = true;
    static bool opt_fullscreen = true;
    static bool opt_padding = false;
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

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

    if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
        window_flags |= ImGuiWindowFlags_NoBackground;

    if (!opt_padding)
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("DockSpace Demo", &p_open, window_flags);
    if (!opt_padding)
        ImGui::PopStyleVar();

    if (opt_fullscreen)
        ImGui::PopStyleVar(2);

    // Submit the DockSpace
    ImGuiIO& io = ImGui::GetIO();
    ImGuiStyle& style = ImGui::GetStyle();
    float MinWinSize = style.WindowMinSize.x;
    style.WindowMinSize.x = 370.f;
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
    {
        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
    }
    style.WindowMinSize.x = MinWinSize;

    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            // Disabling fullscreen would allow the window to be moved to the front of other windows,
            // which we can't undo at the moment without finer window depth/z control.
            if(ImGui::MenuItem("Serialize"))
            {
                SceneSerializer serializer(m_ActiveScene);
                serialzer.Serialize("");
            }
            if(ImGui::MenuItem("Deserialize"))
            {
                SceneSerializer serializer(m_ActiveScene);
                serialzer.Serialize("");
            }

            if (ImGui::MenuItem("Exit"))
                Application::getApplication().Close();
            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }

    m_panel.OnImGuiRender();

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding,ImVec2{0,0});
    ImGui::Begin("ViewPort");
    uint32_t textureID = m_framebuffer->getColorAttachment();

    m_ViewportFocused = ImGui::IsWindowFocused();
    m_ViewportHovered = ImGui::IsWindowHovered();
    Application::getApplication().getImGuiLayer()->SetBlockEvents(!m_ViewportFocused && !m_ViewportHovered);

    ImVec2 vps = ImGui::GetContentRegionAvail();
    if(*((glm::vec2*)&vps) != m_ViewPortSize && vps.x > 0 && vps.y > 0)
    {
        m_ViewPortSize = {vps.x,vps.y};
        m_framebuffer->Resize(static_cast<uint32_t>(vps.x),static_cast<uint32_t>(vps.y));
        m_camera_controller.OnResize(vps.x,vps.y);
    }
    ImGui::Image((void*)textureID,vps);
    
    ImGui::PopStyleVar();
    ImGui::End();
    ImGui::End();

    ImGui::Begin("Settings");

    auto stats = Rengin::Renderer2D::getStats();
    ImGui::Text("Renderer2D Stats:");
    ImGui::Text("Call Draw: %d",stats.DrawCall);
    ImGui::Text("Quads: %d",stats.QuadCount);
    ImGui::Text("Vertices: %d",stats.GetTotalVertexCount());
    ImGui::Text("Indices: %d",stats.GetTotalIndexCount());

    ImGui::End();
}

void EditorLayer::OnAttach()
{
    RE_PROFILE_FUNCTION();

    m_texture = Texture2D::Create("../../../Regine-Editor/assets/textures/France.jpg");
    
    FrameBufferSpecification FbSpec;
    FbSpec.Width = 1280;
    FbSpec.Height = 720;
    m_framebuffer = FrameBuffer::Create(FbSpec);

    m_ActiveScene = CreateRef<Scene>();
    
    m_SquareEntity = m_ActiveScene->CreateEntity("Square");
    m_SquareEntity.AddComponent<SpriteRendererComponent>(glm::vec4{0.0f,1.0f,0.0f,1.0f});

    
    m_Camera = m_ActiveScene->CreateEntity("Camera");
    // m_Camera.AddComponent<CameraComponent>(glm::ortho(-16.0f,16.0f,-9.0f,9.0f,-1.0f,1.0f));
    m_Camera.AddComponent<CameraComponent>();


    class CameraController :public ScriptableEntity
    {
    public:
        void OnUpdate(TimeStep ts) {
            std::cout << "OnUpdate :" << ts << std::endl;
        }
        void OnDestroy() {}
        void OnCreate() {
            // GetComponent<TransformComponent>();
            std::cout << "OnCreate :"  << std::endl;
        }
    };

    m_Camera.AddComponent<NativeScriptComponent>().Bind<CameraController>();

    m_panel.SetContext(m_ActiveScene);

    SceneSerializer serializer(m_ActiveScene);
    serializer.Serializer("../../../Rengine-Editor/assets/scenes/Example.yaml");
}

void EditorLayer::OnDetach()
{
    RE_PROFILE_FUNCTION();

}

void EditorLayer::OnEvent(Event& ev)
{
    m_camera_controller.OnEvent(ev);
}

} // namespace Rengin