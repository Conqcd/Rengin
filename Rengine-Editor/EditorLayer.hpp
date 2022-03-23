#pragma once
#include <rengine.hpp>
#include <imgui.h>
#include "Panels/SceneHierarchyPanel.hpp"
#include "Panels/ContentBrowserPanel.hpp"
#include "Rengine/Renderer/EditorCamera.hpp"

namespace Rengin
{
class EditorLayer : public Layer
{
private:
    ShaderLibrary m_shader_lib;
    Ref<Shader> m_shader;
    // Ref<ObjManager> m_Obj;
    Ref<RendererObject> m_RenderObj;
    SkyBox m_SkyBox;

    Ref<VertexBuffer> m_verbuf;
    Ref<IndexBuffer> m_indbuf;
    Ref<VertexArray> m_verarr;
    Ref<Texture2D> m_texture;
    Ref<FrameBuffer> m_framebuffer;
    OrthoGraphicsCameraController m_camera_controller;

    Ref<Scene> m_ActiveScene;
    Ref<Scene> m_EditorScene;
    std::filesystem::path m_EditorScenePath;

    EditorCamera m_EditorCamera;

    Entity m_SquareEntity;
    Entity m_HoverEntity = {};
    Entity m_Camera;

    glm::vec2 m_ViewPortSize;
    glm::vec2 m_ViewPortBounds[2];
    bool m_ViewportFocused = false;
    bool m_ViewportHovered = false;

    int  m_GizmoType = -1;

    //Editor2D
    Ref<Texture2D> m_IconPlay,m_IconStop;
    //Panels
    SceneHierarchyPanel m_panel;
    ContentBrowserPanel m_ContentBrowserPanel;

    enum class SceneState
    {
        Edit = 0,Play = 1
    };

    SceneState m_SceneState = SceneState::Edit;
private:
    bool OnMouseButtonPressed(MouseButtonPressEvent& e);
    bool OnKeyPressed(KeyPressEvent& e);

    void OnOverlayRender();

    void NewScene();
    void OpenScene();
    void OpenScene(const std::filesystem::path& path);
    void SaveScene();
    void SaveSceneAs();

    void SerializeScene(Ref<Scene> scene,const std::filesystem::path& path);

    void OnScenePlay();
    void OnSceneStop();

    void OnDuplicateEntity();

    //UI Panels
    void UI_Toolbar();
public:
    EditorLayer(/* args */);
    ~EditorLayer();
    void OnUpdate(TimeStep timestep) override;
    virtual void OnImGuiRender() override;
    virtual void OnAttach()override;
    virtual void OnDetach()override;
    void OnEvent(Event& ev) override;
};


} // namespace Rengin

