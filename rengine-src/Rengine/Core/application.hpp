#pragma once
#include "core.hpp"
#include "Window.hpp"
#include "Rengine/Event/ApplicationEvent.hpp"
#include "LayerStack.hpp"
#include "Rengine/ImGui/ImGuiLayer.hpp"
#include "Rengine/Renderer/Shader.hpp"
#include "Rengine/Renderer/Buffer.hpp"
#include "Rengine/Renderer/VertexArray.hpp"
#include "Rengine/Renderer/OrthoGraphicsCamera.hpp"
#include "TimeStep.hpp"

namespace Rengin
{

class RE_API Application
{
private:
    Scope<Window> m_window;
    float m_last_frame_time = 0.0f;
    ImGuiLayer* m_imgui_layer;
    bool m_minimized = false;
    bool m_running = true;
    LayerStack m_layer_stack;
public:
    Application();

    virtual ~Application();

    void Run();

    void OnEvent(Event& e);

    void PushLayer(Layer* layer);
    void PushOverLayer(Layer* layer);

    inline static Application& getApplication() {return *m_instance;}
    inline Window& getWindow() {return *m_window;}
private:
    bool OnWindowClose(WindowCloseEvent& ev);
    bool OnWindowResize(WindowResizeEvent& ev);
    static Application* m_instance;
};

Application* CreateApplication();
}
