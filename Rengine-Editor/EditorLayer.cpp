#include "EditorLayer.hpp"
#include <chrono>
#include "Rengine/Platform/OpenGL/OpenGLTexture.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Rengine/Utils/PlatformUtils.hpp"
#include <ImGuizmo.h>
#include <Rengine/Math/Math.hpp>
#include "Rengine/Renderer/Methods/PhongMethod.hpp";
#include "Rengine/Renderer/Methods/ShadowMapMethod.hpp";
#include "Rengine/Renderer/Methods/PRTMethod.hpp";

namespace Rengin
{
extern const std::filesystem::path g_AssetPath;

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

    //Render    
    Renderer2D::resetStats();
    m_framebuffer->Bind();
    RenderCommand::SetClearColor({0.1f,0.1f,0.1f,1});
    RenderCommand::Clear();

    //Clear framebuffer id to -1
    int values = -1;
    m_framebuffer->ClearAttachment(1,&values);

    switch (m_SceneState)
    {
    case SceneState::Edit:
        if (m_ViewportFocused) {
            m_camera_controller.OnUpdate(timestep);
        }
        m_EditorCamera.OnUpdate(timestep);
        // m_ActiveScene->OnUpdateEditor(timestep, m_EditorCamera);
        m_SkyBox.RenderCube(4,m_EditorCamera);
        m_RenderObj->DrawObject("ShadowMap",{0,1},m_EditorCamera);
        break;
    case SceneState::Play:
        m_ActiveScene->OnUpdateRuntime(timestep);
        break;
    default:
        break;
    }

    //Update Scene
    auto [mx,my] = ImGui::GetMousePos();
    mx -= m_ViewPortBounds[0].x;
    my -= m_ViewPortBounds[0].y;
    auto viewportSize = m_ViewPortBounds[1] - m_ViewPortBounds[0];
    my = viewportSize.y - my;
    int mouseX = static_cast<int>(mx);
    int mouseY = static_cast<int>(my);
    
    if(mouseX >= 0 && mouseX < (int)viewportSize.x && mouseY >= 0 && mouseY < (int)viewportSize.y)
    {
        int pixelData;
        m_framebuffer->ReadPixel(1,mouseX,mouseY,&pixelData);
        RE_CORE_WARN("pixel data {0}",pixelData);
        // if(pixelData == -1)
        //     m_HoverEntity = {};
        // else
        //     m_HoverEntity = {(entt::entity)pixelData,m_ActiveScene.get()};
    }
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
    m_ContentBrowserPanel.OnImGuiRender();

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding,ImVec2{0,0});
    ImGui::Begin("ViewPort");
    auto viewportOffset = ImGui::GetCursorPos();
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

    if(ImGui::BeginDragDropTarget())
    {
        if(const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
        {
          const wchar_t *path = (const wchar_t *)payload->Data;
          OpenScene(std::filesystem::path(g_AssetPath) / path);
        }
        ImGui::EndDragDropTarget();
    }

    auto windowSize = ImGui::GetWindowSize();
    ImVec2 minBound = ImGui::GetWindowPos();
    minBound.x += viewportOffset.x;
    minBound.y += viewportOffset.y;

    ImVec2 maxBound = {minBound.x + windowSize.x,minBound.y + windowSize.y};
    m_ViewPortBounds[0] = {minBound.x,minBound.y};
    m_ViewPortBounds[1] = {maxBound.x,maxBound.y};

    RE_CORE_WARN("Min Bounds = {0} {1}",minBound.x,minBound.y);
    RE_CORE_WARN("Max Bounds = {0} {1}",maxBound.x,maxBound.y);

    //Gizmos
    Entity selectedEntity = m_panel.GetSelectedEntity();
    if (selectedEntity || m_GizmoType != -1)
    {
        ImGuizmo::SetOrthographic(false);
        ImGuizmo::SetDrawlist();
        float windowWidth = static_cast<float>(ImGui::GetWindowWidth());
        float windowHeight = static_cast<float>(ImGui::GetWindowHeight());
        ImGuizmo::SetRect(ImGui::GetWindowPos().x,ImGui::GetWindowPos().y,windowWidth,windowHeight);

        //Camera
        auto cameraEntity = m_ActiveScene->GetPrimaryCameraEntity();
        const auto& camera = cameraEntity.GetComponent<CameraComponent>().Camera;
        const glm::mat4& cameraProjection = camera.getProjection();
        glm::mat4 cameraView = glm::inverse(cameraEntity.GetComponent<TransformComponent>().GetTransform());

        //EditorCamera
        // const glm::mat4& cameraProjection = m_EditorCamera.getProjection();
        // glm::mat4 cameraView = m_EditorCamera.GetViewMatrix();


        auto &tc = selectedEntity.GetComponent<TransformComponent>();
        glm::mat4 transform = tc.GetTransform();

        bool snap = Input::isKeyPressed(static_cast<int>(Key::LeftControl));
        float snapValue = 0.5f;
        if(m_GizmoType == ImGuizmo::OPERATION::ROTATE)
            snapValue = 45.0f;
        float snapValues[3] = {snapValue, snapValue, snapValue};

        ImGuizmo::Manipulate(
            glm::value_ptr(cameraView), glm::value_ptr(cameraProjection),
            static_cast<ImGuizmo::OPERATION>(m_GizmoType), ImGuizmo::LOCAL,
            glm::value_ptr(transform), nullptr, snap ? snapValues : nullptr);

        if (ImGuizmo::IsUsing()) {
          glm::vec3 translation, rotation, scale;
          Math::DecomposeTransform(transform, translation, rotation, scale);
          glm::vec3 deltaRotation = rotation - tc.Rotation;
          tc.Translation = translation;
          tc.Rotation += deltaRotation;
          tc.Scale = scale;
        }
    }

    ImGui::PopStyleVar();
    ImGui::End();
    ImGui::End();

    // ImGui::Begin("Settings");

    // auto stats = Rengin::Renderer2D::getStats();
    // ImGui::Text("Renderer2D Stats:");
    // ImGui::Text("Call Draw: %d",stats.DrawCall);
    // ImGui::Text("Quads: %d",stats.QuadCount);
    // ImGui::Text("Vertices: %d",stats.GetTotalVertexCount());
    // ImGui::Text("Indices: %d",stats.GetTotalIndexCount());

    // UI_Toolbar();

    // ImGui::End();
}

void EditorLayer::OnAttach()
{
    RE_PROFILE_FUNCTION();

    m_shader = Shader::Create("../../../Rengine-Editor/assets/shaders/BaseVertex.glsl","../../../Rengine-Editor/assets/shaders/BaseFragment.glsl");
    m_RenderObj = CreateRef<RendererObject>();
    glm::mat4 transform1(1.0f);
    transform1 = glm::scale(transform1,glm::vec3(20.f,20.f,20.f));
    m_RenderObj->AddObj(CreateRef<PRTObjManager>("./assets/objects/mary/mary.obj","./assets/objects/mary",transform1));
    // m_RenderObj->AddObj(CreateRef<ObjManager>("./assets/objects/mary/mary.obj","./assets/objects/mary",transform1));
    glm::mat4 transform2(1.0f);
    transform2 = glm::translate(transform2,glm::vec3(40.f,0.f,-40.f));
    transform2 = glm::scale(transform2,glm::vec3(10.f,10.f,10.f));
    // m_RenderObj->AddObj(CreateRef<ObjManager>("./assets/objects/mary/mary.obj","./assets/objects/mary",transform2));
    glm::mat4 transform3(1.0f);
    transform3 = glm::translate(transform3,glm::vec3(0.f,0.f,-30.f));
    transform3 = glm::scale(transform3,glm::vec3(4.f,4.f,4.f));
    m_RenderObj->AddObj(CreateRef<PRTObjManager>("./assets/objects/floor/floor.obj","./assets/objects/floor",transform3));
    // m_RenderObj->AddObj(CreateRef<ObjManager>("./assets/objects/floor/floor.obj","./assets/objects/floor",transform3));

    auto phongMethod = CreateRef<PhongMethod>();
    phongMethod->AddResource(m_shader);
    m_RenderObj->AddMethod("BlinnPhong",phongMethod);

    //Texture
    m_texture = Texture2D::Create("assets/textures/France.jpg");
    m_IconPlay = Texture2D::Create("assets/textures/France.jpg");
    m_IconStop = Texture2D::Create("assets/textures/France.jpg");
    
    //FrameBuffer
    FrameBufferSpecification FbSpec;
    FbSpec.Attachments = {FramebufferTextureFormat::RGBA8,FramebufferTextureFormat::RED_INTEGER , FramebufferTextureFormat::Depth};
    m_ViewPortSize.x = FbSpec.Width = 1280;
    m_ViewPortSize.y = FbSpec.Height = 720;
    m_framebuffer = FrameBuffer::Create(FbSpec);

    m_ActiveScene = CreateRef<Scene>();
 
    m_EditorCamera = EditorCamera(30.0f,1.778f,0.1f,1000.0f);

    m_ActiveScene->OnViewportResize(static_cast<uint32_t>(m_ViewPortSize.x),static_cast<uint32_t>(m_ViewPortSize.y));

    m_SquareEntity = m_ActiveScene->CreateEntity("Square");
    m_SquareEntity.AddComponent<SpriteRendererComponent>(glm::vec4{0.0f,1.0f,0.0f,1.0f});

    m_Camera = m_ActiveScene->CreateEntity("Camera");
    // m_Camera.AddComponent<CameraComponent>(glm::ortho(-16.0f,16.0f,-9.0f,9.0f,-1.0f,1.0f));
    m_Camera.AddComponent<CameraComponent>();


    // Render 
    FrameBufferSpecification FbSpecShadow;
    FbSpecShadow.Attachments = {FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::Depth};
    FbSpecShadow.Width = 12800;
    FbSpecShadow.Height = 7200;
    auto ShadowFrame = FrameBuffer::Create(FbSpecShadow);

    auto shadowMapMethod = CreateRef<ShadowMapMethod>();
    auto shadowShader = Shader::Create("../../../Rengine-Editor/assets/shaders/ShadowMapV.glsl","../../../Rengine-Editor/assets/shaders/ShadowMapF.glsl");
    auto shadowPhongShader = Shader::Create("../../../Rengine-Editor/assets/shaders/ShadowPhongV.glsl","../../../Rengine-Editor/assets/shaders/ShadowPhongF.glsl");
    shadowMapMethod->AddResource(shadowPhongShader,shadowShader);
    shadowMapMethod->AddResource(ShadowFrame,m_framebuffer);
    m_RenderObj->AddMethod("ShadowMap",shadowMapMethod);
    

    //      SkyBox
    auto SkyboxShader = Shader::Create("../../../Rengine-Editor/assets/shaders/SkyBoxVertex.glsl","../../../Rengine-Editor/assets/shaders/SkyBoxFragment.glsl");
    m_SkyBox.SetShader(SkyboxShader);
    m_SkyBox.AddCubeMap("../../../Rengine-Editor/assets/cubemap/CornellBox/");
    m_SkyBox.AddCubeMap("../../../Rengine-Editor/assets/cubemap/GraceCathedral/");
    m_SkyBox.AddCubeMap("../../../Rengine-Editor/assets/cubemap/Indoor/");
    m_SkyBox.AddCubeMap("../../../Rengine-Editor/assets/cubemap/Skybox/");
    m_SkyBox.AddCubeMap("../../../Rengine-Editor/assets/cubemap/Skybox2/");

    // PRT
    auto prtMethod = CreateRef<PRTMethod>();
    auto prtShader = Shader::Create("../../../Rengine-Editor/assets/shaders/ShadowMapV.glsl","../../../Rengine-Editor/assets/shaders/ShadowMapF.glsl");
    prtMethod->AddResource(prtShader,SkyboxShader);
    m_RenderObj->AddMethod("PRT",prtMethod);

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
    dispatcher.Dispatch<MouseButtonPressEvent>(RE_BIND_FUNC_EVENT_1(EditorLayer::OnMouseButtonPressed));
}

bool EditorLayer::OnMouseButtonPressed(MouseButtonPressEvent& e) {
    if(e.getButton() == Mouse::ButtonLeft)
    {
        if(m_HoverEntity && !ImGuizmo::IsOver() && !Input::isKeyPressed(static_cast<int>(Key::LeftAlt)))
          m_panel.SetSelectedEntity(m_HoverEntity);
    }
    return false;
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
        if (control)
            if(shift)
                SaveSceneAs();
            else
                SaveScene();
        break;
    case KeyCode::D:
        if (control)
            OnDuplicateEntity();
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
    return false;
}

void EditorLayer::NewScene()
{
    m_ActiveScene = CreateRef<Scene>();
    m_ActiveScene->OnViewportResize(static_cast<uint32_t>(m_ViewPortSize.x), static_cast<uint32_t>(m_ViewPortSize.y));
    m_panel.SetContext(m_ActiveScene);
    m_EditorScenePath = std::filesystem::path();
}

//Render Collider Rect
void EditorLayer::OnOverlayRender()
{
    auto view = m_ActiveScene->GetAllEntitiesWidth<CircleRendererComponent>();
}

void EditorLayer::OpenScene()
{
    std::string filepath = FileDialogs::OpenFile("Rengine Scene (*.yaml)\0*.yaml\0");
    if (!filepath.empty())
    {
        OpenScene(filepath);
    }
}

void EditorLayer::OpenScene(const std::filesystem::path& path)
{
    if(m_SceneState == SceneState::Edit)
        OnSceneStop();
 
    if(path.extension() != ".yaml")
    {
        RE_WARN("Could not load {0} - not a scene file!",path.filename().string());
        return;
    }
    Ref<Scene> newScene = CreateRef<Scene>();
    SceneSerializer serializer(m_ActiveScene);

    if(serializer.Deserializer(path.string()))
    {
        m_EditorScene = newScene;
        m_ActiveScene->OnViewportResize(static_cast<uint32_t>(m_ViewPortSize.x),static_cast<uint32_t>(m_ViewPortSize.y));
        m_panel.SetContext(m_EditorScene);

        m_ActiveScene = m_EditorScene;
        m_EditorScenePath = path;
    }
}

void EditorLayer::SaveSceneAs()
{
    std::string filepath = FileDialogs::SaveFile("Rengine Scene (*.yaml)\0*.yaml\0");
    if (!filepath.empty())
    {
        m_EditorScenePath = filepath;
        SerializeScene(m_ActiveScene,m_EditorScenePath);
    }
}

void EditorLayer::SerializeScene(Ref<Scene> scene, const std::filesystem::path &path)
{
    SceneSerializer serializer(scene);
    serializer.Serializer(path.string());
}

void EditorLayer::SaveScene()
{
    if(!m_EditorScenePath.empty())
        SerializeScene(m_ActiveScene,m_EditorScenePath);
}

void EditorLayer::OnScenePlay()
{
    m_SceneState = SceneState::Play;

    m_ActiveScene = Scene::Copy(m_EditorScene);
    m_ActiveScene->OnRuntimeStart();

    m_panel.SetContext(m_ActiveScene);
}

void EditorLayer::OnSceneStop()
{
    m_SceneState = SceneState::Edit;
    m_ActiveScene->OnRuntimeStop();

    m_ActiveScene = m_EditorScene;

    m_panel.SetContext(m_ActiveScene);
}

void EditorLayer::OnDuplicateEntity()
{
    if(m_SceneState != SceneState::Edit)
        return;

    auto slEntity = m_panel.GetSelectedEntity();
    if(slEntity)
        m_EditorScene->DuplicateEntity(slEntity);
}

void EditorLayer::UI_Toolbar()
{
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding,ImVec2(0,2));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing,ImVec2(0,0));
    ImGui::PushStyleColor(ImGuiCol_Button,ImVec4(0,0,0,0));
    auto& color = ImGui::GetStyle().Colors;
    const auto& buttonHovered = color[ImGuiCol_ButtonHovered];
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered,ImVec4(buttonHovered.x,buttonHovered.y,buttonHovered.z,0.5f));
    const auto& buttonActive = color[ImGuiCol_ButtonActive];
    ImGui::PushStyleColor(ImGuiCol_ButtonActive,ImVec4(buttonActive.x,buttonActive.y,buttonActive.z,0.5f));

    ImGui::Begin("##toolbar",nullptr,ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
    
    float size = ImGui::GetWindowHeight() - 4.0f;
    auto icon = m_SceneState == SceneState::Edit? m_IconStop : m_IconPlay;
    ImGui::SetCursorPosX((ImGui::GetWindowContentRegionMax().x * 0.5f - (size * 0.5f)));
    if(ImGui::ImageButton((ImTextureID)icon->getRendererID(),ImVec2(size,size),ImVec2(0,0),ImVec2(1,1),0))
    {
        if(m_SceneState == SceneState::Edit)
            OnScenePlay();
        else if(m_SceneState == SceneState::Play)
            OnSceneStop();
    }

    ImGui::End();
}

} // namespace Rengin