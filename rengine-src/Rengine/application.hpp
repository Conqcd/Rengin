#pragma once
#include "core.hpp"
#include "Window.hpp"
#include "Event/ApplicationEvent.hpp"
#include "LayerStack.hpp"
#include "ImGui/ImGuiLayer.hpp"
#include "Renderer/Shader.hpp"
#include "Renderer/Buffer.hpp"

namespace Rengin
{

class RE_API Application
{
private:
    std::unique_ptr<Window> m_window;
    std::unique_ptr<Shader> m_shader;
    std::unique_ptr<VertexBuffer> m_verbuf;
    std::unique_ptr<IndexBuffer> m_indbuf;
    ImGuiLayer* m_imgui_layer;
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
    static Application* m_instance;
};

Application* CreateApplication();
}
