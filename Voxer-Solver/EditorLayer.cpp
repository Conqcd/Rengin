#include "EditorLayer.hpp"
#include <chrono>
#include "Rengine/Platform/OpenGL/OpenGLTexture.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Rengine/Scene/SceneSerializer.hpp"
#include "Rengine/Utils/PlatformUtils.hpp"
#include <ImGuizmo.h>
#include <Rengine/Math/Math.hpp>

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
        m_EditorCamera.SetViewportSize(m_ViewPortSize.x,m_ViewPortSize.y);
        m_ActiveScene->OnViewportResize(static_cast<uint32_t>(m_ViewPortSize.x),static_cast<uint32_t>(m_ViewPortSize.y));
    }

    if (m_ViewportFocused)
    {
        m_camera_controller.OnUpdate(timestep);
        m_EditorCamera.OnUpdate(timestep);
    }

    Renderer2D::resetStats();

    m_framebuffer->Bind();
    RenderCommand::SetClearColor({0.1f,0.1f,0.1f,1});
    RenderCommand::Clear();

    //Update Scene
    m_ActiveScene->OnUpdateRuntime(timestep);
    // m_ActiveScene->OnUpdateEditor(timestep,m_EditorCamera);

    
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
            if(ImGui::MenuItem("New","Ctrl+N"))
            {
                NewScene();
            }
            if(ImGui::MenuItem("Open...","Ctrl+O"))
            {
                OpenScene();
            }
            if(ImGui::MenuItem("Save as ...","Ctrl+Shift+S"))
            {
                SaveSceneAs();
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
    // textureID = std::dynamic_pointer_cast<OpenGLTexture2D>(m_texture)->getRendererID();

    m_ViewportFocused = ImGui::IsWindowFocused();
    m_ViewportHovered = ImGui::IsWindowHovered();
    Application::getApplication().getImGuiLayer()->SetBlockEvents(!m_ViewportFocused && !m_ViewportHovered);

    ImVec2 vps = ImGui::GetContentRegionAvail();
    if(*((glm::vec2*)&vps) != m_ViewPortSize && vps.x > 0 && vps.y > 0)
    {
        m_ViewPortSize = {vps.x,vps.y};
        // m_framebuffer->Resize(static_cast<uint32_t>(vps.x),static_cast<uint32_t>(vps.y));
        // m_camera_controller.OnResize(vps.x,vps.y);
    }
    ImGui::Image((void*)textureID,vps,ImVec2{0,1},ImVec2{1,0});

    //Gizmos
    // Entity selectedEntity = m_panel.GetSelectedEntity();
    // if (selectedEntity || m_GizmoType != -1)
    // {
    //     ImGuizmo::SetOrthographic(false);
    //     ImGuizmo::SetDrawlist();
    //     float windowWidth = static_cast<float>(ImGui::GetWindowWidth());
    //     float windowHeight = static_cast<float>(ImGui::GetWindowHeight());
    //     ImGuizmo::SetRect(ImGui::GetWindowPos().x,ImGui::GetWindowPos().y,windowWidth,windowHeight);

    //     //Camera
    //     auto cameraEntity = m_ActiveScene->GetPrimaryCameraEntity();
    //     const auto& camera = cameraEntity.GetComponent<CameraComponent>().Camera;
    //     const glm::mat4& cameraProjection = camera.getProjection();
    //     glm::mat4 cameraView = glm::inverse(cameraEntity.GetComponent<TransformComponent>().GetTransform());

        //EditorCamera
        // const glm::mat4& cameraProjection = m_EditorCamera.getProjection();
        // glm::mat4 cameraView = m_EditorCamera.GetViewMatrix();


        // auto &tc = selectedEntity.GetComponent<TransformComponent>();
        // glm::mat4 transform = tc.GetTransform();

        // bool snap = Input::isKeyPressed(static_cast<int>(Key::LeftControl));
        // float snapValue = 0.5f;
        // if(m_GizmoType == ImGuizmo::OPERATION::ROTATE)
        //     snapValue = 45.0f;
        // float snapValues[3] = {snapValue, snapValue, snapValue};

        // ImGuizmo::Manipulate(
        //     glm::value_ptr(cameraView), glm::value_ptr(cameraProjection),
        //     static_cast<ImGuizmo::OPERATION>(m_GizmoType), ImGuizmo::LOCAL,
        //     glm::value_ptr(transform), nullptr, snap ? snapValues : nullptr);

        // if (ImGuizmo::IsUsing()) {
        //   glm::vec3 translation, rotation, scale;
        //   Math::DecomposeTransform(transform, translation, rotation, scale);
        //   glm::vec3 deltaRotation = rotation - tc.Rotation;
        //   tc.Translation = translation;
        //   tc.Rotation += deltaRotation;
        //   tc.Scale = scale;
        // }
    // }

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

    m_texture = Texture2D::Create("assets/textures/France.jpg");
    
    FrameBufferSpecification FbSpec;
    FbSpec.Attachments = {FramebufferTextureFormat::RGBA8 , FramebufferTextureFormat::Depth};
    m_ViewPortSize.x = FbSpec.Width = 1280;
    m_ViewPortSize.y = FbSpec.Height = 720;
    m_framebuffer = FrameBuffer::Create(FbSpec);

    m_ActiveScene = CreateRef<Scene>();
    
    m_EditorCamera = EditorCamera(30.0f,1.778f,0.1f,1000.0f);

    m_ActiveScene->OnViewportResize(static_cast<uint32_t>(m_ViewPortSize.x),static_cast<uint32_t>(m_ViewPortSize.y));
    
    m_SquareEntity = m_ActiveScene->CreateEntity("Square");
    m_SquareEntity.AddComponent<SpriteRendererComponent>(glm::vec4{0.0f,1.0f,0.0f,1.0f});

    Ref<Texture3D> texture_v = Texture3D::Create("assets/textures/cbct_sample_z=507_y=512_x=512.raw");
    auto m_CubeEntity = m_ActiveScene->CreateEntity("Volume");
    m_CubeEntity.AddComponent<Texture3DComponent>(texture_v);
    std::vector<float> l1{0.f,1.f},l2{0.f,1.f},l3{0.0f,0.5f,1.0f};
    std::vector<glm::vec3> l4{{1.0f,0.0f,0.0f},{1.0f,1.0f,1.0f},{0.0f,0.0f,1.0f}};
    // std::vector<float> l1{0.f,1.f},l2{0.f,1.f},l3{0.0f,1.0f};
    // std::vector<glm::vec3> l4{{1.0f,0.0f,0.0f},{0.0f,0.0f,1.0f}};
    TransferFunction<float, float> t1{l1,l2};
    TransferFunction<float, glm::vec3> t2{l3,l4};

    m_CubeEntity.AddComponent<OpacityTransferFunctionComponent>(t1);
    // m_CubeEntity.AddComponent<OpacityTransferFunctionComponent>({0.0,1.0},{0.0,1.0});
    m_CubeEntity.AddComponent<ColorTransferFunctionComponent>(t2);
    // m_CubeEntity.AddComponent<ColorTransferFunctionComponent>({0.0,1.0},{{1.0,0.0,0.0},{0.0,0.0,1.0}});
    auto& texCom = m_CubeEntity.GetComponent<Texture3DComponent>();
    texCom.Path = "E:\\Volume_Rendering\\raw_data\\cbct_sample_z=507_y=512_x=512.raw";

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

    // SceneSerializer serializer(m_ActiveScene);
    // serializer.Serializer("../../../Rengine-Editor/assets/scenes/Example.yaml");
}

void EditorLayer::OnDetach()
{
    RE_PROFILE_FUNCTION();

}

void EditorLayer::OnEvent(Event& ev)
{
    m_camera_controller.OnEvent(ev);
    m_EditorCamera.OnEvent(ev);
    EventDispatcher dispatcher(ev);
    dispatcher.Dispatch<KeyPressEvent>(RE_BIND_FUNC_EVENT_1(EditorLayer::OnKeyPressed));
}

bool EditorLayer::OnKeyPressed(KeyPressEvent& e)
{
    if (e.getRepeatCount() > 0)
        return false;
    bool control = Input::isKeyPressed(static_cast<int>(KeyCode::LeftControl)) || Input::isKeyPressed(static_cast<int>(KeyCode::RightControl));
    bool shift = Input::isKeyPressed(static_cast<int>(KeyCode::LeftShift)) || Input::isKeyPressed(static_cast<int>(KeyCode::RightShift));

    switch (static_cast<KeyCode>(e.getKeyValue()))
    {
    case KeyCode::N:
        if (control)
            NewScene();
        
        break;
    case KeyCode::O:
        if (control)
            OpenScene();
        break;
    case KeyCode::S:
        if (control && shift)
            SaveSceneAs();
        break;
    case KeyCode::Q:
      m_GizmoType = -1;
      break;
    case KeyCode::W:
      m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;
      break;
    case KeyCode::E:
      m_GizmoType = ImGuizmo::OPERATION::ROTATE;
      break;
    case KeyCode::R:
      m_GizmoType = ImGuizmo::OPERATION::SCALE;
      break;
    default:
      break;
    }
}

void EditorLayer::NewScene()
{
    m_ActiveScene = CreateRef<Scene>();
    m_ActiveScene->OnViewportResize(static_cast<uint32_t>(m_ViewPortSize.x), static_cast<uint32_t>(m_ViewPortSize.y));
    m_panel.SetContext(m_ActiveScene);
}

void EditorLayer::OpenScene()
{
    std::string filepath = FileDialogs::OpenFile("Rengine Scene (*.yaml)\0*.yaml\0");
    if (!filepath.empty())
    {
        m_ActiveScene = CreateRef<Scene>();
        m_ActiveScene->OnViewportResize(static_cast<uint32_t>(m_ViewPortSize.x), static_cast<uint32_t>(m_ViewPortSize.y));
        m_panel.SetContext(m_ActiveScene);
        SceneSerializer serializer(m_ActiveScene);
        serializer.Deserializer(filepath);
    }
}

void EditorLayer::SaveSceneAs()
{
    std::string filepath = FileDialogs::SaveFile("Rengine Scene (*.yaml)\0*.yaml\0");
    if (!filepath.empty())
    {
        SceneSerializer serializer(m_ActiveScene);
        serializer.Serializer(filepath);
    }
}

} // namespace Rengin